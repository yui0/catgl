//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#ifndef CATGL_NANOVG_H_
#define CATGL_NANOVG_H_


#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)

	#ifdef _WIN64
		//define something for Windows (64-bit only)
	#endif

#elif __APPLE__
	#if TARGET_IPHONE_SIMULATOR
		// iOS Simulator
	#elif TARGET_OS_IPHONE
		// iOS device
	#elif TARGET_OS_MAC
		// Other kinds of Mac OS
	#else
		// Unsupported platform
	#endif

#elif __ANDROID__

	#include <GLES2/gl2.h>
	//#include <GLES2/gl2ext.h>

	#define NANOVG_GLES2_IMPLEMENTATION
	#define nvgCreate	nvgCreateGLES2
	#define nvgDelete	nvgDeleteGLES2

	/*#include <GLES3/gl3.h>
	#define NANOVG_GLES3_IMPLEMENTATION
	#define nvgCreate	nvgCreateGLES3
	#define nvgDelete	nvgDeleteGLES3*/

#elif __linux

	#include <GL/glx.h>

	#define NANOVG_GL2_IMPLEMENTATION
	#define nvgCreate	nvgCreateGL2
	#define nvgDelete	nvgDeleteGL2

#elif __unix // all unices not caught above
	// Unix
#elif __posix
	// POSIX
#endif

#include "nanovg.h"
#include "nanovg_gl.h"


#endif // !CATGL_NANOVG_H_
