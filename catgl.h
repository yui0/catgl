//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#ifndef CATGL_H_
#define CATGL_H_


#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)

	#include <windows.h>
	#include <GL/gl.h>
	#include <glext.h>

	#ifdef _WIN64
		//define something for Windows (64-bit only)
	#endif

#elif __APPLE__
	#if TARGET_IPHONE_SIMULATOR
		// iOS Simulator
	#elif TARGET_OS_IPHONE
		// iOS device
		#include <OpenGLES/ES1/gl.h>
	#elif TARGET_OS_MAC
		// Other kinds of Mac OS
		#include <GLUT/glut.h>
	#else
		// Unsupported platform
	#endif

#elif __ANDROID__

	#include "catgl_android.h"
	#include <GLES2/gl2.h>

	// for nanovg
	#define NANOVG_GLES2_IMPLEMENTATION
	#define nvgCreate	nvgCreateGLES2
	#define nvgDelete	nvgDeleteGLES2

	/*#include <GLES3/gl3.h>
	#define NANOVG_GLES3_IMPLEMENTATION
	#define nvgCreate	nvgCreateGLES3
	#define nvgDelete	nvgDeleteGLES3*/

#elif __linux
	// linux

	#include "catgl_glfw.h"
	#include <GL/glx.h>

	// for nanovg
	#define NANOVG_GL2_IMPLEMENTATION
	#define nvgCreate	nvgCreateGL2
	#define nvgDelete	nvgDeleteGL2

#elif __unix // all unices not caught above
	// Unix
#elif __posix
	// POSIX
#endif


#endif // !CATGL_H_
