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


//functions implemetations here
struct clConsoleSprite{
    char* buffer;
    vec2 pos;
    vec2 dim;
    vec3 color;
};

//TODO funkcja do printowania sprita na inny sprite, i do printowania sprita do konsoli i najpierw printowac na prite BUFFER i z buffera na kosole