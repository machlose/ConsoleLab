#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#define CONSOLELAB_VERSION_MAJOR 0
#define CONSOLELAB_VERSION_MINOR 0

#ifdef _WIN32
#include <windows.h>
#else
// other platforms in the future
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    union {
        struct { uint8_t r, g, b, a; };
        uint8_t arr[4];
    };
} Color;

// Mapowanie RGB na najbliższy z 16 kolorów WinAPI
static inline WORD Color_ToWinAPI(uint8_t r, uint8_t g, uint8_t b) {
    static const uint8_t winColors[16][3] = {
        {0,0,0},{0,0,128},{0,128,0},{0,128,128},
        {128,0,0},{128,0,128},{128,128,0},{192,192,192},
        {128,128,128},{0,0,255},{0,255,0},{0,255,255},
        {255,0,0},{255,0,255},{255,255,0},{255,255,255}
    };
    int best = 0, bestDist = 999999;
    for (int i = 0; i < 16; i++) {
        int dr = (int)r - winColors[i][0];
        int dg = (int)g - winColors[i][1];
        int db = (int)b - winColors[i][2];
        int dist = dr*dr + dg*dg + db*db;
        if (dist < bestDist) { bestDist = dist; best = i; }
    }
    return (WORD)best;
}

///////////////////////////////////////////////////////////////////////////////////////

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
    char32_t character;
    uint8_t  backgroundColor; // indeks do colorPalette
    uint8_t  foregroundColor; // indeks do colorPalette
} Character;

typedef struct {
    vec3i     position;
    vec2i     dimensions;
    Character* buffer;
} Sprite;

static inline void Sprite_AllocateBuffer(Sprite* sprite) {
    sprite->buffer = (Character*)calloc(
        sprite->dimensions.width * sprite->dimensions.height,
        sizeof(Character)
    );
}

static inline Character* Sprite_At(Sprite* sprite, int x, int y) {
    return &sprite->buffer[y * sprite->dimensions.width + x];
}

#define BUFFERS_NUMBER 2
typedef struct {
    Sprite  buffers[BUFFERS_NUMBER];
    Sprite* current;
} Buffer;

///////////////////////////////////////////////////////////////////////////////////////

// Wartości enum = VK_ kody Windows
#define KEY_COUNT 256

typedef enum {
    MOUSE_LEFT   = 0x01, // VK_LBUTTON
    MOUSE_RIGHT  = 0x02, // VK_RBUTTON
    MOUSE_MIDDLE = 0x04, // VK_MBUTTON
    MOUSE_4      = 0x05, // VK_XBUTTON1
    MOUSE_5      = 0x06, // VK_XBUTTON2

    KEY_BACKSPACE = 0x08,
    KEY_TAB       = 0x09,
    KEY_ENTER     = 0x0D,
    KEY_SHIFT     = 0x10,
    KEY_CTRL      = 0x11,
    KEY_ALT       = 0x12,
    KEY_ESCAPE    = 0x1B,
    KEY_SPACE     = 0x20,
    KEY_LEFT      = 0x25,
    KEY_UP        = 0x26,
    KEY_RIGHT     = 0x27,
    KEY_DOWN      = 0x28,

    KEY_0 = 0x30, KEY_1, KEY_2, KEY_3, KEY_4,
    KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

    KEY_A = 0x41, KEY_B, KEY_C, KEY_D, KEY_E,
    KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
    KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
    KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
    KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

    KEY_F1  = 0x70, KEY_F2,  KEY_F3,  KEY_F4,
    KEY_F5,  KEY_F6,  KEY_F7,  KEY_F8,
    KEY_F9,  KEY_F10, KEY_F11, KEY_F12,
} Keys;

typedef struct {
    vec2i mousePosition;
    vec2i mouseDelta;
    int   mouseWheel;
    bool  keys[KEY_COUNT];
    bool  keysPrev[KEY_COUNT];
} Input;

static inline bool InputKeyPressed (const Input* input, Keys key) { return  input->keys[key] && !input->keysPrev[key]; }
static inline bool InputKeyReleased(const Input* input, Keys key) { return !input->keys[key] &&  input->keysPrev[key]; }
static inline bool InputKeyDown    (const Input* input, Keys key) { return  input->keys[key]; }

///////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    vec2i  screenSize;
    HANDLE In;
    HANDLE Out;
    bool   ansiSupported;
} Window;

typedef struct {
    Color  colorPalette[256];
    Input  input;
    Buffer buffer;
    Window window;
} ConsoleLabContext;

ConsoleLabContext ConsoleLab_GlobalContext;


static inline vec2i GetScreenSize_Ctx(Window* window) {
    RECT rect;
    GetWindowRect(GetConsoleWindow(), &rect);
    vec2i r;
    r.width  = rect.right  - rect.left;
    r.height = rect.bottom - rect.top;
    return r;
}
static inline vec2i GetScreenSize(void) { return GetScreenSize_Ctx(&ConsoleLab_GlobalContext.window); }

static inline vec2i GetFontSize_Ctx(Window* window) {
    CONSOLE_FONT_INFOEX fi;
    fi.cbSize = sizeof(fi);
    GetCurrentConsoleFontEx(window->Out, FALSE, &fi);
    vec2i r;
    r.width  = fi.dwFontSize.X ? fi.dwFontSize.X : 8;
    r.height = fi.dwFontSize.Y ? fi.dwFontSize.Y : 16;
    return r;
}
static inline vec2i GetFontSize(void) { return GetFontSize_Ctx(&ConsoleLab_GlobalContext.window); }

static inline vec2i GetConsoleScreenSize_Ctx(Window* window) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(window->Out, &csbi);
    vec2i r;
    r.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    r.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    return r;
}
static inline vec2i GetConsoleScreenSize(void) { return GetConsoleScreenSize_Ctx(&ConsoleLab_GlobalContext.window); }

static inline void ResizeConsoleBuffer_Ctx(ConsoleLabContext* ctx) {
    vec2i size = GetConsoleScreenSize_Ctx(&ctx->window);
    if (size.width  < 1) size.width  = 1;
    if (size.height < 1) size.height = 1;

    // Zmniejsz okno do minimum zeby uniknac bledu gdy nowy bufor < okno
    SMALL_RECT tiny = {0, 0, 0, 0};
    SetConsoleWindowInfo(ctx->window.Out, TRUE, &tiny);

    COORD coord;
    coord.X = (SHORT)size.width;
    coord.Y = (SHORT)size.height;
    SetConsoleScreenBufferSize(ctx->window.Out, coord);

    SMALL_RECT rect;
    rect.Left   = 0;
    rect.Top    = 0;
    rect.Right  = (SHORT)(size.width  - 1);
    rect.Bottom = (SHORT)(size.height - 1);
    SetConsoleWindowInfo(ctx->window.Out, TRUE, &rect);
}
static inline void ResizeConsoleBuffer(void) { ResizeConsoleBuffer_Ctx(&ConsoleLab_GlobalContext); }

static inline void Input_Update_Ctx(ConsoleLabContext* ctx) {
    Input* input = &ctx->input;

    memcpy(input->keysPrev, input->keys, sizeof(input->keys));
    vec2i prevMouse = input->mousePosition;
    input->mouseWheel = 0;

    for (int vk = 0; vk < KEY_COUNT; vk++) {
        input->keys[vk] = (GetAsyncKeyState(vk) & 0x8000) != 0;
    }

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(GetConsoleWindow(), &pt);
    vec2i font = GetFontSize_Ctx(&ctx->window);
    input->mousePosition.x = pt.x / font.width;
    input->mousePosition.y = pt.y / font.height;
    input->mouseDelta.x = input->mousePosition.x - prevMouse.x;
    input->mouseDelta.y = input->mousePosition.y - prevMouse.y;

    DWORD numEvents = 0;
    GetNumberOfConsoleInputEvents(ctx->window.In, &numEvents);
    if (numEvents > 0) {
        INPUT_RECORD records[64];
        DWORD read = 0;
        ReadConsoleInput(ctx->window.In, records, 64, &read);
        for (DWORD i = 0; i < read; i++) {
            if (records[i].EventType == MOUSE_EVENT) {
                MOUSE_EVENT_RECORD* me = &records[i].Event.MouseEvent;
                if (me->dwEventFlags == MOUSE_WHEELED) {
                    input->mouseWheel = ((short)HIWORD(me->dwButtonState)) > 0 ? 1 : -1;
                }
            }
        }
    }
}
static inline void Input_Update(void) { Input_Update_Ctx(&ConsoleLab_GlobalContext); }

///////////////////////////////////////////////////////////////////////////////////////

#define CL_DRAW_BUF (1024 * 128)

static inline void ConsoleLabDrawBuffer_Ctx(ConsoleLabContext* ctx, Sprite* sprite) {
    int w = sprite->dimensions.width;
    int h = sprite->dimensions.height;

    if (ctx->window.ansiSupported) {
        char* buf = (char*)malloc(CL_DRAW_BUF);
        if (!buf) return;
        int pos = 0;

        pos += sprintf(buf + pos, "\x1b[H\x1b[?25l");

        uint8_t lastBg = 255, lastFg = 255;

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Character* ch = Sprite_At(sprite, x, y);
                uint8_t bgIdx = ch->backgroundColor;
                uint8_t fgIdx = ch->foregroundColor;

                // Kolory tylko gdy sie zmienily
                if (bgIdx != lastBg || fgIdx != lastFg) {
                    Color bg = ctx->colorPalette[bgIdx];
                    Color fg = ctx->colorPalette[fgIdx];
                    pos += sprintf(buf + pos,
                        "\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm",
                        fg.r, fg.g, fg.b, bg.r, bg.g, bg.b);
                    lastBg = bgIdx; lastFg = fgIdx;
                }

                // Znak jako UTF-8
                char32_t c = ch->character ? ch->character : U' ';
                if (pos < CL_DRAW_BUF - 128) {
                    if (c < 0x80) {
                        buf[pos++] = (char)c;
                    } else if (c < 0x800) {
                        buf[pos++] = (char)(0xC0 | (c >> 6));
                        buf[pos++] = (char)(0x80 | (c & 0x3F));
                    } else if (c < 0x10000) {
                        buf[pos++] = (char)(0xE0 | (c >> 12));
                        buf[pos++] = (char)(0x80 | ((c >> 6) & 0x3F));
                        buf[pos++] = (char)(0x80 | (c & 0x3F));
                    } else {
                        buf[pos++] = (char)(0xF0 | (c >> 18));
                        buf[pos++] = (char)(0x80 | ((c >> 12) & 0x3F));
                        buf[pos++] = (char)(0x80 | ((c >> 6) & 0x3F));
                        buf[pos++] = (char)(0x80 | (c & 0x3F));
                    }
                }

                // Flush jesli bufor prawie pelny
                if (pos > CL_DRAW_BUF - 256) {
                    DWORD written;
                    WriteConsoleA(ctx->window.Out, buf, (DWORD)pos, &written, NULL);
                    pos = 0;
                    lastBg = lastFg = 255;
                }
            }
            if (y < h - 1 && pos < CL_DRAW_BUF - 4)
                buf[pos++] = '\n';
        }

        pos += sprintf(buf + pos, "\x1b[0m");
        DWORD written;
        WriteConsoleA(ctx->window.Out, buf, (DWORD)pos, &written, NULL);
        free(buf);

    } else {
        // 16 kolorow
        CHAR_INFO* cells = (CHAR_INFO*)malloc(w * h * sizeof(CHAR_INFO));
        if (!cells) return;

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Character* ch = Sprite_At(sprite, x, y);
                Color bg = ctx->colorPalette[ch->backgroundColor];
                Color fg = ctx->colorPalette[ch->foregroundColor];
                WORD bgA = Color_ToWinAPI(bg.r, bg.g, bg.b);
                WORD fgA = Color_ToWinAPI(fg.r, fg.g, fg.b);
                cells[y * w + x].Char.UnicodeChar = (WCHAR)(ch->character ? ch->character : L' ');
                cells[y * w + x].Attributes       = (WORD)((bgA << 4) | fgA);
            }
        }

        COORD bufSize;  bufSize.X = (SHORT)w; bufSize.Y = (SHORT)h;
        COORD bufCoord; bufCoord.X = 0; bufCoord.Y = 0;
        SMALL_RECT wr;  wr.Left = 0; wr.Top = 0; wr.Right = (SHORT)(w-1); wr.Bottom = (SHORT)(h-1);
        WriteConsoleOutputW(ctx->window.Out, cells, bufSize, bufCoord, &wr);
        free(cells);
    }
}

static inline void ConsoleLabDrawBuffer(void) {
    ConsoleLabDrawBuffer_Ctx(&ConsoleLab_GlobalContext, ConsoleLab_GlobalContext.buffer.current);
}

static inline void ConsoleLabFlipBuffer_Ctx(ConsoleLabContext* ctx) {
    ConsoleLabDrawBuffer_Ctx(ctx, ctx->buffer.current);
    ctx->buffer.current = (ctx->buffer.current == &ctx->buffer.buffers[0]) ? &ctx->buffer.buffers[1] : &ctx->buffer.buffers[0];
}
static inline void ConsoleLabFlipBuffer(void) { ConsoleLabFlipBuffer_Ctx(&ConsoleLab_GlobalContext); }

static inline void ClearBuffer_Ctx(ConsoleLabContext* ctx, uint8_t bg) {
    Sprite* s = ctx->buffer.current;
    int total = s->dimensions.width * s->dimensions.height;
    for (int i = 0; i < total; i++) {
        s->buffer[i].character       = U' ';
        s->buffer[i].foregroundColor = 0;
        s->buffer[i].backgroundColor = bg;
    }
}
static inline void ClearBuffer(uint8_t bg) { ClearBuffer_Ctx(&ConsoleLab_GlobalContext, bg); }

static inline void DrawChar_Ctx(ConsoleLabContext* ctx, int x, int y, char32_t ch, uint8_t fg, uint8_t bg) {
    Sprite* s = ctx->buffer.current;
    if (x < 0 || y < 0 || x >= s->dimensions.width || y >= s->dimensions.height) return;
    Character* c = Sprite_At(s, x, y);
    c->character       = ch;
    c->foregroundColor = fg;
    c->backgroundColor = bg;
}
static inline void DrawChar(int x, int y, char32_t ch, uint8_t fg, uint8_t bg) {
    DrawChar_Ctx(&ConsoleLab_GlobalContext, x, y, ch, fg, bg);
}

static inline void DrawText_Ctx(ConsoleLabContext* ctx, int x, int y, const char* text, uint8_t fg, uint8_t bg) {
    for (int i = 0; text[i]; i++)
        DrawChar_Ctx(ctx, x + i, y, (char32_t)(unsigned char)text[i], fg, bg);
}
static inline void CL_DrawText(int x, int y, const char* text, uint8_t fg, uint8_t bg) {
    DrawText_Ctx(&ConsoleLab_GlobalContext, x, y, text, fg, bg);
}


static inline void Sprite_DrawWString(Sprite* sprite, int x, int y, const char32_t* str, uint8_t fg, uint8_t bg) {
    int cx = x;
    for (int i = 0; str[i] != U'\0'; i++, cx++) {
        if (cx < 0 || cx >= sprite->dimensions.width) continue;
        if (y  < 0 || y  >= sprite->dimensions.height) return;
        Character* c = Sprite_At(sprite, cx, y);
        c->character       = str[i];
        c->foregroundColor = fg;
        c->backgroundColor = bg;
    }
}

static inline void Sprite_DrawWStringWrapped(Sprite* sprite, int startX, int startY, const char32_t* str, uint8_t fg, uint8_t bg) {
    int x = startX;
    int y = startY;
    for (int i = 0; str[i] != U'\0'; i++) {
        if (x >= sprite->dimensions.width) {
            x = startX;
            y++;
        }
        if (y >= sprite->dimensions.height) break;
        if (x >= 0 && y >= 0) {
            Character* c = Sprite_At(sprite, x, y);
            c->character       = str[i];
            c->foregroundColor = fg;
            c->backgroundColor = bg;
        }
        x++;
    }
}

// Znaki z characterem == 0 sa traktowane jako przezroczyste
static inline void DrawSprite(Sprite* dst, Sprite* src) {
    int offX = src->position.x;
    int offY = src->position.y;
    for (int y = 0; y < src->dimensions.height; y++) {
        for (int x = 0; x < src->dimensions.width; x++) {
            int dstX = x + offX;
            int dstY = y + offY;
            if (dstX < 0 || dstY < 0 || dstX >= dst->dimensions.width || dstY >= dst->dimensions.height)
                continue;
            Character* sc = Sprite_At(src, x, y);
            Character* dc = Sprite_At(dst, dstX, dstY);
            if (sc->character == 0 || sc->character == U' ') continue;

            dc->character       = sc->character;
            dc->foregroundColor = sc->foregroundColor;
            // tlo kopiuj tylko jesli nie przezroczyste
            if (sc->backgroundColor != 255)
                dc->backgroundColor = sc->backgroundColor;
        }
    }
}
 
// Wersja bez przezroczystosci - kopiuje wszystko lacznie z zerowymi znakami
static inline void DrawSpriteOpaque(Sprite* dst, Sprite* src) {
    int offX = src->position.x;
    int offY = src->position.y;
    for (int y = 0; y < src->dimensions.height; y++) {
        for (int x = 0; x < src->dimensions.width; x++) {
            int dstX = x + offX;
            int dstY = y + offY;
            if (dstX < 0 || dstY < 0 || dstX >= dst->dimensions.width || dstY >= dst->dimensions.height)
                continue;
            *Sprite_At(dst, dstX, dstY) = *Sprite_At(src, x, y);
        }
    }
}
 
static inline void DrawToBuffer(Sprite* src) {
    DrawSprite(ConsoleLab_GlobalContext.buffer.current, src);
}
static inline void DrawToBufferOpaque(Sprite* src) {
    DrawSpriteOpaque(ConsoleLab_GlobalContext.buffer.current, src);
}
 

static inline void ConsoleLab_Init_Ctx(ConsoleLabContext* ctx) {
    ctx->window.In  = GetStdHandle(STD_INPUT_HANDLE);
    ctx->window.Out = GetStdHandle(STD_OUTPUT_HANDLE);
 

    DWORD mode = 0;
    GetConsoleMode(ctx->window.Out, &mode);
    if (SetConsoleMode(ctx->window.Out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        ctx->window.ansiSupported = true;
        SetConsoleOutputCP(CP_UTF8);
    } else {
        ctx->window.ansiSupported = false;
    }
 

    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 1; ci.bVisible = FALSE;
    SetConsoleCursorInfo(ctx->window.Out, &ci);
 

    DWORD inMode = 0;
    GetConsoleMode(ctx->window.In, &inMode);
    inMode &= ~(ENABLE_ECHO_INPUT | ENABLE_QUICK_EDIT_MODE | ENABLE_LINE_INPUT);
    inMode |= ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
    SetConsoleMode(ctx->window.In, inMode);
 
    Color defaultPalette[256];
    memset(defaultPalette, 0, sizeof(defaultPalette));
    defaultPalette[0] = (Color){{255,255,255,255}}; // white
    defaultPalette[1] = (Color){{0,  0,  0,  255}}; // black
    defaultPalette[2] = (Color){{255,0,  0,  255}}; // czerwony
    defaultPalette[3] = (Color){{0,  255,0,  255}}; // zielony
    defaultPalette[4] = (Color){{0,  0,  255,255}}; // niebieski
    defaultPalette[5] = (Color){{255,255,0,  255}}; // zolty
    defaultPalette[6] = (Color){{0,  255,255,255}}; // cyan
    defaultPalette[7] = (Color){{255,0,  255,255}}; // magenta
    defaultPalette[8] = (Color){{128,128,128,255}}; // szary
    defaultPalette[255] = (Color){{0,0,0,0}}; // transparent
    memcpy(ctx->colorPalette, defaultPalette, sizeof(ctx->colorPalette));
 
    memset(&ctx->input, 0, sizeof(ctx->input));
 
    ResizeConsoleBuffer_Ctx(ctx);
 
    vec2i size = GetConsoleScreenSize_Ctx(&ctx->window);
    for (int i = 0; i < BUFFERS_NUMBER; i++) {
        ctx->buffer.buffers[i].position.x = 0;
        ctx->buffer.buffers[i].position.y = 0;
        ctx->buffer.buffers[i].position.z = 0;
        ctx->buffer.buffers[i].dimensions = size;
        Sprite_AllocateBuffer(&ctx->buffer.buffers[i]);
    }
    ctx->buffer.current = &ctx->buffer.buffers[0];
}
static inline void ConsoleLab_Init(void) { ConsoleLab_Init_Ctx(&ConsoleLab_GlobalContext); }
 
static inline void ConsoleLab_Update_Ctx(ConsoleLabContext* ctx) {
    Input_Update_Ctx(ctx);
 
    vec2i newSize = GetConsoleScreenSize_Ctx(&ctx->window);
    vec2i oldSize = ctx->buffer.buffers[0].dimensions;
    if (newSize.width != oldSize.width || newSize.height != oldSize.height) {
        COORD coord;
        coord.X = (SHORT)newSize.width;
        coord.Y = (SHORT)newSize.height;
        SetConsoleScreenBufferSize(ctx->window.Out, coord);
 
        for (int i = 0; i < BUFFERS_NUMBER; i++) {
            free(ctx->buffer.buffers[i].buffer);
            ctx->buffer.buffers[i].dimensions = newSize;
            Sprite_AllocateBuffer(&ctx->buffer.buffers[i]);
        }
        ctx->buffer.current = &ctx->buffer.buffers[0];
    }
}
static inline void ConsoleLab_Update(void) { ConsoleLab_Update_Ctx(&ConsoleLab_GlobalContext); }

#ifdef __cplusplus
}
#endif
#ifdef CONSOLELAB_INCLUDE_IMPLEMENTATION

#endif
#endif