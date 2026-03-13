#define mu_SUBJECT_THINGBEINGDONE(...) mux_SUBJECT_THINGBEINGDONE(mux_global_context, &mux_global_context->result, __VA_ARGS__)
#define mu_SUBJECT_THINGSBEINGDONE_(result, ...) mux_SUBJECT_THINGBEINGDONE(mux_global_context, result, __VA_ARGS__)

// ths allows for

// Fully explicit:
mux_SUBJECT_THINGBEINGDONE(&context, &result, ...);
// Explicit result-checking with assumed context:
mu_SUBJECT_THINGBEINGDONE_(&result, ...);
// Assumed context, result goes into context:
mu_SUBJECT_THINGBEINGDONE(...);

/////////////////////////////////////////////////////////////////////

muxContext context;
mux_context_create(&context, MU_TRUE);

// ...mu_whatever(...)...
// context.result

mux_context_destroy(&context);

// ths allows for

muxContext context1, context2;
mux_context_create(&context1, MU_FALSE);
mux_context_create(&context2, MU_FALSE);

// ...mux_whatever(&context1...)...
// ...mux_whatever(&context2...)...

mux_context_destroy(&context1);
mux_context_destroy(&context2);

/////////////////////////////////////////////////////////////////////

Object referencing
Functions that rely on context structuring usually return types that are just macros for the void* type, acting as a reference to dynamically allocated memory to be dereferenced when used by the library, follwing a general syntax of:

muObject object = mu_object_create(...); // 'object' is now a pointer.
...
...mu_object_do_something(...object...)... // The library is now using the pointer to hold information about the object.
...
object = mu_object_destroy(...object...); // The object's data is now wrapped up and destroyed, with the pointer being freed. Now 'object' is 0.
The reason why object needs to be dynamically allocated and freed is because the exact memory that object will take up cannot always be known beforehand. For example, in muCOSA, a library that can handle window management, exactly what a 'window' is defined as can depend on the window system currently running. It could be assumed that only one window system can be defined at a time, but then that makes operating systems that can use multiple window systems (for example, Linux with X11 and Wayland) need to have separately-compiled versions of muCOSA, stored rather as separate library files or as separate executables. A union could be used, but risks taking up more memory than is needed.

/////////////////////////////////////////////////////////////////////

/*
FILENAME - AUTHOR
SHORT DESCRIPTION
No warranty implied; use at your own risk.

// license or some shit

*/

#define MUX_VERSION_MAJOR n
#define MUX_VERSION_MINOR n

/////////////////////////////////////////////////////////////////////

C standard library dependencies
Any C standard library dependencies not defined beforehand are usually defined within the header section, following this general format:

/* C standard library dependencies */
	
	#if !defined(mu_function) || \
		!defined(type_m)      || \
		!defined(MU_MACRO)
		
		#include <relevantfile.h>
		
		#ifndef mu_function
			#define mu_function function
		#endif

		#ifndef type_m
			#define type_m type
		#endif

		#ifndef MU_MACRO
			#define MU_MACRO MACRO
		#endif
	
	#endif
	
	...
	
Note that these C standard library dependencies are overridable if defined beforehand, as this formatting would suggest, and are all listed in the documentation for their respective mu library.

/////////////////////////////////////////////////////////////////////

