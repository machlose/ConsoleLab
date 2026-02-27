// console_gl.c
// Kompilacja: gcc console_gl.c -o console_gl.exe -lopengl32 -lgdi32 -luser32
// Lub MSVC:   cl console_gl.c opengl32.lib gdi32.lib user32.lib

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Ładujemy funkcje OpenGL 3.3 ręcznie przez wglGetProcAddress
typedef char GLchar;
typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef float GLfloat;
typedef signed long long GLsizeiptr;
typedef unsigned char GLboolean;
typedef int GLsizei;

#define GL_FRAGMENT_SHADER        0x8B30
#define GL_VERTEX_SHADER          0x8B31
#define GL_COMPILE_STATUS         0x8B81
#define GL_LINK_STATUS            0x8B82
#define GL_ARRAY_BUFFER           0x8892
#define GL_STATIC_DRAW            0x88B4
#define GL_TEXTURE_2D             0x0DE1
#define GL_TEXTURE0               0x84C0
#define GL_RGBA                   0x1908
#define GL_UNSIGNED_BYTE          0x1401
#define GL_FLOAT                  0x1406
#define GL_TRIANGLES              0x0004
#define GL_TRIANGLE_STRIP         0x0005
#define GL_COLOR_BUFFER_BIT       0x00004000
#define GL_TEXTURE_MIN_FILTER     0x2801
#define GL_TEXTURE_MAG_FILTER     0x2800
#define GL_NEAREST                0x2600
#define GL_LINEAR                 0x2601
#define GL_TEXTURE_WRAP_S         0x2802
#define GL_TEXTURE_WRAP_T         0x2803
#define GL_CLAMP_TO_EDGE          0x812F
#define GL_TRUE                   1
#define GL_FALSE                  0

// Deklaracje funkcji OpenGL 3.3
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum);
typedef void   (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint, GLsizei, const GLchar**, const GLint*);
typedef void   (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint);
typedef void   (APIENTRY *PFNGLGETSHADERIVPROC)(GLuint, GLenum, GLint*);
typedef void   (APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void   (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint, GLuint);
typedef void   (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint);
typedef void   (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint);
typedef void   (APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei, GLuint*);
typedef void   (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void   (APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei, GLuint*);
typedef void   (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum, GLuint);
typedef void   (APIENTRY *PFNGLBUFFERDATAPROC)(GLenum, GLsizeiptr, const void*, GLenum);
typedef void   (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void   (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef GLint  (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint, const GLchar*);
typedef void   (APIENTRY *PFNGLUNIFORM1IPROC)(GLint, GLint);
typedef void   (APIENTRY *PFNGLUNIFORM1FPROC)(GLint, GLfloat);
typedef void   (APIENTRY *PFNGLUNIFORM2FPROC)(GLint, GLfloat, GLfloat);
typedef void   (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum);
typedef BOOL   (APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int);

// Instancje funkcji
PFNGLCREATESHADERPROC           glCreateShader;
PFNGLSHADERSOURCEPROC           glShaderSource;
PFNGLCOMPILESHADERPROC          glCompileShader;
PFNGLGETSHADERIVPROC            glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC       glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC          glCreateProgram;
PFNGLATTACHSHADERPROC           glAttachShader;
PFNGLLINKPROGRAMPROC            glLinkProgram;
PFNGLUSEPROGRAMPROC             glUseProgram;
PFNGLGENVERTEXARRAYSPROC        glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC        glBindVertexArray;
PFNGLGENBUFFERSPROC             glGenBuffers;
PFNGLBINDBUFFERPROC             glBindBuffer;
PFNGLBUFFERDATAPROC             glBufferData;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC    glVertexAttribPointer;
PFNGLGETUNIFORMLOCATIONPROC     glGetUniformLocation;
PFNGLUNIFORM1IPROC              glUniform1i;
PFNGLUNIFORM1FPROC              glUniform1f;
PFNGLUNIFORM2FPROC              glUniform2f;
PFNGLACTIVETEXTUREPROC          glActiveTexture;
PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT;

// Standardowe funkcje OpenGL (są w opengl32.lib bezpośrednio)
extern void   APIENTRY glClear(GLbitfield);
extern void   APIENTRY glClearColor(GLfloat, GLfloat, GLfloat, GLfloat);
extern void   APIENTRY glDrawArrays(GLenum, GLint, GLsizei);
extern void   APIENTRY glGenTextures(GLsizei, GLuint*);
extern void   APIENTRY glBindTexture(GLenum, GLuint);
extern void   APIENTRY glTexImage2D(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
extern void   APIENTRY glTexParameteri(GLenum, GLenum, GLint);
extern void   APIENTRY glViewport(GLint, GLint, GLsizei, GLsizei);

// ─── Rozmiar konsoli w znakach ───────────────────────────────────────────────
#define CON_W 120
#define CON_H 30

// Rozmiar tekstury: każdy znak to 8x16 pikseli
#define GLYPH_W 8
#define GLYPH_H 16
#define TEX_W   (CON_W * GLYPH_W)
#define TEX_H   (CON_H * GLYPH_H)

// Okno
#define WIN_W 1200
#define WIN_H 600

static HWND  g_hwnd;
static HDC   g_hdc;
static HGLRC g_hglrc;
static GLuint g_program;
static GLuint g_vao;
static GLuint g_texture;
static float  g_time = 0.0f;

// ─── Vertex shader ───────────────────────────────────────────────────────────
static const char* VS = 
    "#version 330 core\n"
    "layout(location=0) in vec2 pos;\n"
    "layout(location=1) in vec2 uv;\n"
    "out vec2 vUV;\n"
    "void main() {\n"
    "    vUV = uv;\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "}\n";

// ─── Fragment shader: efekt CRT ──────────────────────────────────────────────
static const char* FS =
    "#version 330 core\n"
    "in vec2 vUV;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D uTex;\n"
    "uniform float uTime;\n"
    "uniform vec2 uRes;\n"

    // Scanlines
    "float scanline(vec2 uv) {\n"
    "    return 0.85 + 0.15 * sin(uv.y * uRes.y * 3.14159);\n"
    "}\n"

    // Zniekształcenie CRT (beczka)
    "vec2 crt_warp(vec2 uv) {\n"
    "    uv = uv * 2.0 - 1.0;\n"
    "    uv += uv * vec2(dot(uv,uv)) * 0.07;\n"
    "    return uv * 0.5 + 0.5;\n"
    "}\n"

    // Chromatic aberration (RGB split)
    "vec3 chromatic(sampler2D tex, vec2 uv) {\n"
    "    float amt = 0.003;\n"
    "    float r = texture(tex, uv + vec2( amt, 0.0)).r;\n"
    "    float g = texture(tex, uv              ).g;\n"
    "    float b = texture(tex, uv - vec2( amt, 0.0)).b;\n"
    "    return vec3(r, g, b);\n"
    "}\n"

    "void main() {\n"
    // CRT warp
    "    vec2 uv = crt_warp(vUV);\n"
    // Wytnij poza ekranem (czarne rogi)
    "    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {\n"
    "        fragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
    "        return;\n"
    "    }\n"
    // Chromatic aberration
    "    vec3 col = chromatic(uTex, uv);\n"
    // Scanlines
    "    col *= scanline(uv);\n"
    // Losowy szum (migotanie)
    "    float noise = fract(sin(dot(uv, vec2(12.9898 + uTime, 78.233))) * 43758.5453);\n"
    "    col += noise * 0.02;\n"
    // Podbij zieleń jak stary terminal
    "    col = col * vec3(0.85, 1.15, 0.85);\n"
    // Winietowanie
    "    vec2 vig = uv * (1.0 - uv.yx);\n"
    "    float vignette = pow(vig.x * vig.y * 15.0, 0.4);\n"
    "    col *= vignette;\n"
    "    fragColor = vec4(col, 1.0);\n"
    "}\n";

// ─── Ładowanie funkcji OpenGL 3.3 ────────────────────────────────────────────
static void load_gl_funcs() {
    #define LOAD(type, name) name = (type)wglGetProcAddress(#name)
    LOAD(PFNGLCREATESHADERPROC,           glCreateShader);
    LOAD(PFNGLSHADERSOURCEPROC,           glShaderSource);
    LOAD(PFNGLCOMPILESHADERPROC,          glCompileShader);
    LOAD(PFNGLGETSHADERIVPROC,            glGetShaderiv);
    LOAD(PFNGLGETSHADERINFOLOGPROC,       glGetShaderInfoLog);
    LOAD(PFNGLCREATEPROGRAMPROC,          glCreateProgram);
    LOAD(PFNGLATTACHSHADERPROC,           glAttachShader);
    LOAD(PFNGLLINKPROGRAMPROC,            glLinkProgram);
    LOAD(PFNGLUSEPROGRAMPROC,             glUseProgram);
    LOAD(PFNGLGENVERTEXARRAYSPROC,        glGenVertexArrays);
    LOAD(PFNGLBINDVERTEXARRAYPROC,        glBindVertexArray);
    LOAD(PFNGLGENBUFFERSPROC,             glGenBuffers);
    LOAD(PFNGLBINDBUFFERPROC,             glBindBuffer);
    LOAD(PFNGLBUFFERDATAPROC,             glBufferData);
    LOAD(PFNGLENABLEVERTEXATTRIBARRAYPROC,glEnableVertexAttribArray);
    LOAD(PFNGLVERTEXATTRIBPOINTERPROC,    glVertexAttribPointer);
    LOAD(PFNGLGETUNIFORMLOCATIONPROC,     glGetUniformLocation);
    LOAD(PFNGLUNIFORM1IPROC,              glUniform1i);
    LOAD(PFNGLUNIFORM1FPROC,              glUniform1f);
    LOAD(PFNGLUNIFORM2FPROC,              glUniform2f);
    LOAD(PFNGLACTIVETEXTUREPROC,          glActiveTexture);
    LOAD(PFNWGLSWAPINTERVALEXTPROC,       wglSwapIntervalEXT);
    #undef LOAD
}

// ─── Kompilacja shaderów ─────────────────────────────────────────────────────
static GLuint compile_shader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    if (!s) { printf("glCreateShader FAIL\n"); Sleep(5000); return 0; }
    
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    
    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(s, 1024, NULL, log);
        printf("Shader FAIL:\n%s\n", log);
        Sleep(10000); // 10 sekund żebyś zdążył przeczytać
        return 0;
    }
    printf("Shader OK (type=%d)\n", type);
    return s;
}

static GLuint create_program(const char* vs_src, const char* fs_src) {
    GLuint vs = compile_shader(GL_VERTEX_SHADER,   vs_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_src);
    if (!vs || !fs) { printf("program nie powstanie bo shader fail\n"); Sleep(10000); return 0; }
    
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    
    GLint ok;
    // potrzebujemy glGetProgramiv - dodaj do load_gl_funcs
    printf("Program linkowany\n");
    return p;
}

// ─── Tworzenie kontekstu OpenGL przez WGL ────────────────────────────────────
// Najpierw dummy context żeby dostać wglCreateContextAttribsARB
typedef HGLRC (APIENTRY *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
// zamień create_gl_context na tę wersję z logowaniem
static void create_gl_context(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0,
        24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };
    int pf = ChoosePixelFormat(hdc, &pfd);
    if (!pf) { printf("ChoosePixelFormat FAIL: %lu\n", GetLastError()); return; }
    if (!SetPixelFormat(hdc, pf, &pfd)) { printf("SetPixelFormat FAIL: %lu\n", GetLastError()); return; }

    HGLRC dummy = wglCreateContext(hdc);
    if (!dummy) { printf("wglCreateContext dummy FAIL: %lu\n", GetLastError()); return; }
    wglMakeCurrent(hdc, dummy);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB) { 
        printf("wglCreateContextAttribsARB niedostepne, uzywam dummy\n");
        g_hglrc = dummy;
        return; 
    }

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    g_hglrc = wglCreateContextAttribsARB(hdc, NULL, attribs);
    if (!g_hglrc) { 
        printf("wglCreateContextAttribsARB FAIL: %lu, uzywam dummy\n", GetLastError()); 
        g_hglrc = dummy;
        return;
    }
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummy);
    wglMakeCurrent(hdc, g_hglrc);
    printf("OpenGL context OK\n");
}

// ─── Czytanie bufora konsoli i rysowanie na teksturę przez GDI ───────────────
static unsigned char g_pixels[TEX_H][TEX_W][4];

// Kolory konsoli Windows → RGB
static const COLORREF CGA[16] = {
    0x000000, 0x800000, 0x008000, 0x808000,
    0x000080, 0x800080, 0x008080, 0xC0C0C0,
    0x808080, 0xFF0000, 0x00FF00, 0xFFFF00,
    0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF,
};

static void read_console_to_texture() {
    HANDLE hCon = CreateFile("CONOUT$", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                             NULL, OPEN_EXISTING, 0, NULL);
    if (hCon == INVALID_HANDLE_VALUE) return;

    CHAR_INFO buf[CON_H][CON_W];
    COORD bufSize   = { CON_W, CON_H };
    COORD bufOrigin = { 0, 0 };
    SMALL_RECT region = { 0, 0, CON_W - 1, CON_H - 1 };

    ReadConsoleOutput(hCon, (CHAR_INFO*)buf, bufSize, bufOrigin, &region);
    CloseHandle(hCon);

    // Utwórz bitmap GDI w pamięci do rysowania znaków
    HDC     memDC  = CreateCompatibleDC(NULL);
    HBITMAP hBitmap = CreateCompatibleBitmap(GetDC(NULL), TEX_W, TEX_H);
    SelectObject(memDC, hBitmap);

    // Czcionka rastrowa konsolowa 8x16
    HFONT hFont = CreateFont(
        GLYPH_H, GLYPH_W, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        OEM_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY, FIXED_PITCH | FF_DONTCARE, "Terminal"
    );
    SelectObject(memDC, hFont);
    SetBkMode(memDC, OPAQUE);

    for (int row = 0; row < CON_H; row++) {
        for (int col = 0; col < CON_W; col++) {
            CHAR_INFO* ci = &buf[row][col];
            char ch = ci->Char.AsciiChar ? ci->Char.AsciiChar : ' ';

            WORD attr = ci->Attributes;
            int fgIdx = attr & 0x0F;
            int bgIdx = (attr >> 4) & 0x0F;

            COLORREF fg = CGA[fgIdx];
            COLORREF bg = CGA[bgIdx];

            int x = col * GLYPH_W;
            int y = row * GLYPH_H;

            SetTextColor(memDC, fg);
            SetBkColor(memDC, bg);
            TextOutA(memDC, x, y, &ch, 1);
        }
    }

    // Skopiuj piksele z bitmap do tablicy
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = TEX_W;
    bmi.bmiHeader.biHeight      = -TEX_H; // ujemne = top-down
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    GetDIBits(memDC, hBitmap, 0, TEX_H, g_pixels, &bmi, DIB_RGB_COLORS);

    // GDI daje BGR, zamieniamy na RGBA
    for (int i = 0; i < TEX_H * TEX_W; i++) {
        unsigned char* p = (unsigned char*)g_pixels + i * 4;
        unsigned char tmp = p[0]; p[0] = p[2]; p[2] = tmp; // swap R i B
        p[3] = 255;
    }

    DeleteObject(hFont);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
}

// ─── Inicjalizacja OpenGL ─────────────────────────────────────────────────────
static void gl_init() {
    load_gl_funcs();

    if (!glCreateShader)       { printf("glCreateShader NULL\n");       Sleep(10000); return; }
    if (!glCreateProgram)      { printf("glCreateProgram NULL\n");      Sleep(10000); return; }
    if (!glGenVertexArrays)    { printf("glGenVertexArrays NULL\n");    Sleep(10000); return; }
    if (!glBindVertexArray)    { printf("glBindVertexArray NULL\n");    Sleep(10000); return; }
    if (!glGenBuffers)         { printf("glGenBuffers NULL\n");         Sleep(10000); return; }
    if (!glBindBuffer)         { printf("glBindBuffer NULL\n");         Sleep(10000); return; }
    if (!glBufferData)         { printf("glBufferData NULL\n");         Sleep(10000); return; }
    if (!glEnableVertexAttribArray) { printf("glEnableVertexAttribArray NULL\n"); Sleep(10000); return; }
    if (!glVertexAttribPointer)     { printf("glVertexAttribPointer NULL\n");     Sleep(10000); return; }
    printf("wszystkie funkcje OK\n");


    if (wglSwapIntervalEXT) wglSwapIntervalEXT(1); // vsync

    
    float quad[] = {
       -1,-1,  0,1,
        1,-1,  1,1,
       -1, 1,  0,0,
        1, 1,  1,0,
    };
    printf("quad ok\n");

    GLuint vbo;
    glGenVertexArrays(1, &g_vao);
    printf("VAO=%u\n", g_vao);
    
    glBindVertexArray(g_vao);
    printf("bind VAO ok\n");
    
    glGenBuffers(1, &vbo);
    printf("VBO=%u\n", vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    printf("bind VBO ok\n");
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    printf("bufferdata ok\n");
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    printf("attrib 0 ok\n");
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    printf("attrib 1 ok\n");

    g_program = create_program(VS, FS);
    printf("program=%u\n", g_program);

    glGenTextures(1, &g_texture);
    printf("texture=%u\n", g_texture);
    
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    printf("gl_init done\n");

    // for(;;){}
}

// ─── Render ───────────────────────────────────────────────────────────────────
static void render() {
    g_time += 0.016f;

    static unsigned char pixels[TEX_H][TEX_W][4];
    for (int y = 0; y < TEX_H; y++) {
        for (int x = 0; x < TEX_W; x++) {
            pixels[y][x][0] = (unsigned char)(x * 255 / TEX_W);
            pixels[y][x][1] = (unsigned char)(y * 255 / TEX_H);
            pixels[y][x][2] = 100;
            pixels[y][x][3] = 255;
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_W, TEX_H, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_program);
    glUniform1i(glGetUniformLocation(g_program, "uTex"),  0);
    glUniform1f(glGetUniformLocation(g_program, "uTime"), g_time);
    glUniform2f(glGetUniformLocation(g_program, "uRes"),  (float)WIN_W, (float)WIN_H);

    glBindVertexArray(g_vao);
    printf("przed draw\n");
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    printf("po draw\n");

        printf("dzila");
    for(;;){}
    SwapBuffers(g_hdc);
    
}
    
    // g_time += 0.016f;

    // read_console_to_texture();
    
    // // sprawdź czy tekstura w ogóle powstała
    // static int first = 1;
    // if (first) {
    //     first = 0;
    //     printf("pierwszy render, tekstura=%u\n", g_texture);
    //     // wypełnij teksturę testowym kolorem zamiast konsoli
    //     unsigned char test[4] = {255, 0, 255, 255}; // magenta
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, g_texture);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
    //                  GL_RGBA, GL_UNSIGNED_BYTE, test);
    //     printf("test textura wgrana\n");
    // }

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, g_texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_W, TEX_H, 0,
    //              GL_RGBA, GL_UNSIGNED_BYTE, g_pixels);

    // glClearColor(0, 0, 0, 1);
    // glClear(GL_COLOR_BUFFER_BIT);

    // glUseProgram(g_program);
    // glUniform1i(glGetUniformLocation(g_program, "uTex"),  0);
    // glUniform1f(glGetUniformLocation(g_program, "uTime"), g_time);
    // glUniform2f(glGetUniformLocation(g_program, "uRes"),  (float)WIN_W, (float)WIN_H);

    // glBindVertexArray(g_vao);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // SwapBuffers(g_hdc);
// }

// ─── WndProc ──────────────────────────────────────────────────────────────────
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_SIZE:
            glViewport(0, 0, LOWORD(lp), HIWORD(lp));
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (wp == VK_ESCAPE) PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// ─── WinMain ──────────────────────────────────────────────────────────────────
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    // Podepnij konsolę żebyś miał skąd czytać output
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$",  "r", stdin);

    // Przykładowy output w konsoli — zastąp swoim kodem
    SetConsoleTitleA("console_gl source");
    for (int i = 0; i < 5; i++)
        printf("Linia %d: Hello from console! abcdefghijklmnopqrstuvwxyz 0123456789\n", i);

    // Rejestracja klasy okna
    WNDCLASSEXA wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "ConsoleGL";
    RegisterClassExA(&wc);

    g_hwnd = CreateWindowExA(0, "ConsoleGL", "Console → OpenGL CRT",
                              WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                              100, 100, WIN_W, WIN_H,
                              NULL, NULL, hInst, NULL);

    g_hdc = GetDC(g_hwnd);
    create_gl_context(g_hdc);
    gl_init();

    MSG msg = {0};
    while (1) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) goto done;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        render();
        Sleep(1); // daj windowsowi odetchnąć
    }
done:
    printf("zamknieto\n");
    return 0;
}

// int main(){
//     AllocConsole();
//     printf("Hello world!!\n glglgglg\n\ngl");
//     for(;;){};
//     return 0;
// }