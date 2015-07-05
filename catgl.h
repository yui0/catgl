//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#ifndef CATGL_H_
#define CATGL_H_


#ifdef CATGL_GLES_IMPLEMENTATION
#define CATGL_GLES
#define CATGL_IMPLEMENTATION
#endif

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

	#include <GLES2/gl2.h>
	//#include <GLES2/gl2ext.h>

#ifdef CATGL_IMPLEMENTATION
/*	#ifdef __cplusplus
		#undef __cplusplus
		#define cxx
	#endif*/
	#include "catgl_android.h"
/*	#ifdef cxx
		#ifdef __cplusplus
	#endif*/
	#define NANOVG_GLES2_IMPLEMENTATION
#else
	#define NANOVG_GLES2
#endif
	#define nvgCreate	nvgCreateGLES2
	#define nvgDelete	nvgDeleteGLES2

	/*#include <GLES3/gl3.h>
	#define NANOVG_GLES3_IMPLEMENTATION
	#define nvgCreate	nvgCreateGLES3
	#define nvgDelete	nvgDeleteGLES3*/

	#define CATGL_ASSETS(s)	caGetPath(s)

#elif __linux

	#ifndef GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
	#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
	//#include <GL/glx.h>
	//#include <GL/glut.h>

#ifdef CATGL_IMPLEMENTATION
	#include "catgl_glfw.h"
	#define NANOVG_GL2_IMPLEMENTATION
#else
	#define NANOVG_GL2
#endif
	#define nvgCreate	nvgCreateGL2
	#define nvgDelete	nvgDeleteGL2

	#define CATGL_ASSETS(s)	("assets/" s)

#elif __unix // all unices not caught above
	// Unix
#elif __posix
	// POSIX
#endif

#ifdef CATGL_NANOVG
#include "nanovg.h"
#include "nanovg_gl.h"

// icon font
#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729
#endif


#ifdef CATGL_IMPLEMENTATION
#ifdef CATGL_NANOVG
char* cpToUTF8(int cp, char* str)
{
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
	case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
	case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
	case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
	case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
	case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
	case 1: str[0] = cp;
	}
	return str;
}
#include "catgl_window.h"
#include "catgl_searchbox.h"
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

GLuint caCreateProgram(const char* pVertexSource, const char *pv, const char* pFragmentSource, const char *fc)
{
	GLuint vertexShader = caLoadShader(GL_VERTEX_SHADER, pVertexSource);
	GLuint pixelShader = caLoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glAttachShader(program, pixelShader);
	glDeleteShader(pixelShader);
	glBindAttribLocation(program, 0, pv);
	glBindFragDataLocation(program, 0, fc);
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	return program;
}

GLuint caCreateObject(const GLfloat *position, int size, GLuint num)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size * num, position, GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vao;
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

#endif // !CATGL_IMPLEMENTATION


#endif // !CATGL_H_
