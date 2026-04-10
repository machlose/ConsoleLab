// OpenGL 3.3 - przechwytuje okno konsoli jako teksture i wyswietla z shaderem
// Kompilacja: gcc main.c -o main.exe -lopengl32 -lgdi32 -luser32 -mwindows

#include <windows.h>
#include <gl/gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// -------------------------------------------------------
//  Konsola - tworzymy ja recznie zeby miec HWND
// -------------------------------------------------------
static HWND  g_console_hwnd = NULL;
static FILE *g_console_out  = NULL;

static void console_init(void)
{
    AllocConsole();
    freopen_s(&g_console_out, "CONOUT$", "w", stdout);
    freopen_s(&g_console_out, "CONOUT$", "w", stderr);
    g_console_hwnd = GetConsoleWindow();

    // Ustaw rozmiar i pozycje konsoli
    MoveWindow(g_console_hwnd, 20, 20, 800, 400, TRUE);

    // Ukryj kursor konsoli (estetyka)
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

// Zwykly printf - idzie do konsoli
static void con_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

// -------------------------------------------------------
//  WGL extensions
// -------------------------------------------------------
typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC, HGLRC, const int*);
typedef BOOL  WINAPI wglChoosePixelFormatARB_type(HDC, const int*, const FLOAT*, UINT, int*, UINT*);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;
wglChoosePixelFormatARB_type    *wglChoosePixelFormatARB;

#define WGL_DRAW_TO_WINDOW_ARB    0x2001
#define WGL_ACCELERATION_ARB      0x2003
#define WGL_SUPPORT_OPENGL_ARB    0x2010
#define WGL_DOUBLE_BUFFER_ARB     0x2011
#define WGL_PIXEL_TYPE_ARB        0x2013
#define WGL_COLOR_BITS_ARB        0x2014
#define WGL_DEPTH_BITS_ARB        0x2022
#define WGL_STENCIL_BITS_ARB      0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB         0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

// -------------------------------------------------------
//  OpenGL function pointers
// -------------------------------------------------------
typedef unsigned int GLuint;
typedef int          GLint;
typedef int          GLsizei;
typedef unsigned int GLenum;
typedef float        GLfloat;
typedef char         GLchar;
typedef ptrdiff_t    GLsizeiptr;

#define GL_ARRAY_BUFFER    0x8892
#define GL_STATIC_DRAW     0x88B4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER   0x8B31
#define GL_COMPILE_STATUS  0x8B81
#define GL_LINK_STATUS     0x8B82
#define GL_TEXTURE0        0x84C0
#define GL_CLAMP_TO_EDGE   0x812F
#define GL_TEXTURE_WRAP_S  0x2802
#define GL_TEXTURE_WRAP_T  0x2803
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_LINEAR          0x2601

typedef void   (WINAPI *PFN_glGenBuffers)             (GLsizei, GLuint*);
typedef void   (WINAPI *PFN_glBindBuffer)             (GLenum, GLuint);
typedef void   (WINAPI *PFN_glBufferData)             (GLenum, GLsizeiptr, const void*, GLenum);
typedef void   (WINAPI *PFN_glGenVertexArrays)        (GLsizei, GLuint*);
typedef void   (WINAPI *PFN_glBindVertexArray)        (GLuint);
typedef void   (WINAPI *PFN_glEnableVertexAttribArray)(GLuint);
typedef void   (WINAPI *PFN_glVertexAttribPointer)    (GLuint, GLint, GLenum, unsigned char, GLsizei, const void*);
typedef GLuint (WINAPI *PFN_glCreateShader)           (GLenum);
typedef void   (WINAPI *PFN_glShaderSource)           (GLuint, GLsizei, const GLchar**, const GLint*);
typedef void   (WINAPI *PFN_glCompileShader)          (GLuint);
typedef GLuint (WINAPI *PFN_glCreateProgram)          (void);
typedef void   (WINAPI *PFN_glAttachShader)           (GLuint, GLuint);
typedef void   (WINAPI *PFN_glLinkProgram)            (GLuint);
typedef void   (WINAPI *PFN_glUseProgram)             (GLuint);
typedef void   (WINAPI *PFN_glGetShaderiv)            (GLuint, GLenum, GLint*);
typedef void   (WINAPI *PFN_glGetShaderInfoLog)       (GLuint, GLsizei, GLsizei*, GLchar*);
typedef GLint  (WINAPI *PFN_glGetUniformLocation)     (GLuint, const GLchar*);
typedef void   (WINAPI *PFN_glUniform1i)              (GLint, GLint);
typedef void   (WINAPI *PFN_glUniform1f)              (GLint, GLfloat);
typedef void   (WINAPI *PFN_glActiveTexture)          (GLenum);
typedef void   (WINAPI *PFN_glGenerateMipmap)         (GLenum);

static PFN_glGenBuffers              glGenBuffers;
static PFN_glBindBuffer              glBindBuffer;
static PFN_glBufferData              glBufferData;
static PFN_glGenVertexArrays         glGenVertexArrays;
static PFN_glBindVertexArray         glBindVertexArray;
static PFN_glEnableVertexAttribArray glEnableVertexAttribArray;
static PFN_glVertexAttribPointer     glVertexAttribPointer;
static PFN_glCreateShader            glCreateShader;
static PFN_glShaderSource            glShaderSource;
static PFN_glCompileShader           glCompileShader;
static PFN_glCreateProgram           glCreateProgram;
static PFN_glAttachShader            glAttachShader;
static PFN_glLinkProgram             glLinkProgram;
static PFN_glUseProgram              glUseProgram;
static PFN_glGetShaderiv             glGetShaderiv;
static PFN_glGetShaderInfoLog        glGetShaderInfoLog;
static PFN_glGetUniformLocation      glGetUniformLocation;
static PFN_glUniform1i               glUniform1i;
static PFN_glUniform1f               glUniform1f;
static PFN_glActiveTexture           glActiveTexture;
static PFN_glGenerateMipmap          glGenerateMipmap;

static void *gl_proc(const char *n) { return (void*)wglGetProcAddress(n); }

static void load_gl_functions(void) {
    glGenBuffers              = gl_proc("glGenBuffers");
    glBindBuffer              = gl_proc("glBindBuffer");
    glBufferData              = gl_proc("glBufferData");
    glGenVertexArrays         = gl_proc("glGenVertexArrays");
    glBindVertexArray         = gl_proc("glBindVertexArray");
    glEnableVertexAttribArray = gl_proc("glEnableVertexAttribArray");
    glVertexAttribPointer     = gl_proc("glVertexAttribPointer");
    glCreateShader            = gl_proc("glCreateShader");
    glShaderSource            = gl_proc("glShaderSource");
    glCompileShader           = gl_proc("glCompileShader");
    glCreateProgram           = gl_proc("glCreateProgram");
    glAttachShader            = gl_proc("glAttachShader");
    glLinkProgram             = gl_proc("glLinkProgram");
    glUseProgram              = gl_proc("glUseProgram");
    glGetShaderiv             = gl_proc("glGetShaderiv");
    glGetShaderInfoLog        = gl_proc("glGetShaderInfoLog");
    glGetUniformLocation      = gl_proc("glGetUniformLocation");
    glUniform1i               = gl_proc("glUniform1i");
    glUniform1f               = gl_proc("glUniform1f");
    glActiveTexture           = gl_proc("glActiveTexture");
    glGenerateMipmap          = gl_proc("glGenerateMipmap");
}

// -------------------------------------------------------
//  Przechwytywanie okna konsoli -> piksele RGBA
// -------------------------------------------------------
#define CAP_W 800
#define CAP_H 400

static unsigned char g_cap_pixels[CAP_H * CAP_W * 4];

// Robi screenshot okna konsoli i wrzuca piksele do g_cap_pixels
static void capture_console(void)
{
    if (!g_console_hwnd) return;

    HDC     hdc_screen = GetDC(g_console_hwnd);
    HDC     hdc_mem    = CreateCompatibleDC(hdc_screen);
    HBITMAP hbmp       = CreateCompatibleBitmap(hdc_screen, CAP_W, CAP_H);
    SelectObject(hdc_mem, hbmp);

    // Kopiuj zawartosc okna konsoli do bitmapy
    PrintWindow(g_console_hwnd, hdc_mem, PW_CLIENTONLY);

    // Wyciagnij piksele
    BITMAPINFOHEADER bi = {0};
    bi.biSize        = sizeof(bi);
    bi.biWidth       = CAP_W;
    bi.biHeight      = -CAP_H; // ujemna = top-down
    bi.biPlanes      = 1;
    bi.biBitCount    = 32;
    bi.biCompression = BI_RGB;

    // Tymczasowy bufor BGR
    static unsigned char bgr[CAP_H * CAP_W * 4];
    GetDIBits(hdc_mem, hbmp, 0, CAP_H, bgr, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // Zamien BGR -> RGBA
    for (int i = 0; i < CAP_W * CAP_H; i++) {
        g_cap_pixels[i*4+0] = bgr[i*4+2]; // R
        g_cap_pixels[i*4+1] = bgr[i*4+1]; // G
        g_cap_pixels[i*4+2] = bgr[i*4+0]; // B
        g_cap_pixels[i*4+3] = 255;         // A
    }

    DeleteObject(hbmp);
    DeleteDC(hdc_mem);
    ReleaseDC(g_console_hwnd, hdc_screen);
}

// -------------------------------------------------------
//  Shadery - fullscreen quad z efektem na teksturze
// -------------------------------------------------------
static const char *vert_src =
    "#version 330 core\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec2 aUV;\n"
    "out vec2 vUV;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "  vUV = aUV;\n"
    "}\n";

// Shader: odwzorowuje teksture konsoli + efekt CRT (scanlines + lekka winietka)
static const char *frag_src =
    "#version 330 core\n"
    "in vec2 vUV;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D uTex;\n"
    "uniform float uTime;\n"
    "void main() {\n"
    "  vec2 uv = vUV;\n"
    // lekkie zaokraglenie naroznikow (CRT)
    "  vec2 crt = (uv - 0.5) * 2.0;\n"
    "  float vignette = 1.0 - dot(crt * 0.42, crt * 0.42);\n"
    "  vignette = clamp(vignette, 0.0, 1.0);\n"
    // scanlines
    "  float scan = 0.85 + 0.15 * sin(uv.y * 600.0);\n"
    // chromatic aberration - lekkie przesuniecie kanalow
    "  float shift = 0.002;\n"
    "  float r = texture(uTex, uv + vec2(shift, 0.0)).r;\n"
    "  float g = texture(uTex, uv).g;\n"
    "  float b = texture(uTex, uv - vec2(shift, 0.0)).b;\n"
    // delikatne zielone zabarwienie (retro terminal)
    "  vec3 col = vec3(r, g, b);\n"
    "  col = mix(col, col * vec3(0.7, 1.0, 0.7), 0.25);\n"
    "  col *= scan * vignette;\n"
    "  FragColor = vec4(col, 1.0);\n"
    "}\n";

// -------------------------------------------------------
//  Stan GL
// -------------------------------------------------------
static GLuint g_program;
static GLuint g_vao, g_vbo;
static GLuint g_texture;
static int    g_win_w = 1024, g_win_h = 576;
static float  g_time  = 0.0f;

static GLuint compile_shader(GLenum type, const char *src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[512]; glGetShaderInfoLog(s, 512, NULL, buf);
        MessageBoxA(NULL, buf, "Shader error", MB_OK|MB_ICONERROR);
    }
    return s;
}

static void init_gl(void)
{
    // Fullscreen quad: dwa trojkaty pokrywajace caly ekran
    float verts[] = {
    // x      y     u     v
      -1.0f,  1.0f, 0.0f, 0.0f,
      -1.0f, -1.0f, 0.0f, 1.0f,
       1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 0.0f,
       1.0f, -1.0f, 1.0f, 1.0f,
       1.0f,  1.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);
    glGenBuffers(1, &g_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, 0x1406/*GL_FLOAT*/, 0, 4*4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, 0x1406/*GL_FLOAT*/, 0, 4*4, (void*)(2*4));
    glBindVertexArray(0);

    g_program = glCreateProgram();
    glAttachShader(g_program, compile_shader(GL_VERTEX_SHADER,   vert_src));
    glAttachShader(g_program, compile_shader(GL_FRAGMENT_SHADER, frag_src));
    glLinkProgram(g_program);

    // Tekstura na screenshot konsoli
    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Pierwsza inicjalizacja - pusta tekstura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CAP_W, CAP_H, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

// Aktualizuje teksture nowym screenshotem konsoli
static void update_console_texture(void)
{
    capture_console();
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CAP_W, CAP_H, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, g_cap_pixels);
}

static void render(void)
{
    glViewport(0, 0, g_win_w, g_win_h);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glUniform1i(glGetUniformLocation(g_program, "uTex"),  0);
    glUniform1f(glGetUniformLocation(g_program, "uTime"), g_time);

    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    g_time += 0.016f;
}

// -------------------------------------------------------
//  WGL / okno OpenGL
// -------------------------------------------------------
static void fatal_error(const char *msg) {
    MessageBoxA(NULL, msg, "Fatal Error", MB_OK|MB_ICONEXCLAMATION);
    exit(1);
}

static void init_opengl_extensions(void) {
    WNDCLASSA wc = {
        .style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC, .lpfnWndProc=DefWindowProcA,
        .hInstance=GetModuleHandle(0), .lpszClassName="DWGL",
    };
    RegisterClassA(&wc);
    HWND dw = CreateWindowExA(0,"DWGL","",0,0,0,1,1,0,0,wc.hInstance,0);
    HDC dc = GetDC(dw);
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(pfd),1,PFD_TYPE_RGBA,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
        32,0,0,0,0,0,0,8,0,0,0,0,0,0,PFD_MAIN_PLANE,0,0,24,8,0
    };
    SetPixelFormat(dc, ChoosePixelFormat(dc,&pfd), &pfd);
    HGLRC ctx = wglCreateContext(dc);
    wglMakeCurrent(dc, ctx);
    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB    = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");
    wglMakeCurrent(dc,0); wglDeleteContext(ctx);
    ReleaseDC(dw,dc); DestroyWindow(dw);
}

static HGLRC init_opengl(HDC dc) {
    init_opengl_extensions();
    int pfa[] = {
        WGL_DRAW_TO_WINDOW_ARB,GL_TRUE, WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,GL_TRUE,  WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,WGL_TYPE_RGBA_ARB, WGL_COLOR_BITS_ARB,32,
        WGL_DEPTH_BITS_ARB,24, WGL_STENCIL_BITS_ARB,8, 0
    };
    int pf; UINT nf;
    wglChoosePixelFormatARB(dc, pfa, 0, 1, &pf, &nf);
    if (!nf) fatal_error("No pixel format.");
    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(dc, pf, sizeof(pfd), &pfd);
    SetPixelFormat(dc, pf, &pfd);
    int att[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,3, WGL_CONTEXT_MINOR_VERSION_ARB,3,
        WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0
    };
    HGLRC ctx = wglCreateContextAttribsARB(dc, 0, att);
    if (!ctx) fatal_error("GL context failed.");
    wglMakeCurrent(dc, ctx);
    return ctx;
}

static LRESULT CALLBACK WndProc(HWND w, UINT m, WPARAM wp, LPARAM lp) {
    if (m == WM_SIZE) { g_win_w=LOWORD(lp); g_win_h=HIWORD(lp); }
    if (m == WM_CLOSE || m == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProcA(w, m, wp, lp);
}

static HWND create_window(HINSTANCE inst) {
    WNDCLASSA wc = {
        .style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC, .lpfnWndProc=WndProc,
        .hInstance=inst, .hCursor=LoadCursor(0,IDC_ARROW), .lpszClassName="OGL",
    };
    RegisterClassA(&wc);
    RECT r={0,0,1024,576}; DWORD style=WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&r,style,0);
    HWND w = CreateWindowExA(0,"OGL","Console Shader",style,
        860,20, r.right-r.left,r.bottom-r.top, 0,0,inst,0);
    if (!w) fatal_error("Window failed.");
    return w;
}

// -------------------------------------------------------
//  ENTRY POINT
// -------------------------------------------------------
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
    // 1. Otworz konsole i umiesc ja po lewej stronie ekranu
    console_init();

    // 2. Wypisz co chcesz w konsoli - to pojawi sie w oknie GL
    con_print("=== OpenGL Console Shader ===");
    con_print("");

    // 3. Stworz okno OpenGL (po prawej stronie)
    HWND  win = create_window(inst);
    HDC   dc  = GetDC(win);
    HGLRC glrc = init_opengl(dc);
    load_gl_functions();

    con_print("GL Version:  %s", glGetString(GL_VERSION));
    con_print("GL Renderer: %s", glGetString(GL_RENDERER));
    con_print("GL Vendor:   %s", glGetString(GL_VENDOR));
    con_print("");
    con_print("Okno OpenGL wyswietla powyzszy");
    con_print("tekst przez shader CRT.");
    con_print("");
    con_print("Efekty: scanlines + vignette");
    con_print("        + chromatic aberration");
    con_print("        + retro terminal green");

    init_gl();

    ShowWindow(win, show);
    UpdateWindow(win);

    // Odczekaj chwile zeby konsola zdazyla sie wyrenderowac
    Sleep(200);

    bool running = true;
    int  frame   = 0;
    while (running) {
        MSG msg;
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            else { TranslateMessage(&msg); DispatchMessageA(&msg); }
        }

        // Odswiez screenshot konsoli co 10 klatek
        if (frame % 10 == 0)
            update_console_texture();

        render();
        SwapBuffers(dc);
        Sleep(16);
        frame++;
    }
    return 0;
}