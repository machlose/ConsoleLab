#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#define CONSOLELAB_VERSION_MAJOR 0
#define CONSOLELAB_VERSION_MINOR 0
// #define CONSOLELAB_INCLUDE_IMPLEMENTATION
#ifdef _WIN32
#include <windows.h>
#else
//includes for other platforms if I will extend it to them in the future
#endif

#include <stdlib.h>
#include <stdio.h>
#include <uchar.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    union {
        struct { int x, y; };
        struct { int w, k; };
        struct { int width, height; };
        int arr[2];
    };
} vec2i;

typedef struct {
    union {
        struct { int x, y, z; };
        struct { int w, k, d; };
        struct { int width, height, depth; };
        int arr[3];
    };
} vec3i;

typedef struct {
    union {
        struct { uint8_t r, g, b, a; };
        uint8_t arr[4];
    };
} Color;

typedef enum {
    WHITE_C = 0,
    BLACK_C = 1
} ColorId;

typedef struct{
    char32_t character;
    uint8_t backgroundColor;
    uint8_t foregroundColor;
} Character;

typedef struct{
    vec3i position;
    vec2i dimensions;
    Character* buffer;
} Sprite; 


#define BUFFERS_NUMBER 2
typedef struct {
    Sprite buffers[BUFFERS_NUMBER];
    Sprite* current;
} Buffer;

void Sprite_AllocateBuffer(Sprite* sprite){
    sprite->buffer = calloc(sprite->dimensions.width * sprite->dimensions.height, sizeof(Character));
}

#define KEY_COUNT 255

enum Keys{
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_4,
    MOUSE_5,
    MOUSE_6,
    MOUSE_7,
    MOUSE_8,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_SENTINEL = KEY_COUNT
};

typedef struct Input{
    vec2i mousePosition;
    vec2i mouseDelta;
    int mouseWheel;
    bool keys[KEY_COUNT];
    bool keysPrev[KEY_COUNT];
} Input;

static inline bool InputKeyPressed(const Input* input, Keys key){
    return input->keys[key] && !input->keysPrev[key];
}

static inline bool InputKeyReleased(const Input* input, Keys key){
    return !input->keys[key] && input->keysPrev[key];
}

static inline bool InputKeyDown(const Input* input, Keys key){
    return input->keys[key];
}

typedef struct {
    vec2i screenSize;
    HANDLE In;
    HANDLE Out;
} Window;

typedef struct {
    Color colorPalette[256];
    Input input;
    Buffer buffer;
    Window window;
} ConsoleLabContext;

ConsoleLabContext ConsoleLab_GlobalContext;

vec2i GetScreenSize_Ctx(Window* window) {
    RECT rect;
    GetWindowRect(GetConsoleWindow(), &rect);
    return (vec2i){rect.right - rect.left, rect.bottom - rect.top};
}

vec2i GetScreenSize(){//in pixels
    GetScreenSize_Ctx(&ConsoleLab_GlobalContext.window);
}

vec2i GetFontSize_Ctx(Window* window) {
    CONSOLE_FONT_INFOEX fontInfo = {0};
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(window->Out, FALSE, &fontInfo);
    return (vec2i){fontInfo.dwFontSize.X, fontInfo.dwFontSize.Y};
}

vec2i GetFontSize(){//in pixels
    GetFontSize_Ctx(&ConsoleLab_GlobalContext.window);
}

vec2i GetConsoleScreenSize_Ctx(Window* window) {
    vec2i screen = GetScreenSize_Ctx(window);
    vec2i font = GetFontSize_Ctx(window);
    return (vec2i){screen.width / font.width, screen.height / font.height};
}

vec2i GetConsoleScreenSize(){//in console cells
    GetConsoleScreenSize_Ctx(&ConsoleLab_GlobalContext.window);
}

void ResizeBuffer_Ctx(ConsoleLabContext* ctx) {
    vec2i size = GetConsoleScreenSize_Ctx(&ctx->window);
    COORD coord = {(SHORT)size.width, (SHORT)size.height};
    SetConsoleScreenBufferSize(ctx->window.Out, coord);

    SMALL_RECT rect = {0, 0, (SHORT)(size.width - 1), (SHORT)(size.height - 1)};
    SetConsoleWindowInfo(ctx->window.Out, TRUE, &rect);
}

void ResizeBuffer(){//in pixels
    ResizeBuffer_Ctx(&ConsoleLab_GlobalContext);
}

void ConsoleLab_Init_Ctx(ConsoleLabContext* ctx) {
    // handles
    ctx->window.In  = GetStdHandle(STD_INPUT_HANDLE);
    ctx->window.Out = GetStdHandle(STD_OUTPUT_HANDLE);

    // paleta
    Color defaultPalette[256] = {
        {255,255,255,255},
        {0,0,0,0}
    };
    memcpy(ctx->colorPalette, defaultPalette, sizeof(ctx->colorPalette));

    // input
    memset(&ctx->input, 0, sizeof(ctx->input));

    // resize konsoli do rozmiaru okna
    ResizeBuffer_Ctx(ctx);

    // bufory
    vec2i size = GetConsoleScreenSize_Ctx(&ctx->window);
    for (int i = 0; i < BUFFERS_NUMBER; i++) {
        ctx->buffer.buffers[i].position   = (vec3i){0, 0, 0};
        ctx->buffer.buffers[i].dimensions = size;
        Sprite_AllocateBuffer(&ctx->buffer.buffers[i]);
    }
    ctx->buffer.current = &ctx->buffer.buffers[0];
}

void ConsoleLab_Init() {
    ConsoleLab_Init_Ctx(&ConsoleLab_GlobalContext);
}

void ConsoleLab_Update_Ctx(ConsoleLabContext* ctx) {
    vec2i newSize = GetConsoleScreenSize_Ctx(&ctx->window);
    vec2i oldSize = ctx->buffer.buffers[0].dimensions;

    if (newSize.width != oldSize.width || newSize.height != oldSize.height) {
        ResizeBuffer_Ctx(ctx);
        for (int i = 0; i < BUFFERS_NUMBER; i++) {
            free(ctx->buffer.buffers[i].buffer);
            ctx->buffer.buffers[i].dimensions = newSize;
            Sprite_AllocateBuffer(&ctx->buffer.buffers[i]);
        }
        ctx->buffer.current = &ctx->buffer.buffers[0];
    }

    
}

void ConsoleLab_Update(){
    ConsoleLab_Update_Ctx(&ConsoleLab_GlobalContext);
}

void ConsoleLabDrawBuffer_Ctx(Sprite* buffer){

}

void ConsoleLabFlipBuffer_Ctx(ConsoleLabContext* ctx) {
    ConsoleLabDrawBuffer_Ctx(ctx->buffer.current);
    // przełącz na drugi bufor
    ctx->buffer.current = (ctx->buffer.current == &ctx->buffer.buffers[0])
        ? &ctx->buffer.buffers[1]
        : &ctx->buffer.buffers[0];
}

#ifdef __cplusplus
}
#endif
#ifdef CONSOLELAB_INCLUDE_IMPLEMENTATION
// if I decide to unsingle header it later
#endif
#endif