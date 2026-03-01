// OpenGL 3.3 - konsola + log w oknie OpenGL
// Kompilacja: gcc main.c -o main.exe -lopengl32 -lgdi32 -luser32
// (bez -mwindows = konsola dziala normalnie)

#include <windows.h>
#include <gl/gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// -------------------------------------------------------
//  LOG - wyswietlany w oknie OpenGL i w konsoli
// -------------------------------------------------------
#define LOG_MAX_LINES 32
#define LOG_LINE_LEN  120
static char  g_log[LOG_MAX_LINES][LOG_LINE_LEN];
static int   g_log_count = 0;

// Uzyj tej funkcji zamiast printf - idzie do konsoli I do okna GL
static void con_print(const char *fmt, ...)
{
    char buf[LOG_LINE_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    // -> konsola
    printf("%s\n", buf);

    // -> log w oknie GL
    if (g_log_count < LOG_MAX_LINES) {
        strncpy(g_log[g_log_count], buf, LOG_LINE_LEN - 1);
        g_log[g_log_count][LOG_LINE_LEN - 1] = '\0';
        g_log_count++;
    }
}

// -------------------------------------------------------
//  WGL extensions
// -------------------------------------------------------
typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC, HGLRC, const int*);
typedef BOOL  WINAPI wglChoosePixelFormatARB_type(HDC, const int*, const FLOAT*, UINT, int*, UINT*);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;
wglChoosePixelFormatARB_type    *wglChoosePixelFormatARB;

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

// -------------------------------------------------------
//  OpenGL 3.3 function pointers
// -------------------------------------------------------
typedef unsigned int  GLuint;
typedef int           GLint;
typedef int           GLsizei;
typedef unsigned int  GLenum;
typedef float         GLfloat;
typedef char          GLchar;
typedef ptrdiff_t     GLsizeiptr;

#define GL_ARRAY_BUFFER         0x8892
#define GL_STATIC_DRAW          0x88B4
#define GL_DYNAMIC_DRAW         0x88E8
#define GL_FRAGMENT_SHADER      0x8B30
#define GL_VERTEX_SHADER        0x8B31
#define GL_COMPILE_STATUS       0x8B81
#define GL_LINK_STATUS          0x8B82
#define GL_TEXTURE0             0x84C0
#define GL_BLEND                0x0BE2
#define GL_SRC_ALPHA            0x0302
#define GL_ONE_MINUS_SRC_ALPHA  0x0303
#define GL_TEXTURE_MIN_FILTER   0x2801
#define GL_TEXTURE_MAG_FILTER   0x2800
#define GL_NEAREST              0x2600

typedef void    (WINAPI *PFNGLGENBUFFERSPROC)            (GLsizei, GLuint*);
typedef void    (WINAPI *PFNGLBINDBUFFERPROC)            (GLenum, GLuint);
typedef void    (WINAPI *PFNGLBUFFERDATAPROC)            (GLenum, GLsizeiptr, const void*, GLenum);
typedef void    (WINAPI *PFNGLBUFFERSUBDATAPROC)         (GLenum, ptrdiff_t, GLsizeiptr, const void*);
typedef void    (WINAPI *PFNGLGENVERTEXARRAYSPROC)        (GLsizei, GLuint*);
typedef void    (WINAPI *PFNGLBINDVERTEXARRAYPROC)        (GLuint);
typedef void    (WINAPI *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void    (WINAPI *PFNGLVERTEXATTRIBPOINTERPROC)    (GLuint, GLint, GLenum, unsigned char, GLsizei, const void*);
typedef GLuint  (WINAPI *PFNGLCREATESHADERPROC)           (GLenum);
typedef void    (WINAPI *PFNGLSHADERSOURCEPROC)           (GLuint, GLsizei, const GLchar**, const GLint*);
typedef void    (WINAPI *PFNGLCOMPILESHADERPROC)          (GLuint);
typedef GLuint  (WINAPI *PFNGLCREATEPROGRAMPROC)          (void);
typedef void    (WINAPI *PFNGLATTACHSHADERPROC)           (GLuint, GLuint);
typedef void    (WINAPI *PFNGLLINKPROGRAMPROC)            (GLuint);
typedef void    (WINAPI *PFNGLUSEPROGRAMPROC)             (GLuint);
typedef void    (WINAPI *PFNGLGETSHADERIVPROC)            (GLuint, GLenum, GLint*);
typedef void    (WINAPI *PFNGLGETSHADERINFOLOGPROC)       (GLuint, GLsizei, GLsizei*, GLchar*);
typedef void    (WINAPI *PFNGLGETPROGRAMIVPROC)           (GLuint, GLenum, GLint*);
typedef GLint   (WINAPI *PFNGLGETUNIFORMLOCATIONPROC)     (GLuint, const GLchar*);
typedef void    (WINAPI *PFNGLUNIFORM1IPROC)              (GLint, GLint);
typedef void    (WINAPI *PFNGLUNIFORM1FPROC)              (GLint, GLfloat);
typedef void    (WINAPI *PFNGLUNIFORM2FPROC)              (GLint, GLfloat, GLfloat);
typedef void    (WINAPI *PFNGLUNIFORM3FPROC)              (GLint, GLfloat, GLfloat, GLfloat);
typedef void    (WINAPI *PFNGLACTIVETEXTUREPROC)          (GLenum);

static PFNGLGENBUFFERSPROC              glGenBuffers;
static PFNGLBINDBUFFERPROC              glBindBuffer;
static PFNGLBUFFERDATAPROC              glBufferData;
static PFNGLBUFFERSUBDATAPROC           glBufferSubData_fn;
static PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
static PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
static PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
static PFNGLCREATESHADERPROC            glCreateShader;
static PFNGLSHADERSOURCEPROC            glShaderSource;
static PFNGLCOMPILESHADERPROC           glCompileShader;
static PFNGLCREATEPROGRAMPROC           glCreateProgram;
static PFNGLATTACHSHADERPROC            glAttachShader;
static PFNGLLINKPROGRAMPROC             glLinkProgram;
static PFNGLUSEPROGRAMPROC              glUseProgram;
static PFNGLGETSHADERIVPROC             glGetShaderiv;
static PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;
static PFNGLGETPROGRAMIVPROC            glGetProgramiv;
static PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
static PFNGLUNIFORM1IPROC               glUniform1i;
static PFNGLUNIFORM1FPROC               glUniform1f;
static PFNGLUNIFORM2FPROC               glUniform2f;
static PFNGLUNIFORM3FPROC               glUniform3f;
static PFNGLACTIVETEXTUREPROC           glActiveTexture;

static void *gl_proc(const char *name) { return (void*)wglGetProcAddress(name); }

static void load_gl_functions(void) {
    glGenBuffers              = gl_proc("glGenBuffers");
    glBindBuffer              = gl_proc("glBindBuffer");
    glBufferData              = gl_proc("glBufferData");
    glBufferSubData_fn        = gl_proc("glBufferSubData");
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
    glGetProgramiv            = gl_proc("glGetProgramiv");
    glGetUniformLocation      = gl_proc("glGetUniformLocation");
    glUniform1i               = gl_proc("glUniform1i");
    glUniform1f               = gl_proc("glUniform1f");
    glUniform2f               = gl_proc("glUniform2f");
    glUniform3f               = gl_proc("glUniform3f");
    glActiveTexture           = gl_proc("glActiveTexture");
}

// -------------------------------------------------------
//  SHADERY - trojkat
// -------------------------------------------------------
static const char *tri_vert_src =
    "#version 330 core\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec3 aColor;\n"
    "out vec3 vColor;\n"
    "uniform float uAngle;\n"
    "void main() {\n"
    "  float c = cos(uAngle), s = sin(uAngle);\n"
    "  mat2 rot = mat2(c,-s,s,c);\n"
    "  gl_Position = vec4(rot * aPos, 0.0, 1.0);\n"
    "  vColor = aColor;\n"
    "}\n";

static const char *tri_frag_src =
    "#version 330 core\n"
    "in vec3 vColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(vColor, 1.0);\n"
    "}\n";

// -------------------------------------------------------
//  SHADERY - tekst
// -------------------------------------------------------
static const char *txt_vert_src =
    "#version 330 core\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec2 aUV;\n"
    "out vec2 vUV;\n"
    "uniform vec2 uScreen;\n"
    "void main() {\n"
    "  vec2 ndc = (aPos / uScreen) * 2.0 - 1.0;\n"
    "  ndc.y = -ndc.y;\n"
    "  gl_Position = vec4(ndc, 0.0, 1.0);\n"
    "  vUV = aUV;\n"
    "}\n";

static const char *txt_frag_src =
    "#version 330 core\n"
    "in vec2 vUV;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D uFont;\n"
    "uniform vec3 uColor;\n"
    "void main() {\n"
    "  float a = texture(uFont, vUV).a;\n"
    "  FragColor = vec4(uColor, a);\n"
    "}\n";

// -------------------------------------------------------
//  Czcionka bitmapowa 8x8 (ASCII 32-127)
// -------------------------------------------------------
#define FONT_COLS  16
#define FONT_ROWS   6
#define GLYPH_W     8
#define GLYPH_H     8
#define FONT_TEX_W (FONT_COLS * GLYPH_W)
#define FONT_TEX_H (FONT_ROWS * GLYPH_H)

static const unsigned char font8x8[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // ' '
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, // !
    {0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00}, // "
    {0x36,0x36,0x7F,0x36,0x7F,0x36,0x36,0x00}, // #
    {0x0C,0x3E,0x03,0x1E,0x30,0x1F,0x0C,0x00}, // $
    {0x00,0x63,0x33,0x18,0x0C,0x66,0x63,0x00}, // %
    {0x1C,0x36,0x1C,0x6E,0x3B,0x33,0x6E,0x00}, // &
    {0x06,0x06,0x03,0x00,0x00,0x00,0x00,0x00}, // '
    {0x18,0x0C,0x06,0x06,0x06,0x0C,0x18,0x00}, // (
    {0x06,0x0C,0x18,0x18,0x18,0x0C,0x06,0x00}, // )
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, // *
    {0x00,0x0C,0x0C,0x3F,0x0C,0x0C,0x00,0x00}, // +
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x06}, // ,
    {0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00}, // -
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00}, // .
    {0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00}, // /
    {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0x00}, // 0
    {0x0C,0x0E,0x0C,0x0C,0x0C,0x0C,0x3F,0x00}, // 1
    {0x1E,0x33,0x30,0x1C,0x06,0x33,0x3F,0x00}, // 2
    {0x1E,0x33,0x30,0x1C,0x30,0x33,0x1E,0x00}, // 3
    {0x38,0x3C,0x36,0x33,0x7F,0x30,0x78,0x00}, // 4
    {0x3F,0x03,0x1F,0x30,0x30,0x33,0x1E,0x00}, // 5
    {0x1C,0x06,0x03,0x1F,0x33,0x33,0x1E,0x00}, // 6
    {0x3F,0x33,0x30,0x18,0x0C,0x0C,0x0C,0x00}, // 7
    {0x1E,0x33,0x33,0x1E,0x33,0x33,0x1E,0x00}, // 8
    {0x1E,0x33,0x33,0x3E,0x30,0x18,0x0E,0x00}, // 9
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x00}, // :
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x06}, // ;
    {0x18,0x0C,0x06,0x03,0x06,0x0C,0x18,0x00}, // <
    {0x00,0x00,0x3F,0x00,0x00,0x3F,0x00,0x00}, // =
    {0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x00}, // >
    {0x1E,0x33,0x30,0x18,0x0C,0x00,0x0C,0x00}, // ?
    {0x3E,0x63,0x7B,0x7B,0x7B,0x03,0x1E,0x00}, // @
    {0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00}, // A
    {0x3F,0x66,0x66,0x3E,0x66,0x66,0x3F,0x00}, // B
    {0x3C,0x66,0x03,0x03,0x03,0x66,0x3C,0x00}, // C
    {0x1F,0x36,0x66,0x66,0x66,0x36,0x1F,0x00}, // D
    {0x7F,0x46,0x16,0x1E,0x16,0x46,0x7F,0x00}, // E
    {0x7F,0x46,0x16,0x1E,0x16,0x06,0x0F,0x00}, // F
    {0x3C,0x66,0x03,0x03,0x73,0x66,0x7C,0x00}, // G
    {0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x00}, // H
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, // I
    {0x78,0x30,0x30,0x30,0x33,0x33,0x1E,0x00}, // J
    {0x67,0x66,0x36,0x1E,0x36,0x66,0x67,0x00}, // K
    {0x0F,0x06,0x06,0x06,0x46,0x66,0x7F,0x00}, // L
    {0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00}, // M
    {0x63,0x67,0x6F,0x7B,0x73,0x63,0x63,0x00}, // N
    {0x1C,0x36,0x63,0x63,0x63,0x36,0x1C,0x00}, // O
    {0x3F,0x66,0x66,0x3E,0x06,0x06,0x0F,0x00}, // P
    {0x1E,0x33,0x33,0x33,0x3B,0x1E,0x38,0x00}, // Q
    {0x3F,0x66,0x66,0x3E,0x36,0x66,0x67,0x00}, // R
    {0x1E,0x33,0x07,0x0E,0x38,0x33,0x1E,0x00}, // S
    {0x3F,0x2D,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, // T
    {0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x00}, // U
    {0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00}, // V
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, // W
    {0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00}, // X
    {0x33,0x33,0x33,0x1E,0x0C,0x0C,0x1E,0x00}, // Y
    {0x7F,0x63,0x31,0x18,0x4C,0x66,0x7F,0x00}, // Z
    {0x1E,0x06,0x06,0x06,0x06,0x06,0x1E,0x00}, // [
    {0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00}, // backslash
    {0x1E,0x18,0x18,0x18,0x18,0x18,0x1E,0x00}, // ]
    {0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00}, // ^
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF}, // _
    {0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,0x00}, // `
    {0x00,0x00,0x1E,0x30,0x3E,0x33,0x6E,0x00}, // a
    {0x07,0x06,0x06,0x3E,0x66,0x66,0x3B,0x00}, // b
    {0x00,0x00,0x1E,0x33,0x03,0x33,0x1E,0x00}, // c
    {0x38,0x30,0x30,0x3e,0x33,0x33,0x6E,0x00}, // d
    {0x00,0x00,0x1E,0x33,0x3f,0x03,0x1E,0x00}, // e
    {0x1C,0x36,0x06,0x0f,0x06,0x06,0x0F,0x00}, // f
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x1F}, // g
    {0x07,0x06,0x36,0x6E,0x66,0x66,0x67,0x00}, // h
    {0x0C,0x00,0x0E,0x0C,0x0C,0x0C,0x1E,0x00}, // i
    {0x30,0x00,0x30,0x30,0x30,0x33,0x33,0x1E}, // j
    {0x07,0x06,0x66,0x36,0x1E,0x36,0x67,0x00}, // k
    {0x0E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, // l
    {0x00,0x00,0x33,0x7F,0x7F,0x6B,0x63,0x00}, // m
    {0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x00}, // n
    {0x00,0x00,0x1E,0x33,0x33,0x33,0x1E,0x00}, // o
    {0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F}, // p
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78}, // q
    {0x00,0x00,0x3B,0x6E,0x66,0x06,0x0F,0x00}, // r
    {0x00,0x00,0x3E,0x03,0x1E,0x30,0x1F,0x00}, // s
    {0x08,0x0C,0x3E,0x0C,0x0C,0x2C,0x18,0x00}, // t
    {0x00,0x00,0x33,0x33,0x33,0x33,0x6E,0x00}, // u
    {0x00,0x00,0x33,0x33,0x33,0x1E,0x0C,0x00}, // v
    {0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00}, // w
    {0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00}, // x
    {0x00,0x00,0x33,0x33,0x33,0x3E,0x30,0x1F}, // y
    {0x00,0x00,0x3F,0x19,0x0C,0x26,0x3F,0x00}, // z
    {0x38,0x0C,0x0C,0x07,0x0C,0x0C,0x38,0x00}, // {
    {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00}, // |
    {0x07,0x0C,0x0C,0x38,0x0C,0x0C,0x07,0x00}, // }
    {0x6E,0x3B,0x00,0x00,0x00,0x00,0x00,0x00}, // ~
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}, // DEL
};

// -------------------------------------------------------
//  Stan globalny
// -------------------------------------------------------
static int   g_win_w = 1024, g_win_h = 576;
static GLuint tri_program, tri_vao, tri_vbo;
static float  tri_angle = 0.0f;
static GLuint txt_program, txt_vao, txt_vbo;
static GLuint font_tex;

// -------------------------------------------------------
//  Kompilacja shaderow
// -------------------------------------------------------
static GLuint compile_shader(GLenum type, const char *src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[512];
        glGetShaderInfoLog(s, 512, NULL, buf);
        MessageBoxA(NULL, buf, "Shader compile error", MB_OK | MB_ICONERROR);
    }
    return s;
}

static GLuint link_program(const char *vs, const char *fs) {
    GLuint p = glCreateProgram();
    glAttachShader(p, compile_shader(GL_VERTEX_SHADER,   vs));
    glAttachShader(p, compile_shader(GL_FRAGMENT_SHADER, fs));
    glLinkProgram(p);
    return p;
}

// -------------------------------------------------------
//  Inicjalizacja trojkata
// -------------------------------------------------------
static void init_triangle(void) {
    float verts[] = {
        0.0f,  0.7f,  1.0f, 0.0f, 0.0f,
       -0.6f, -0.4f,  0.0f, 1.0f, 0.0f,
        0.6f, -0.4f,  0.0f, 0.4f, 1.0f,
    };
    glGenVertexArrays(1, &tri_vao);
    glBindVertexArray(tri_vao);
    glGenBuffers(1, &tri_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, tri_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, 0x1406, 0, 5*4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, 0x1406, 0, 5*4, (void*)(2*4));
    glBindVertexArray(0);
    tri_program = link_program(tri_vert_src, tri_frag_src);
}

// -------------------------------------------------------
//  Inicjalizacja czcionki i VAO tekstu
// -------------------------------------------------------
static void init_text(void) {
    // Atlas czcionki -> tekstura RGBA
    static unsigned char pixels[FONT_TEX_H][FONT_TEX_W];
    memset(pixels, 0, sizeof(pixels));
    for (int ch = 0; ch < 96; ch++) {
        int col = ch % FONT_COLS, row = ch / FONT_COLS;
        for (int y = 0; y < GLYPH_H; y++) {
            unsigned char bits = font8x8[ch][y];
            for (int x = 0; x < GLYPH_W; x++)
                if (bits & (0x80 >> x))
                    pixels[row*GLYPH_H+y][col*GLYPH_W+x] = 255;
        }
    }
    static unsigned char rgba[FONT_TEX_H * FONT_TEX_W * 4];
    for (int i = 0; i < FONT_TEX_H * FONT_TEX_W; i++) {
        unsigned char v = pixels[0][i];
        rgba[i*4+0] = 255; rgba[i*4+1] = 255;
        rgba[i*4+2] = 255; rgba[i*4+3] = v;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FONT_TEX_W, FONT_TEX_H, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    txt_program = link_program(txt_vert_src, txt_frag_src);
    glGenVertexArrays(1, &txt_vao);
    glBindVertexArray(txt_vao);
    glGenBuffers(1, &txt_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, txt_vbo);
    glBufferData(GL_ARRAY_BUFFER, 6*4*sizeof(float)*256, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, 0x1406, 0, 4*4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, 0x1406, 0, 4*4, (void*)(2*4));
    glBindVertexArray(0);
}

// -------------------------------------------------------
//  Rysowanie jednej linii tekstu
// -------------------------------------------------------
#define GLYPH_SCALE 2

static void draw_text(const char *str, float x, float y, float r, float g, float b)
{
    static float buf[6*4*256];
    int n = 0;
    float cx = x;
    for (int i = 0; str[i] && i < 256; i++) {
        int ch = (unsigned char)str[i] - 32;
        if (ch < 0 || ch >= 96) { cx += GLYPH_W*GLYPH_SCALE; continue; }
        int col = ch % FONT_COLS, row = ch / FONT_COLS;
        float u0 = (float)(col*GLYPH_W) / FONT_TEX_W;
        float v0 = (float)(row*GLYPH_H) / FONT_TEX_H;
        float u1 = u0 + (float)GLYPH_W / FONT_TEX_W;
        float v1 = v0 + (float)GLYPH_H / FONT_TEX_H;
        float x0 = cx, y0 = y;
        float x1 = cx + GLYPH_W*GLYPH_SCALE, y1 = y + GLYPH_H*GLYPH_SCALE;
        float quad[6][4] = {
            {x0,y0,u0,v0},{x1,y0,u1,v0},{x1,y1,u1,v1},
            {x0,y0,u0,v0},{x1,y1,u1,v1},{x0,y1,u0,v1},
        };
        memcpy(buf + n*4, quad, sizeof(quad));
        n += 6;
        cx += GLYPH_W * GLYPH_SCALE;
    }
    glBindBuffer(GL_ARRAY_BUFFER, txt_vbo);
    glBufferSubData_fn(GL_ARRAY_BUFFER, 0, n*4*sizeof(float), buf);
    glUseProgram(txt_program);
    glUniform2f(glGetUniformLocation(txt_program, "uScreen"), (float)g_win_w, (float)g_win_h);
    glUniform1i(glGetUniformLocation(txt_program, "uFont"), 0);
    glUniform3f(glGetUniformLocation(txt_program, "uColor"), r, g, b);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glBindVertexArray(txt_vao);
    glDrawArrays(GL_TRIANGLES, 0, n);
    glBindVertexArray(0);
}

// -------------------------------------------------------
//  Rendering
// -------------------------------------------------------
static void render(void)
{
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Trojkat
    glUseProgram(tri_program);
    glUniform1f(glGetUniformLocation(tri_program, "uAngle"), tri_angle);
    glBindVertexArray(tri_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    tri_angle += 0.01f;

    // Log tekstowy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int lh = GLYPH_H * GLYPH_SCALE + 3;
    for (int i = 0; i < g_log_count; i++) {
        // pierwsza linia zolta (naglowek), reszta zielona
        float cr = (i == 0) ? 1.0f : 0.6f;
        float cg = (i == 0) ? 0.8f : 1.0f;
        float cb = (i == 0) ? 0.2f : 0.6f;
        draw_text(g_log[i], 10, 10 + i * lh, cr, cg, cb);
    }
    glDisable(GL_BLEND);
}

// -------------------------------------------------------
//  WGL / okno
// -------------------------------------------------------
static void fatal_error(const char *msg) {
    MessageBoxA(NULL, msg, "Fatal Error", MB_OK | MB_ICONEXCLAMATION);
    exit(EXIT_FAILURE);
}

static void init_opengl_extensions(void) {
    WNDCLASSA wc = {
        .style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC,
        .lpfnWndProc = DefWindowProcA,
        .hInstance = GetModuleHandle(0),
        .lpszClassName = "DummyWGL",
    };
    RegisterClassA(&wc);
    HWND dw = CreateWindowExA(0,"DummyWGL","",0,0,0,1,1,0,0,wc.hInstance,0);
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
    if (!nf) fatal_error("No suitable pixel format.");
    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(dc, pf, sizeof(pfd), &pfd);
    SetPixelFormat(dc, pf, &pfd);
    int att[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,3, WGL_CONTEXT_MINOR_VERSION_ARB,3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0
    };
    HGLRC ctx = wglCreateContextAttribsARB(dc, 0, att);
    if (!ctx) fatal_error("Failed to create GL context.");
    wglMakeCurrent(dc, ctx);
    return ctx;
}

static LRESULT CALLBACK WndProc(HWND w, UINT m, WPARAM wp, LPARAM lp) {
    if (m == WM_SIZE) {
        g_win_w = LOWORD(lp); g_win_h = HIWORD(lp);
        glViewport(0, 0, g_win_w, g_win_h);
    }
    if (m == WM_CLOSE || m == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProcA(w, m, wp, lp);
}

static HWND create_window(HINSTANCE inst) {
    WNDCLASSA wc = {
        .style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC, .lpfnWndProc=WndProc,
        .hInstance=inst, .hCursor=LoadCursor(0,IDC_ARROW), .lpszClassName="OGLWin",
    };
    RegisterClassA(&wc);
    RECT r = {0,0,1024,576};
    DWORD style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&r, style, 0);
    HWND w = CreateWindowExA(0,"OGLWin","OpenGL 3.3",
        style,CW_USEDEFAULT,CW_USEDEFAULT,r.right-r.left,r.bottom-r.top,0,0,inst,0);
    if (!w) fatal_error("Failed to create window.");
    return w;
}

// -------------------------------------------------------
//  ENTRY POINT
// -------------------------------------------------------
int main(int argc, char *argv[])
{
    HINSTANCE inst = GetModuleHandle(NULL);
    HWND  win  = create_window(inst);
    HDC   dc   = GetDC(win);
    HGLRC glrc = init_opengl(dc);
    load_gl_functions();

    // ===================================================
    //  Tutaj uzywaj con_print zamiast printf
    //  - pojawia sie w konsoli I w oknie OpenGL
    // ===================================================
    con_print("=== OpenGL Info ===");
    con_print("Version:  %s", glGetString(GL_VERSION));
    con_print("Renderer: %s", glGetString(GL_RENDERER));
    con_print("Vendor:   %s", glGetString(GL_VENDOR));
    con_print("---");
    con_print("Trojkat RGB, shader GLSL 3.3");
    con_print("con_print -> konsola + okno GL");

    init_triangle();
    init_text();

    ShowWindow(win, SW_SHOW);
    UpdateWindow(win);

    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            else { TranslateMessage(&msg); DispatchMessageA(&msg); }
        }
        render();
        SwapBuffers(dc);
        Sleep(16);
    }

    printf("Zamknieto.\n");
    return 0;
}