#include "ConsoleLab.h"
#define CL_IMPL_VERSION_MAJOR 0
#define CL_IMPL_VERSION_MINOR 0
#if CL_VERSION_MAJOR != CL_IMPL_VERSION_MAJOR || CL_VERSION_MINOR != CL_IMPL_VERSION_MINOR
#error "ConsoleLab version mismatch between header and implementation"
#endif


#ifdef _WIN32
#include "ConsoleLab_win.c"
#else
#include "ConsoleLab_unix.c"
#endif



// void clInit(clContext* context, clResult* result){
    
// }

void clCharInit(clChar* clCharacter,char* character, RGBA* backgroundColor, RGBA* foregroundColor){
    clCharacter->character = character;
    clCharacter->backgroundColor = backgroundColor;
    clCharacter->foregroundColor = foregroundColor;
}

int clSpriteInit(clSprite* sprite) {
    if (!sprite) return 1;

    if (sprite->dimensions.x == 0 || sprite->dimensions.y == 0) {
        sprite->buffer = NULL;
        return 2;
    }

    size_t size = sprite->dimensions.x * sprite->dimensions.y;
    if (size / sprite->dimensions.x != sprite->dimensions.y) {
        sprite->buffer = NULL;
        return 3;
    }

    sprite->buffer = malloc(size * sizeof(*sprite->buffer));
    if (!sprite->buffer) {
        return 4;
    }
    return 0;
}

void clSpriteFree(clSprite* sprite){
    if (!sprite) return;
    free(sprite->buffer);
    free(sprite);
}

int clSpriteCreate(clSprite* sprite, float x, float y, float width, float height){
    sprite->position = Vec2(x, y);
    sprite->dimensions = Vec2(width, height);
    if(!clSpriteInit(sprite)){
        //error handling
        return 1;
    }
    return 0;
}

int clSpriteCreateVec(clSprite* sprite, vec2 position, vec2 dimensions){
    sprite->position = position;
    sprite->dimensions = dimensions;
    if(!clSpriteInit(sprite)){
        //error handling
        return 1;
    }
    return 0;
}
// void GetSpriteFromCharSpriteMap(clSprite* destSprite,char* spriteMap,int x, int y, int h, int w, int mapH, int mapW){
//     if(x < 0 || x+w > mapW){
//         return;
//     }
//     if(y < 0 || y+h>mapH){
//         return;
//     }

//     const int destWidth = mapW;
//     const int startIndex = x+(y*destWidth);

//     int index = 0;
//     for(int yp = y; yp < y+h;yp++){
//         for(int xp = x; xp < x+w;xp++){
//             destSprite->buffer[index] = (clChar){.character = spriteMap[(yp*destWidth)+xp],&colorTable[Clear_c],&colorTable[Clear_c]};
//             index++;
//         }
//     }
// }

// void WriteToSprite(clSprite* destSprite,clSprite* srcSprite){
//     if(srcSprite->position.x < 0 || srcSprite->position.x+srcSprite->dimensions.w>destSprite->dimensions.w){
//         return;
//     }
//     if(srcSprite->position.y < 0 || srcSprite->position.y+srcSprite->dimensions.h>destSprite->dimensions.h){
//         return;
//     }

//     const int destWidth = destSprite->dimensions.w;
//     const int startIndex = srcSprite->position.x+(srcSprite->position.y*destWidth);

//     int index = 0;
//     for(int y = srcSprite->position.y; y < srcSprite->position.y+srcSprite->dimensions.h;y++){
//         for(int x = srcSprite->position.x; y < srcSprite->position.x+srcSprite->dimensions.w;x++){
//             destSprite->buffer[(y*destWidth)+x] = srcSprite->buffer[index];
//             index++;
//         }
//     }
// }

void RenderSpriteString(clSprite* sprite,char* output,int length){
    int index = 0;
    char* outputEnd = output;
    while(index < length){

        clChar* character = &(sprite->buffer[index]);
        char buffer[300] = "";
        char* bufferEnd = buffer;

        char background[200] = "";
        if(character->backgroundColor->a == colorTable[Clear_c].a){
            GetConsoleTextColorDefaultBackgroundString(background);
        }
        else{
            GetConsoleTextBackgroundColorString(background,character->backgroundColor);
        }

        char foreground[200] = "";
        if(character->foregroundColor->a == colorTable[Clear_c].a){
            GetConsoleTextColorDefaultForegroundString(foreground);
        }
        else{
            GetConsoleTextForegroundColorString(foreground,character->foregroundColor);
        }

        bufferEnd = strcatf(bufferEnd,background);
        bufferEnd = strcatf(bufferEnd,foreground);
        char* characterValue = character->character;
        bufferEnd = strcatf(bufferEnd,characterValue);

        outputEnd = strcatf(outputEnd,buffer);
        index += 1;
    }
}
//TODO funkcja do printowania sprita na inny sprite, i do printowania sprita do konsoli i najpierw printowac na prite BUFFER i z buffera na kosole








#define NUMBER_OF_KEYS 128

enum CL_KEYS {
    // Mouse
    KEY_MOUSE_LEFT,
    KEY_MOUSE_RIGHT,
    KEY_MOUSE_MIDDLE,
    KEY_MOUSE_X1,
    KEY_MOUSE_X2,

    // Letters
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
    KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
    KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
    KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

    // Numbers (top row)
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
    KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

    // Numpad
    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,
    KEY_NUMPAD_MULTIPLY,
    KEY_NUMPAD_ADD,
    KEY_NUMPAD_SUBTRACT,
    KEY_NUMPAD_DECIMAL,
    KEY_NUMPAD_DIVIDE,
    KEY_NUMPAD_ENTER,

    // Function keys
    KEY_F1,  KEY_F2,  KEY_F3,  KEY_F4,
    KEY_F5,  KEY_F6,  KEY_F7,  KEY_F8,
    KEY_F9,  KEY_F10, KEY_F11, KEY_F12,

    // Control keys
    KEY_ESC,
    KEY_TAB,
    KEY_CAPS_LOCK,
    KEY_SHIFT,
    KEY_CTRL,
    KEY_ALT,
    KEY_SPACE,
    KEY_ENTER,
    KEY_BACKSPACE,

    // Arrows
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,

    // Navigation
    KEY_INSERT,
    KEY_DELETE,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,

    // Symbols
    KEY_MINUS,
    KEY_EQUAL,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_GRAVE, // `

    // Locks
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,

    // System
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_LALT,
    KEY_RALT,
    KEY_LSUPER, // Windows / Cmd
    KEY_RSUPER,

    // Other
    KEY_MENU,

    KEY_MAX // sentinel
};

typedef struct clInput{
    vec2 mousePos;//vec2 is int
    bool keys[NUMBER_OF_KEYS];//every tick in handle events set to true only if it was clicked in this frame
    bool keysHeld[NUMBER_OF_KEYS];
    //handle mouse scrool too
} clInput;

typedef struct clBuffer{
    clSprite buffers[2];

} clBuffer;



clWindow* ConsoleLabGetWindow(){
    clWindow* window;
    return window;//undfined behavior
}

clBuffer* ConsoleLabCreateBuffer(){
    clBuffer* buffer;
    return buffer;//undfined behavior
}

clInput* ConsoleLabCreateInputHandle(){
    clInput* input;
    return input;//undfined behavior
}

void clCharSetBuffer(clChar* clchar, char* buffer ,size8_t unicodeLen){
    memcpy(clchar->character, buffer, unicodeLen);
}

void clSequenceUnicodeString(clChar* buffer, size_t size, char* unicodeBuffer){
    if (!buffer || !unicodeBuffer || size == 0) {
        return;
    }
    int counter = 0;
    while((*unicodeBuffer)){
        size8_t unicodeLen = getCharLength(unicodeBuffer);
        if(unicodeLen == 1){
            unicodeBuffer++;
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        clCharSetBuffer(&buffer[counter], unicodeBuffer, unicodeLen);
        unicodeBuffer+=unicodeLen;
        counter++;
    }
    return;
}

clSprite* ConsoleLabCreateSprite(char* input, vec2 position, vec2 dimensions){
    clSprite* sprite = malloc(sizeof(clSprite));
    if(!sprite){
        return NULL;
    }
    sprite->position = position;
    sprite->dimensions = dimensions;

    if(sprite->dimensions.x == 0 || sprite->dimensions.y == 0) {
        sprite->buffer = NULL;
        return sprite;
    }

    size_t size = sprite->dimensions.x * sprite->dimensions.y;
    sprite->buffer = calloc(size, sizeof(*sprite->buffer));
    
    if (!sprite->buffer) {
        free(sprite);
        return NULL;
    }

    if (input) {
        clSequenceUnicodeString(sprite->buffer, size, input);
    }

    return sprite;
}

void ConsoleLabHandleEvents(clWindow* window, clInput* input, clBuffer* buffer){
    //handle resizing windows console buffer and clBuffer if window resized

}

void ConsoleLabTick(clWindow* window, clInput* input, clBuffer* buffer){
    ConsoleLabHandleEvents(window, input, buffer);
}