/*
 * vec.h — GLSL-style vector math library
 *
 * Obsługuje vec2, vec3, vec4 z aliasami (x/y/z/w, r/g/b/a, u/v, w/h/d, hsv, hsb).
 * Każda operacja ma bazową funkcję operującą na VECTYPE arr[] oraz makra
 * generujące opakowujące funkcje dla vec2, vec3 i vec4.
 *
 * Konwencja nazewnictwa:
 *   vec_add(n, a, b)   — bazowa funkcja (operuje na tablicach)
 *   vec2_add(a, b)     — makro → funkcja dla vec2
 *   vec3_add(a, b)     — makro → funkcja dla vec3
 *   vec4_add(a, b)     — makro → funkcja dla vec4
 */

#ifndef VEC_H
#define VEC_H

//se ustaw typ jaki chcesz
// funkcje sa na floata i wybombią się na 110% przy intcie
#define VECTYPE int

#include <math.h>
#include <stdbool.h>
#include <string.h>

/* =========================================================================
 * Typy
 * ========================================================================= */

typedef struct {
    union {
        struct { VECTYPE x, y; };
        struct { VECTYPE w, h; };
        struct { VECTYPE u, v; };
        VECTYPE arr[2];
    };
} vec2;

typedef struct {
    union {
        struct { VECTYPE x, y, z; };
        struct { VECTYPE w, h, d; };
        struct { VECTYPE r, g, b; };
        VECTYPE arr[3];
        /* Uwaga: hsv i hsb celowo pominięte w vec3 — kolidują z r,g,b.
           Użyj vec3_hsv2rgb / vec3_rgb2hsv zamiast aliasów. */
    };
} vec3;

typedef struct {
    union {
        struct { VECTYPE x, y, z, w; };
        struct { VECTYPE r, g, b, a; };
        VECTYPE arr[4];
    };
} vec4;

/* =========================================================================
 * Stałe / pomocnicze
 * ========================================================================= */

#ifndef M_PI
#  define M_PI 3.14159265358979323846f
#endif

#define VEC_PI   ((VECTYPE)M_PI)
#define VEC_TAU  (2.0f * VEC_PI)
#define VEC_EPS  1e-6f

/* =========================================================================
 * Makra generujące opakowujące funkcje (rdzeń biblioteki)
 *
 * DECL_VEC_FN1(ret, fn, T)        — funkcja 1-argumentowa
 * DECL_VEC_FN2(ret, fn, T)        — funkcja 2-argumentowa
 * DECL_VEC_FN2S(ret, fn, T)       — funkcja (vec, scalar)
 * DECL_VEC_FN3(ret, fn, T)        — funkcja 3-argumentowa
 * DECL_VEC_FN3S(ret, fn, T)       — funkcja (vec, scalar, scalar)
 * DECL_VEC_FN_SCALAR(ret, fn, T)  — funkcja zwracająca VECTYPE (redukcja)
 * ========================================================================= */

#define DECL_VEC_FN1(ret, fn, T)                                     \
    static inline ret T##_##fn(T a) {                                \
        return (ret){ .arr = { 0 } };                                \
    }

/* Makra generujące specjalizacje dla vec2/vec3/vec4 */
#define GEN_FN1(fn)                                                  \
    static inline vec2 vec2_##fn(vec2 a) {                          \
        vec2 r; vec_##fn(2, a.arr, r.arr); return r; }              \
    static inline vec3 vec3_##fn(vec3 a) {                          \
        vec3 r; vec_##fn(3, a.arr, r.arr); return r; }              \
    static inline vec4 vec4_##fn(vec4 a) {                          \
        vec4 r; vec_##fn(4, a.arr, r.arr); return r; }

#define GEN_FN2(fn)                                                  \
    static inline vec2 vec2_##fn(vec2 a, vec2 b) {                  \
        vec2 r; vec_##fn(2, a.arr, b.arr, r.arr); return r; }       \
    static inline vec3 vec3_##fn(vec3 a, vec3 b) {                  \
        vec3 r; vec_##fn(3, a.arr, b.arr, r.arr); return r; }       \
    static inline vec4 vec4_##fn(vec4 a, vec4 b) {                  \
        vec4 r; vec_##fn(4, a.arr, b.arr, r.arr); return r; }

/* (vec, VECTYPE) */
#define GEN_FN2S(fn)                                                 \
    static inline vec2 vec2_##fn(vec2 a, VECTYPE s) {                 \
        vec2 r; vec_##fn(2, a.arr, s, r.arr); return r; }           \
    static inline vec3 vec3_##fn(vec3 a, VECTYPE s) {                 \
        vec3 r; vec_##fn(3, a.arr, s, r.arr); return r; }           \
    static inline vec4 vec4_##fn(vec4 a, VECTYPE s) {                 \
        vec4 r; vec_##fn(4, a.arr, s, r.arr); return r; }

/* (vec, VECTYPE, VECTYPE) */
#define GEN_FN3S(fn)                                                 \
    static inline vec2 vec2_##fn(vec2 a, VECTYPE lo, VECTYPE hi) {      \
        vec2 r; vec_##fn(2, a.arr, lo, hi, r.arr); return r; }      \
    static inline vec3 vec3_##fn(vec3 a, VECTYPE lo, VECTYPE hi) {      \
        vec3 r; vec_##fn(3, a.arr, lo, hi, r.arr); return r; }      \
    static inline vec4 vec4_##fn(vec4 a, VECTYPE lo, VECTYPE hi) {      \
        vec4 r; vec_##fn(4, a.arr, lo, hi, r.arr); return r; }

/* (vec, vec, VECTYPE) */
#define GEN_FN2VS(fn)                                                \
    static inline vec2 vec2_##fn(vec2 a, vec2 b, VECTYPE t) {         \
        vec2 r; vec_##fn(2, a.arr, b.arr, t, r.arr); return r; }    \
    static inline vec3 vec3_##fn(vec3 a, vec3 b, VECTYPE t) {         \
        vec3 r; vec_##fn(3, a.arr, b.arr, t, r.arr); return r; }    \
    static inline vec4 vec4_##fn(vec4 a, vec4 b, VECTYPE t) {         \
        vec4 r; vec_##fn(4, a.arr, b.arr, t, r.arr); return r; }

/* (vec, vec, vec) */
#define GEN_FN3(fn)                                                  \
    static inline vec2 vec2_##fn(vec2 a, vec2 b, vec2 c) {          \
        vec2 r; vec_##fn(2, a.arr, b.arr, c.arr, r.arr); return r; }\
    static inline vec3 vec3_##fn(vec3 a, vec3 b, vec3 c) {          \
        vec3 r; vec_##fn(3, a.arr, b.arr, c.arr, r.arr); return r; }\
    static inline vec4 vec4_##fn(vec4 a, vec4 b, vec4 c) {          \
        vec4 r; vec_##fn(4, a.arr, b.arr, c.arr, r.arr); return r; }

/* Redukcja → VECTYPE */
#define GEN_REDUCE(fn)                                               \
    static inline VECTYPE vec2_##fn(vec2 a) {                         \
        return vec_##fn(2, a.arr); }                                 \
    static inline VECTYPE vec3_##fn(vec3 a) {                         \
        return vec_##fn(3, a.arr); }                                 \
    static inline VECTYPE vec4_##fn(vec4 a) {                         \
        return vec_##fn(4, a.arr); }

/* Redukcja (vec, vec) → VECTYPE */
#define GEN_REDUCE2(fn)                                              \
    static inline VECTYPE vec2_##fn(vec2 a, vec2 b) {                 \
        return vec_##fn(2, a.arr, b.arr); }                         \
    static inline VECTYPE vec3_##fn(vec3 a, vec3 b) {                 \
        return vec_##fn(3, a.arr, b.arr); }                         \
    static inline VECTYPE vec4_##fn(vec4 a, vec4 b) {                 \
        return vec_##fn(4, a.arr, b.arr); }

/* Predykat → bool */
#define GEN_PRED2(fn)                                                \
    static inline bool vec2_##fn(vec2 a, vec2 b) {                  \
        return vec_##fn(2, a.arr, b.arr); }                         \
    static inline bool vec3_##fn(vec3 a, vec3 b) {                  \
        return vec_##fn(3, a.arr, b.arr); }                         \
    static inline bool vec4_##fn(vec4 a, vec4 b) {                  \
        return vec_##fn(4, a.arr, b.arr); }

/* =========================================================================
 * Pomocnicze funkcje skalarne (używane w bazowych funkcjach wektorowych)
 * ========================================================================= */

static inline VECTYPE f_clamp(VECTYPE x, VECTYPE lo, VECTYPE hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}
static inline VECTYPE f_min(VECTYPE a, VECTYPE b)  { return a < b ? a : b; }
static inline VECTYPE f_max(VECTYPE a, VECTYPE b)  { return a > b ? a : b; }
static inline VECTYPE f_abs(VECTYPE a)           { return a < 0.0f ? -a : a; }
static inline VECTYPE f_sign(VECTYPE a)          { return a < 0.0f ? -1.0f : (a > 0.0f ? 1.0f : 0.0f); }
static inline VECTYPE f_frac(VECTYPE a)          { return a - floorf(a); }
static inline VECTYPE f_mix(VECTYPE a, VECTYPE b, VECTYPE t) { return a + (b - a) * t; }
static inline VECTYPE f_step(VECTYPE edge, VECTYPE x)      { return x < edge ? 0.0f : 1.0f; }
static inline VECTYPE f_smoothstep(VECTYPE e0, VECTYPE e1, VECTYPE x) {
    VECTYPE t = f_clamp((x - e0) / (e1 - e0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}
static inline VECTYPE f_deg2rad(VECTYPE d) { return d * (VEC_PI / 180.0f); }
static inline VECTYPE f_rad2deg(VECTYPE r) { return r * (180.0f / VEC_PI); }

/* =========================================================================
 * === BAZOWE FUNKCJE WEKTOROWE ===
 * Wszystkie operują na (int n, const VECTYPE* src, VECTYPE* dst) lub podobnych
 * sygnaturach. To są prawdziwe implementacje; makra GEN_* tylko je owijają.
 * ========================================================================= */

/* ---- Arytmetyka --------------------------------------------------------- */

static inline void vec_add(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] + b[i];
}
static inline void vec_sub(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] - b[i];
}
static inline void vec_mul(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] * b[i];
}
static inline void vec_div(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] / b[i];
}
static inline void vec_neg(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = -a[i];
}
static inline void vec_scale(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] * s;
}
static inline void vec_divs(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    VECTYPE inv = 1.0f / s;
    for (int i = 0; i < n; i++) r[i] = a[i] * inv;
}
static inline void vec_adds(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] + s;
}
static inline void vec_subs(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = a[i] - s;
}
static inline void vec_mod(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = fmodf(a[i], b[i]);
}
static inline void vec_mods(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = fmodf(a[i], s);
}

/* ---- Redukcje (wynik: VECTYPE) -------------------------------------------- */

static inline VECTYPE vec_dot(int n, const VECTYPE* a, const VECTYPE* b) {
    VECTYPE s = 0.0f;
    for (int i = 0; i < n; i++) s += a[i] * b[i];
    return s;
}
static inline VECTYPE vec_length(int n, const VECTYPE* a) {
    return sqrtf(vec_dot(n, a, a));
}
static inline VECTYPE vec_length2(int n, const VECTYPE* a) {   /* length squared */
    return vec_dot(n, a, a);
}
static inline VECTYPE vec_distance(int n, const VECTYPE* a, const VECTYPE* b) {
    VECTYPE tmp[4] = {0};
    vec_sub(n, a, b, tmp);
    return vec_length(n, tmp);
}
static inline VECTYPE vec_distance2(int n, const VECTYPE* a, const VECTYPE* b) {
    VECTYPE tmp[4] = {0};
    vec_sub(n, a, b, tmp);
    return vec_length2(n, tmp);
}
static inline VECTYPE vec_sum(int n, const VECTYPE* a) {
    VECTYPE s = 0.0f;
    for (int i = 0; i < n; i++) s += a[i];
    return s;
}
static inline VECTYPE vec_product(int n, const VECTYPE* a) {
    VECTYPE s = 1.0f;
    for (int i = 0; i < n; i++) s *= a[i];
    return s;
}
static inline VECTYPE vec_compmin(int n, const VECTYPE* a) {
    VECTYPE m = a[0];
    for (int i = 1; i < n; i++) if (a[i] < m) m = a[i];
    return m;
}
static inline VECTYPE vec_compmax(int n, const VECTYPE* a) {
    VECTYPE m = a[0];
    for (int i = 1; i < n; i++) if (a[i] > m) m = a[i];
    return m;
}

/* ---- Normalizacja -------------------------------------------------------- */

static inline void vec_normalize(int n, const VECTYPE* a, VECTYPE* r) {
    VECTYPE len = vec_length(n, a);
    if (len < VEC_EPS) { memcpy(r, a, n * sizeof(VECTYPE)); return; }
    vec_divs(n, a, len, r);
}
static inline void vec_normalize_safe(int n, const VECTYPE* a, const VECTYPE* fallback, VECTYPE* r) {
    VECTYPE len = vec_length(n, a);
    if (len < VEC_EPS) { memcpy(r, fallback, n * sizeof(VECTYPE)); return; }
    vec_divs(n, a, len, r);
}

/* ---- Operacje komponentowe (GLSL built-ins) ----------------------------- */

static inline void vec_abs(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = fabsf(a[i]);
}
static inline void vec_sign(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_sign(a[i]);
}
static inline void vec_floor(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = floorf(a[i]);
}
static inline void vec_ceil(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = ceilf(a[i]);
}
static inline void vec_round(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = roundf(a[i]);
}
static inline void vec_trunc(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = truncf(a[i]);
}
static inline void vec_frac(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_frac(a[i]);
}
static inline void vec_sqrt(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = sqrtf(a[i]);
}
static inline void vec_rsqrt(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = 1.0f / sqrtf(a[i]);
}
static inline void vec_exp(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = expf(a[i]);
}
static inline void vec_exp2(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = exp2f(a[i]);
}
static inline void vec_log(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = logf(a[i]);
}
static inline void vec_log2(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = log2f(a[i]);
}
static inline void vec_pow(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = powf(a[i], b[i]);
}
static inline void vec_pows(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = powf(a[i], s);
}

/* ---- Trygonometria ------------------------------------------------------- */

static inline void vec_sin(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = sinf(a[i]);
}
static inline void vec_cos(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = cosf(a[i]);
}
static inline void vec_tan(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = tanf(a[i]);
}
static inline void vec_asin(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = asinf(a[i]);
}
static inline void vec_acos(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = acosf(a[i]);
}
static inline void vec_atan(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = atanf(a[i]);
}
static inline void vec_atan2(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = atan2f(a[i], b[i]);
}
static inline void vec_sinh(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = sinhf(a[i]);
}
static inline void vec_cosh(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = coshf(a[i]);
}
static inline void vec_tanh(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = tanhf(a[i]);
}
static inline void vec_deg2rad(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_deg2rad(a[i]);
}
static inline void vec_rad2deg(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_rad2deg(a[i]);
}

/* ---- Min / Max / Clamp -------------------------------------------------- */

static inline void vec_min(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_min(a[i], b[i]);
}
static inline void vec_max(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_max(a[i], b[i]);
}
static inline void vec_mins(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_min(a[i], s);
}
static inline void vec_maxs(int n, const VECTYPE* a, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_max(a[i], s);
}
static inline void vec_clamp(int n, const VECTYPE* a, VECTYPE lo, VECTYPE hi, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_clamp(a[i], lo, hi);
}
static inline void vec_clamp01(int n, const VECTYPE* a, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_clamp(a[i], 0.0f, 1.0f);
}
static inline void vec_clampv(int n, const VECTYPE* a, const VECTYPE* lo, const VECTYPE* hi, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_clamp(a[i], lo[i], hi[i]);
}
static inline void vec_saturate(int n, const VECTYPE* a, VECTYPE* r) {
    vec_clamp01(n, a, r);
}

/* ---- Interpolacja ------------------------------------------------------- */

static inline void vec_mix(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE t, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_mix(a[i], b[i], t);
}
static inline void vec_mixv(int n, const VECTYPE* a, const VECTYPE* b, const VECTYPE* t, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_mix(a[i], b[i], t[i]);
}
static inline void vec_step(int n, const VECTYPE* edge, const VECTYPE* x, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_step(edge[i], x[i]);
}
static inline void vec_steps(int n, VECTYPE edge, const VECTYPE* x, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_step(edge, x[i]);
}
static inline void vec_smoothstep(int n, const VECTYPE* a, VECTYPE e0, VECTYPE e1, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_smoothstep(e0, e1, a[i]);
}
static inline void vec_smoothstepv(int n, const VECTYPE* e0, const VECTYPE* e1,
                                    const VECTYPE* x, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = f_smoothstep(e0[i], e1[i], x[i]);
}
/* Hermite cubic — GLSL smoothstep alias */
#define vec_hermite vec_smoothstepv

/* Bilinearna interpolacja między 4 wektorami */
static inline void vec_bilinear(int n,
    const VECTYPE* v00, const VECTYPE* v10,
    const VECTYPE* v01, const VECTYPE* v11,
    VECTYPE tx, VECTYPE ty, VECTYPE* r)
{
    for (int i = 0; i < n; i++)
        r[i] = f_mix(f_mix(v00[i], v10[i], tx), f_mix(v01[i], v11[i], tx), ty);
}

/* ---- Odbicie / Załamanie ------------------------------------------------- */

/* reflect(I, N) = I - 2*dot(N,I)*N */
static inline void vec_reflect(int n, const VECTYPE* I, const VECTYPE* N, VECTYPE* r) {
    VECTYPE d = 2.0f * vec_dot(n, N, I);
    for (int i = 0; i < n; i++) r[i] = I[i] - d * N[i];
}

/* refract(I, N, eta) — GLSL */
static inline void vec_refract(int n, const VECTYPE* I, const VECTYPE* N, VECTYPE eta, VECTYPE* r) {
    VECTYPE d   = vec_dot(n, N, I);
    VECTYPE k   = 1.0f - eta * eta * (1.0f - d * d);
    if (k < 0.0f) {
        for (int i = 0; i < n; i++) r[i] = 0.0f;
    } else {
        VECTYPE sq = sqrtf(k);
        for (int i = 0; i < n; i++) r[i] = eta * I[i] - (eta * d + sq) * N[i];
    }
}

/* faceforward(N, I, Nref) = dot(Nref,I) < 0 ? N : -N */
static inline void vec_faceforward(int n, const VECTYPE* N, const VECTYPE* I,
                                    const VECTYPE* Nref, VECTYPE* r) {
    VECTYPE d = vec_dot(n, Nref, I);
    for (int i = 0; i < n; i++) r[i] = d < 0.0f ? N[i] : -N[i];
}

/* ---- Predykaty ---------------------------------------------------------- */

static inline bool vec_equal(int n, const VECTYPE* a, const VECTYPE* b) {
    for (int i = 0; i < n; i++) if (a[i] != b[i]) return false;
    return true;
}
static inline bool vec_equal_eps(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE eps) {
    for (int i = 0; i < n; i++) if (fabsf(a[i] - b[i]) > eps) return false;
    return true;
}
static inline bool vec_less(int n, const VECTYPE* a, const VECTYPE* b) {
    for (int i = 0; i < n; i++) if (a[i] >= b[i]) return false;
    return true;
}
static inline bool vec_lesseq(int n, const VECTYPE* a, const VECTYPE* b) {
    for (int i = 0; i < n; i++) if (a[i] > b[i]) return false;
    return true;
}
static inline bool vec_greater(int n, const VECTYPE* a, const VECTYPE* b) {
    for (int i = 0; i < n; i++) if (a[i] <= b[i]) return false;
    return true;
}
static inline bool vec_greatereq(int n, const VECTYPE* a, const VECTYPE* b) {
    for (int i = 0; i < n; i++) if (a[i] < b[i]) return false;
    return true;
}
static inline bool vec_isnan(int n, const VECTYPE* a) {
    for (int i = 0; i < n; i++) if (isnan(a[i])) return true;
    return false;
}
static inline bool vec_isinf(int n, const VECTYPE* a) {
    for (int i = 0; i < n; i++) if (isinf(a[i])) return true;
    return false;
}
static inline bool vec_isfinite(int n, const VECTYPE* a) {
    for (int i = 0; i < n; i++) if (!isfinite(a[i])) return false;
    return true;
}

/* ---- Różne -------------------------------------------------------------- */

static inline void vec_madd(int n, const VECTYPE* a, const VECTYPE* b, const VECTYPE* c, VECTYPE* r) {
    /* r = a * b + c  (fused multiply-add, komponentowe) */
    for (int i = 0; i < n; i++) r[i] = a[i] * b[i] + c[i];
}
static inline void vec_lerp(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE t, VECTYPE* r) {
    vec_mix(n, a, b, t, r);
}
static inline void vec_nlerp(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE t, VECTYPE* r) {
    vec_mix(n, a, b, t, r);
    vec_normalize(n, r, r);
}
static inline void vec_project(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    /* Rzut a na b */
    VECTYPE s = vec_dot(n, a, b) / (vec_dot(n, b, b) + VEC_EPS);
    vec_scale(n, b, s, r);
}
static inline void vec_reject(int n, const VECTYPE* a, const VECTYPE* b, VECTYPE* r) {
    /* Składowa a prostopadła do b */
    VECTYPE proj[4] = {0};
    vec_project(n, a, b, proj);
    vec_sub(n, a, proj, r);
}
static inline void vec_copy(int n, const VECTYPE* src, VECTYPE* dst) {
    memcpy(dst, src, n * sizeof(VECTYPE));
}
static inline void vec_set(int n, VECTYPE s, VECTYPE* r) {
    for (int i = 0; i < n; i++) r[i] = s;
}
static inline void vec_zero(int n, VECTYPE* r) {
    memset(r, 0, n * sizeof(VECTYPE));
}
static inline void vec_one(int n, VECTYPE* r) {
    vec_set(n, 1.0f, r);
}
static inline void vec_swapcomp(VECTYPE* a, VECTYPE* b, int ia, int ib) {
    VECTYPE t = a[ia]; a[ia] = b[ib]; b[ib] = t;
}

/* =========================================================================
 * Specyficzne dla wymiaru (nie uogólniają się przez n)
 * ========================================================================= */

/* ---- cross product (tylko vec3) ---------------------------------------- */
static inline vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    }};
}

/* ---- perpendicular (vec2 — obrót o 90°) --------------------------------- */
static inline vec2 vec2_perp(vec2 a)  { return (vec2){{ -a.y, a.x }}; }
static inline vec2 vec2_perp_cw(vec2 a) { return (vec2){{ a.y, -a.x }}; }

/* ---- Obrót vec2 --------------------------------------------------------- */
static inline vec2 vec2_rotate(vec2 a, VECTYPE angle) {
    VECTYPE c = cosf(angle), s = sinf(angle);
    return (vec2){{ a.x * c - a.y * s, a.x * s + a.y * c }};
}
/* Kąt między wektorem a osią X */
static inline VECTYPE vec2_angle(vec2 a) { return atan2f(a.y, a.x); }
/* Kąt między dwoma wektorami */
static inline VECTYPE vec2_angle_between(vec2 a, vec2 b) {
    return atan2f(a.x * b.y - a.y * b.x, a.x * b.x + a.y * b.y);
}

/* ---- slerp (tylko vec3, vec4 — sferyczna interpolacja) ----------------- */
static inline vec3 vec3_slerp(vec3 a, vec3 b, VECTYPE t) {
    VECTYPE d = f_clamp(vec_dot(3, a.arr, b.arr), -1.0f, 1.0f);
    VECTYPE angle = acosf(d) * t;
    vec3 rel;
    for (int i = 0; i < 3; i++) rel.arr[i] = b.arr[i] - a.arr[i] * d;
    vec_normalize(3, rel.arr, rel.arr);
    VECTYPE ca = cosf(angle), sa = sinf(angle);
    vec3 r;
    for (int i = 0; i < 3; i++) r.arr[i] = a.arr[i] * ca + rel.arr[i] * sa;
    return r;
}

/* ---- Homogeniczne operacje (vec4 ↔ vec3) -------------------------------- */
static inline vec4 vec3_to_vec4_point(vec3 v)  { return (vec4){{ v.x, v.y, v.z, 1.0f }}; }
static inline vec4 vec3_to_vec4_dir(vec3 v)    { return (vec4){{ v.x, v.y, v.z, 0.0f }}; }
static inline vec3 vec4_to_vec3(vec4 v)         { return (vec3){{ v.x, v.y, v.z }}; }
static inline vec3 vec4_to_vec3_hdiv(vec4 v)    {
    VECTYPE inv = (fabsf(v.w) > VEC_EPS) ? 1.0f / v.w : 1.0f;
    return (vec3){{ v.x * inv, v.y * inv, v.z * inv }};
}
static inline vec2 vec3_to_vec2(vec3 v) { return (vec2){{ v.x, v.y }}; }
static inline vec3 vec2_to_vec3(vec2 v, VECTYPE z) { return (vec3){{ v.x, v.y, z }}; }

/* =========================================================================
 * === KOLORY ===
 * ========================================================================= */

/* ---- Konstruktory kolorów ----------------------------------------------- */
typedef vec4 RGBA;
#define RGBA_NULL (RGBA){.r=-1,.g=-1,.b=-1,.a=-1}

static inline vec4 color_rgba(VECTYPE r, VECTYPE g, VECTYPE b, VECTYPE a) {
    return (vec4){{ r, g, b, a }};
}
static inline vec4 color_rgb(VECTYPE r, VECTYPE g, VECTYPE b) {
    return (vec4){{ r, g, b, 1.0f }};
}
static inline vec4 color_gray(VECTYPE v) {
    return (vec4){{ v, v, v, 1.0f }};
}
static inline vec4 color_graya(VECTYPE v, VECTYPE a) {
    return (vec4){{ v, v, v, a }};
}
/* Hex RRGGBB (0xRRGGBB) */
static inline vec4 color_hex(unsigned int hex) {
    return (vec4){{
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >>  8) & 0xFF) / 255.0f,
        ( hex        & 0xFF) / 255.0f,
        1.0f
    }};
}
/* Hex RRGGBBAA */
static inline vec4 color_hexa(unsigned int hex) {
    return (vec4){{
        ((hex >> 24) & 0xFF) / 255.0f,
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >>  8) & 0xFF) / 255.0f,
        ( hex        & 0xFF) / 255.0f
    }};
}
/* uint8 r,g,b,a */
static inline vec4 color_u8(unsigned char r, unsigned char g,
                              unsigned char b, unsigned char a) {
    return (vec4){{ r/255.0f, g/255.0f, b/255.0f, a/255.0f }};
}

/* ---- Konwersje przestrzeni barw ----------------------------------------- */

/* RGB → HSV */
static inline vec3 vec3_rgb2hsv(vec3 c) {
    VECTYPE r = c.r, g = c.g, b = c.b;
    VECTYPE mx = f_max(r, f_max(g, b));
    VECTYPE mn = f_min(r, f_min(g, b));
    VECTYPE delta = mx - mn;
    VECTYPE h = 0.0f, s = 0.0f, v = mx;
    if (delta > VEC_EPS) {
        s = delta / mx;
        if      (mx == r) h = (g - b) / delta + (g < b ? 6.0f : 0.0f);
        else if (mx == g) h = (b - r) / delta + 2.0f;
        else              h = (r - g) / delta + 4.0f;
        h /= 6.0f;
    }
    return (vec3){{ h, s, v }};
}

/* HSV → RGB */
static inline vec3 vec3_hsv2rgb(vec3 c) {
    VECTYPE h = c.x, s = c.y, v = c.z;
    if (s < VEC_EPS) return (vec3){{ v, v, v }};
    VECTYPE f = h * 6.0f;
    int   i = (int)f % 6;
    VECTYPE frac = f - floorf(f);
    VECTYPE p = v * (1.0f - s);
    VECTYPE q = v * (1.0f - s * frac);
    VECTYPE t = v * (1.0f - s * (1.0f - frac));
    switch (i) {
        case 0: return (vec3){{ v, t, p }};
        case 1: return (vec3){{ q, v, p }};
        case 2: return (vec3){{ p, v, t }};
        case 3: return (vec3){{ p, q, v }};
        case 4: return (vec3){{ t, p, v }};
        default:return (vec3){{ v, p, q }};
    }
}

/* RGB → HSL */
static inline vec3 vec3_rgb2hsl(vec3 c) {
    VECTYPE mx = f_max(c.r, f_max(c.g, c.b));
    VECTYPE mn = f_min(c.r, f_min(c.g, c.b));
    VECTYPE l = (mx + mn) * 0.5f;
    VECTYPE h = 0.0f, s = 0.0f;
    VECTYPE delta = mx - mn;
    if (delta > VEC_EPS) {
        s = delta / (1.0f - fabsf(2.0f * l - 1.0f));
        if      (mx == c.r) h = (c.g - c.b) / delta + (c.g < c.b ? 6.0f : 0.0f);
        else if (mx == c.g) h = (c.b - c.r) / delta + 2.0f;
        else                h = (c.r - c.g) / delta + 4.0f;
        h /= 6.0f;
    }
    return (vec3){{ h, s, l }};
}

/* HSL → RGB */
static inline vec3 vec3_hsl2rgb(vec3 c) {
    VECTYPE h = c.x, s = c.y, l = c.z;
    if (s < VEC_EPS) return (vec3){{ l, l, l }};
    VECTYPE q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
    VECTYPE p = 2.0f * l - q;
    #define _HUE(t) ({ \
        VECTYPE _t = t; \
        _t = _t < 0 ? _t + 1.0f : (_t > 1 ? _t - 1.0f : _t); \
        _t < 1.0f/6 ? p + (q-p)*6*_t : \
        _t < 1.0f/2 ? q : \
        _t < 2.0f/3 ? p + (q-p)*(2.0f/3-_t)*6 : p; })
    vec3 r = {{ _HUE(h + 1.0f/3), _HUE(h), _HUE(h - 1.0f/3) }};
    #undef _HUE
    return r;
}

/* RGB → YCbCr (BT.601) */
static inline vec3 vec3_rgb2ycbcr(vec3 c) {
    return (vec3){{
         0.2990f * c.r + 0.5870f * c.g + 0.1140f * c.b,
        -0.1687f * c.r - 0.3313f * c.g + 0.5000f * c.b + 0.5f,
         0.5000f * c.r - 0.4187f * c.g - 0.0813f * c.b + 0.5f
    }};
}

/* YCbCr → RGB */
static inline vec3 vec3_ycbcr2rgb(vec3 c) {
    VECTYPE cb = c.y - 0.5f, cr = c.z - 0.5f;
    return (vec3){{
        f_clamp(c.x                  + 1.4020f * cr, 0.0f, 1.0f),
        f_clamp(c.x - 0.3441f * cb  - 0.7141f * cr, 0.0f, 1.0f),
        f_clamp(c.x + 1.7720f * cb,                  0.0f, 1.0f)
    }};
}

/* RGB → liniowe (gamma 2.2 sRGB) */
static inline vec3 vec3_srgb2linear(vec3 c) {
    vec3 r;
    for (int i = 0; i < 3; i++)
        r.arr[i] = c.arr[i] <= 0.04045f
            ? c.arr[i] / 12.92f
            : powf((c.arr[i] + 0.055f) / 1.055f, 2.4f);
    return r;
}

/* Liniowe → sRGB */
static inline vec3 vec3_linear2srgb(vec3 c) {
    vec3 r;
    for (int i = 0; i < 3; i++)
        r.arr[i] = c.arr[i] <= 0.0031308f
            ? 12.92f * c.arr[i]
            : 1.055f * powf(c.arr[i], 1.0f / 2.4f) - 0.055f;
    return r;
}

/* ---- Operacje na kolorach ----------------------------------------------- */

/* Luminancja (BT.709) */
static inline VECTYPE color_luminance(vec4 c) {
    return 0.2126f * c.r + 0.7152f * c.g + 0.0722f * c.b;
}
static inline VECTYPE color_luminance3(vec3 c) {
    return 0.2126f * c.r + 0.7152f * c.g + 0.0722f * c.b;
}

/* Mieszanie alfa (premultiplied alpha over) */
static inline vec4 color_alpha_over(vec4 src, vec4 dst) {
    VECTYPE a = src.a + dst.a * (1.0f - src.a);
    if (a < VEC_EPS) return (vec4){{ 0, 0, 0, 0 }};
    return (vec4){{
        (src.r * src.a + dst.r * dst.a * (1.0f - src.a)) / a,
        (src.g * src.a + dst.g * dst.a * (1.0f - src.a)) / a,
        (src.b * src.a + dst.b * dst.a * (1.0f - src.a)) / a,
        a
    }};
}

/* Mieszanie addytywne */
static inline vec4 color_add(vec4 a, vec4 b) {
    return (vec4){{
        f_min(a.r + b.r, 1.0f),
        f_min(a.g + b.g, 1.0f),
        f_min(a.b + b.b, 1.0f),
        f_min(a.a + b.a, 1.0f)
    }};
}

/* Multiply blend */
static inline vec4 color_multiply(vec4 a, vec4 b) {
    return (vec4){{ a.r*b.r, a.g*b.g, a.b*b.b, a.a*b.a }};
}

/* Screen blend */
static inline vec4 color_screen(vec4 a, vec4 b) {
    return (vec4){{
        1.0f - (1.0f - a.r) * (1.0f - b.r),
        1.0f - (1.0f - a.g) * (1.0f - b.g),
        1.0f - (1.0f - a.b) * (1.0f - b.b),
        1.0f - (1.0f - a.a) * (1.0f - b.a)
    }};
}

/* Overlay blend */
static inline vec4 color_overlay(vec4 a, vec4 b) {
    vec4 r;
    for (int i = 0; i < 4; i++)
        r.arr[i] = a.arr[i] < 0.5f
            ? 2.0f * a.arr[i] * b.arr[i]
            : 1.0f - 2.0f * (1.0f - a.arr[i]) * (1.0f - b.arr[i]);
    return r;
}

/* Lerp kolorów (z interpolacją alpha) */
static inline vec4 color_mix(vec4 a, vec4 b, VECTYPE t) {
    vec4 r;
    vec_mix(4, a.arr, b.arr, t, r.arr);
    return r;
}

/* Odcień szarości */
static inline vec4 color_to_grayscale(vec4 c) {
    VECTYPE l = color_luminance(c);
    return (vec4){{ l, l, l, c.a }};
}

/* Inwersja RGB (nie alpha) */
static inline vec4 color_invert(vec4 c) {
    return (vec4){{ 1.0f - c.r, 1.0f - c.g, 1.0f - c.b, c.a }};
}

/* Kontrast */
static inline vec4 color_contrast(vec4 c, VECTYPE contrast) {
    vec4 r;
    for (int i = 0; i < 3; i++)
        r.arr[i] = f_clamp(0.5f + (c.arr[i] - 0.5f) * contrast, 0.0f, 1.0f);
    r.a = c.a;
    return r;
}

/* Jasność */
static inline vec4 color_brightness(vec4 c, VECTYPE brightness) {
    vec4 r;
    for (int i = 0; i < 3; i++)
        r.arr[i] = f_clamp(c.arr[i] + brightness, 0.0f, 1.0f);
    r.a = c.a;
    return r;
}

/* Nasycenie */
static inline vec4 color_saturation(vec4 c, VECTYPE saturation) {
    VECTYPE l = color_luminance(c);
    vec4 r;
    for (int i = 0; i < 3; i++)
        r.arr[i] = f_clamp(l + (c.arr[i] - l) * saturation, 0.0f, 1.0f);
    r.a = c.a;
    return r;
}

/* Gamma */
static inline vec4 color_gamma(vec4 c, VECTYPE gamma) {
    VECTYPE inv = 1.0f / gamma;
    return (vec4){{
        powf(f_max(c.r, 0.0f), inv),
        powf(f_max(c.g, 0.0f), inv),
        powf(f_max(c.b, 0.0f), inv),
        c.a
    }};
}

/* Premultiply alpha */
static inline vec4 color_premultiply(vec4 c) {
    return (vec4){{ c.r * c.a, c.g * c.a, c.b * c.a, c.a }};
}
/* Un-premultiply */
static inline vec4 color_unpremultiply(vec4 c) {
    if (c.a < VEC_EPS) return (vec4){{ 0, 0, 0, 0 }};
    VECTYPE inv = 1.0f / c.a;
    return (vec4){{ c.r * inv, c.g * inv, c.b * inv, c.a }};
}

/* Eksport do u8 */
static inline void color_to_u8(vec4 c, unsigned char out[4]) {
    for (int i = 0; i < 4; i++)
        out[i] = (unsigned char)(f_clamp(c.arr[i], 0.0f, 1.0f) * 255.0f + 0.5f);
}
/* Eksport do uint32 RGBA */
static inline unsigned int color_to_u32_rgba(vec4 c) {
    unsigned char b[4];
    color_to_u8(c, b);
    return ((unsigned int)b[0] << 24) | ((unsigned int)b[1] << 16)
         | ((unsigned int)b[2] <<  8) |  (unsigned int)b[3];
}

/* =========================================================================
 * Generowanie specjalizowanych funkcji makrami GEN_*
 * ========================================================================= */

/* Arytmetyka */
GEN_FN2(add)   GEN_FN2(sub)   GEN_FN2(mul)   GEN_FN2(div)
GEN_FN1(neg)
GEN_FN2S(scale) GEN_FN2S(divs) GEN_FN2S(adds) GEN_FN2S(subs)
GEN_FN2(mod)   GEN_FN2S(mods)

/* Potęgi / logarytmy */
GEN_FN1(sqrt)  GEN_FN1(rsqrt)
GEN_FN1(exp)   GEN_FN1(exp2)
GEN_FN1(log)   GEN_FN1(log2)
GEN_FN2(pow)   GEN_FN2S(pows)

/* Trygonometria */
GEN_FN1(sin)  GEN_FN1(cos)  GEN_FN1(tan)
GEN_FN1(asin) GEN_FN1(acos) GEN_FN1(atan)
GEN_FN2(atan2)
GEN_FN1(sinh) GEN_FN1(cosh) GEN_FN1(tanh)
GEN_FN1(deg2rad) GEN_FN1(rad2deg)

/* Zaokrąglanie */
GEN_FN1(abs)   GEN_FN1(sign)
GEN_FN1(floor) GEN_FN1(ceil) GEN_FN1(round) GEN_FN1(trunc) GEN_FN1(frac)

/* Min / Max / Clamp */
GEN_FN2(min)    GEN_FN2(max)
GEN_FN2S(mins)  GEN_FN2S(maxs)
GEN_FN3S(clamp) GEN_FN1(clamp01) GEN_FN1(saturate)

/* Interpolacja */
GEN_FN2VS(mix)    GEN_FN2VS(lerp)
GEN_FN3S(smoothstep)

/* Normalizacja */
GEN_FN1(normalize)

/* Redukcje → VECTYPE */
GEN_REDUCE(length)   GEN_REDUCE(length2)
GEN_REDUCE(sum)      GEN_REDUCE(product)
GEN_REDUCE(compmin)  GEN_REDUCE(compmax)
GEN_REDUCE2(dot)     GEN_REDUCE2(distance)  GEN_REDUCE2(distance2)

/* Predykaty */
GEN_PRED2(equal)    GEN_PRED2(less)     GEN_PRED2(lesseq)
GEN_PRED2(greater)  GEN_PRED2(greatereq)

/* =========================================================================
 * Skrótowe makra do tworzenia wektorów (wygodne w użyciu)
 * ========================================================================= */

#define V2(x,y)       ((vec2){{ (x), (y) }})
#define VEC2(x,y)       ((vec2){{ (x), (y) }})
#define Vec2(x,y)       ((vec2){{ (x), (y) }})
#define V3(x,y,z)     ((vec3){{ (x), (y), (z) }})
#define VEC3(x,y,z)     ((vec3){{ (x), (y), (z) }})
#define Vec3(x,y,z)     ((vec3){{ (x), (y), (z) }})
#define V4(x,y,z,w)   ((vec4){{ (x), (y), (z), (w) }})
#define VEC4(x,y,z,w)   ((vec4){{ (x), (y), (z), (w) }})
#define Vec4(x,y,z,w)   ((vec4){{ (x), (y), (z), (w) }})

#define VEC2_ZERO     V2(0,0)
#define VEC2_ONE      V2(1,1)
#define VEC2_X        V2(1,0)
#define VEC2_Y        V2(0,1)
#define VEC2_INF      V2(1e30f, 1e30f)

#define VEC3_ZERO     V3(0,0,0)
#define VEC3_ONE      V3(1,1,1)
#define VEC3_X        V3(1,0,0)
#define VEC3_Y        V3(0,1,0)
#define VEC3_Z        V3(0,0,1)
#define VEC3_UP       V3(0,1,0)
#define VEC3_RIGHT    V3(1,0,0)
#define VEC3_FORWARD  V3(0,0,-1)

#define VEC4_ZERO     V4(0,0,0,0)
#define VEC4_ONE      V4(1,1,1,1)

/* Kolory */
#define COLOR_WHITE   ((vec4){{ 1,1,1,1 }})
#define COLOR_BLACK   ((vec4){{ 0,0,0,1 }})
#define COLOR_RED     ((vec4){{ 1,0,0,1 }})
#define COLOR_GREEN   ((vec4){{ 0,1,0,1 }})
#define COLOR_BLUE    ((vec4){{ 0,0,1,1 }})
#define COLOR_YELLOW  ((vec4){{ 1,1,0,1 }})
#define COLOR_CYAN    ((vec4){{ 0,1,1,1 }})
#define COLOR_MAGENTA ((vec4){{ 1,0,1,1 }})
#define COLOR_TRANSP  ((vec4){{ 0,0,0,0 }})
#define COLOR_GRAY(v) color_gray(v)

#endif /* VEC_H */