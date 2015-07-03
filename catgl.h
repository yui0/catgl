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
	//#include <glext.h>

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
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <GLUT/glut.h>
	#else
		// Unsupported platform
	#endif

#elif __ANDROID__

/*	#ifdef __cplusplus
		#undef __cplusplus
		#define cxx
	#endif*/
	#include "catgl_android.h"
/*	#ifdef cxx
		#ifdef __cplusplus
	#endif*/
	#include <GLES2/gl2.h>
	//#include <GLES2/gl2ext.h>

	#define CA_PATH(s)	caGetPath(s)

#elif __linux

	#ifndef GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
	#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
	//#include <GL/glx.h>
	//#include <GL/glut.h>

	#include "catgl_glfw.h"

	#define CA_PATH(s)	("assets/" s)

#elif __unix // all unices not caught above
	// Unix
#elif __posix
	// POSIX
#endif


/*void caPrintShaderInfo(GLuint shader)
{
	int logSize;
	int length;

	// ログの長さは、最後のNULL文字も含む
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

	if (logSize > 1) {
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		fprintf(stderr, "Shader Info\n%s\n", s_logBuffer);
	}
}*/

GLuint caLoadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &pSource, /*nullptr*/0);
	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
//	if (!compiled) printf("Error at glCompileShader\n");
	return shader;
}

GLuint caCreateProgram(const char* pVertexSource, const char* pFragmentSource)
{
	GLuint vertexShader = caLoadShader(GL_VERTEX_SHADER, pVertexSource);
	GLuint pixelShader = caLoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glAttachShader(program, pixelShader);
	glDeleteShader(pixelShader);
//	glBindAttribLocation(program, 0, "vPosition");
//	glBindFragDataLocation(program, 0, "gl_FragColor");
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	return program;
}

GLuint caCreateTexture(unsigned char *tex, int w, int h)
{
	GLuint id;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// テクスチャの設定を行う
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	return id;
}
//GLuint caLoadTexture(char *name)


#endif // !CATGL_H_
