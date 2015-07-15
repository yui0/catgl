//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#ifndef CATGL_H_
#define CATGL_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef CATGL_GLES_IMPLEMENTATION
#define CATGL_GLES
#define CATGL_IMPLEMENTATION
#endif

#define CATGL_PI			3.14159265358979

typedef struct _CATGL_VERTEX
{
	float x, y, z;
	float r, g, b, a;
	float u, v;
} CATGL_VERTEX;

void (*caMouseEvent)(int button, int action, int x, int y);

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

	//#include <GLES/gl.h>
	#include <GLES2/gl2.h>
	//#include <GLES2/gl2ext.h>

	// for debug
//	#ifdef DEBUG
	#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__))
//	#else
//	#define LOGD(...)
//	#endif

	// for OpenGL ES2
	#define glBindFragDataLocation(...)	// use 'gl_FragColor' only!!

	#define CATGL_ASSETS(s)	caGetPath(s)

#ifdef CATGL_IMPLEMENTATION
	#include "catgl_android.h"
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

	#define CATGL_ACTION_UP		AMOTION_EVENT_ACTION_UP
	#define CATGL_ACTION_DOWN		AMOTION_EVENT_ACTION_DOWN
	#define CATGL_ACTION_MOVE		AMOTION_EVENT_ACTION_MOVE

	#define CATGL_MOUSE_BUTTON_LEFT	AINPUT_SOURCE_TOUCHSCREEN
	#define CATGL_MOUSE_BUTTON_RIGHT	-1
	#define CATGL_MOUSE_BUTTON_MIDDLE	-1

#elif __linux

	#ifndef GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
	#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
	//#include <GL/glx.h>
	//#include <GL/glut.h>

	// for debug
//	#ifdef DEBUG
	#include <stdio.h>
	#define LOGD(...) (fprintf(stderr, __VA_ARGS__))
//	#else
//	#define LOGD(...)
//	#endif

	#define CATGL_ASSETS(s)	("assets/" s)

#ifdef CATGL_IMPLEMENTATION
	#include "catgl_glfw.h"
	#define NANOVG_GL2_IMPLEMENTATION
#else
	#define NANOVG_GL2
#endif
	#define nvgCreate	nvgCreateGL2
	#define nvgDelete	nvgDeleteGL2

	#define CATGL_ACTION_UP		GLFW_RELEASE	// 0
	#define CATGL_ACTION_DOWN		GLFW_PRESS	// 1
	#define CATGL_ACTION_MOVE		2

	#define CATGL_MOUSE_BUTTON_LEFT	GLFW_MOUSE_BUTTON_LEFT
	#define CATGL_MOUSE_BUTTON_RIGHT	GLFW_MOUSE_BUTTON_RIGHT
	#define CATGL_MOUSE_BUTTON_MIDDLE	GLFW_MOUSE_BUTTON_MIDDLE

#elif __unix // all unices not caught above
	// Unix
#elif __posix
	// POSIX
#endif


#ifdef CATGL_NANOVG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wsign-compare"
#include "nanovg.h"
#include "nanovg_gl.h"
#include "catgl_ui.h"
#pragma GCC diagnostic pop
#endif


#ifdef CATGL_IMPLEMENTATION
#ifdef CATGL_NANOVG
#include "fonts/entypo.h"
#include "fonts/mplus-1c-regular-sub.h"
#include "fonts/mplus-1c-black-sub.h"
//#include "fonts/mplus-1c-light-sub.h"
#define nvgCreateEx(vg,opt)	\
	vg = nvgCreate(opt); \
	nvgCreateFontMem(vg, "sans", mplus_1c_regular_sub_ttf, sizeof(mplus_1c_regular_sub_ttf), 0); \
	nvgFontFace(vg, "sans"); \
	nvgCreateFontMem(vg, "sans-bold", mplus_1c_black_sub_ttf, sizeof(mplus_1c_black_sub_ttf), 0); \
	nvgFontFace(vg, "sans-bold"); \
	nvgCreateFontMem(vg, "icons", (unsigned char*)entypo_ttf, sizeof(entypo_ttf), 0); \
	nvgFontFace(vg, "icons");
//	nvgCreateFontMem(vg, "icons", (unsigned char*)icons, sizeof(icons), 0);
//	nvgFontFace(vg, "icons");
#endif

// 合成(glMultMatrixf)
/*void caMultMatrix(float *s1, float *s2, float *d)
{
	for (int y=0; y<4; y++) {
		for (int x=0; x<4; x++) {
			d[y*4+x] = s2[y*4]*s1[x] + s2[y*4+1]*s1[x+4] + s2[y*4+2]*s1[x+8] + s2[y*4+3]*s1[x+12];
		}
	}
}

// 正規化
void caNormalize(float* v)
{
	float m=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (m > 0.0f) { m = 1.0f / m; } else { m = 0.0f; }
	v[0]*=m;
	v[1]*=m;
	v[2]*=m;
}
// 外積
void caCross(float *s1, float *s2, float *d)
{
	d[0] = s1[1]*s2[2] - s1[2]*s2[1];
	d[1] = s1[2]*s2[0] - s1[0]*s2[2];
	d[2] = s1[0]*s2[1] - s1[1]*s2[0];
}

// gluLookAt
void caLookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	float forward[3], side[3], up[3];

	forward[0] = centerx - eyex;
	forward[1] = centery - eyey;
	forward[2] = centerz - eyez;

	up[0] = upx;
	up[1] = upy;
	up[2] = upz;

	caNormalize(forward);

	caCross(forward, up, side);
	caNormalize(side);

	caCross(side, forward, up);

	float m[]= {
		side[0], up[0], -forward[0], 0,
		side[1], up[1], -forward[1], 0,
		side[2], up[2], -forward[2], 0,
		0, 0, 0, 1
	};

	float mov[]= {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eyex, -eyey, -eyez, 1
	};

	caMultiplication(m, mov, multi);
	//glLoadMatrixf(multi);
}

// gluPerspective
void caPerspective(float fovy, float aspect, float znear, float zfar)
{
	float radian=2*PI*fovy/360.0;
	float t = (float)(1.0 / tan(radian/2));
	float m[]= {
		t / aspect, 0, 0, 0,
		0, t, 0, 0,
		0, 0, (zfar + znear) / (znear - zfar), -1,
		0, 0, (2 * zfar * znear) / (znear - zfar), 0
	};
	//glLoadMatrixf(m);
}

// 基準座標変換
void makeOrthoMatrix(float left, float right, float bottom, float top, float *ret)
{
	float dx = right - left;
	float dy = top - bottom;
	float dz = 0;
 
	float tx = (dx != 0) ? -(right   left) / dx : 0;
	float ty = (dy != 0) ? -(top   bottom) / dy : 0;
	float tz = 0;
 
	ret[0] = 2.0f / dx;		ret[3] = tx;
	ret[5] = 2.0f / dy; 	ret[7] = ty;
	ret[10] = -2.0f / dz;	ret[11] = tz;
	ret[15] = 1;
}*/
#include <math.h>
#include <memory.h>
void caPrintMatrix(float *m)
{
	LOGD("m[0]:% 7.5f m[4]:% 7.5f m[8] :% 7.5f m[12]:% 7.5f\n", m[0], m[4], m[8],  m[12]);
	LOGD("m[1]:% 7.5f m[5]:% 7.5f m[9] :% 7.5f m[13]:% 7.5f\n", m[1], m[5], m[9],  m[13]);
	LOGD("m[2]:% 7.5f m[6]:% 7.5f m[10]:% 7.5f m[14]:% 7.5f\n", m[2], m[6], m[10], m[14]);
	LOGD("m[3]:% 7.5f m[7]:% 7.5f m[11]:% 7.5f m[16]:% 7.5f\n", m[3], m[7], m[11], m[15]);
}
// glLoadIdentity
void caMakeUnit(float *m)
{
	memset(m, 0, sizeof(float)*16);
	m[0] = m[5] = m[10]= m[15] = 1.0f;
}
void caRotationX(float *m, float degree)
{
	float rad = ((float)degree * CATGL_PI / 180.0);
	m[ 5] = cos(rad);
	m[ 9] = - sin(rad);
	m[ 6] = sin(rad);
	m[10] = cos(rad);
}
void caRotationY(float *m, float degree)
{
	float rad = ((float)degree * CATGL_PI / 180.0);
	m[ 0] = cos(rad);
	m[ 8] = sin(rad);
	m[ 2] = - sin(rad);
	m[10] = cos(rad);
}
void caRotationZ(float *m, float degree)
{
	float rad = ((float)degree * CATGL_PI / 180.0);
	m[ 0] = cos(rad);
	m[ 4] = - sin(rad);
	m[ 1] = sin(rad);
	m[ 5] = cos(rad);
}
// 射影行列を近面、遠面、視野角、アスペクト比で指定
void makeProjectionMatrix(float *m, float n, float f, float hfov, float r)
{
	float w = 1.0f / tan(hfov * 0.5f * CATGL_PI / 180);
	float h = w * r;
	float q = 1.0f / (f - n);

	memset(&m[1], 0, sizeof(float)*15);
	m[0] = w;
	m[5] = h;
	m[10]= -(f + n) * q;
	m[11]= -1.0f;
	m[14]= -2.0f * f * n * q;
//	m[1] = m[2] = m[3] = m[4]  = m[6]  = m[7]
//		= m[8] = m[9] = m[12] = m[13] = m[15] = 0.0f;
}

void caPrintShaderInfo(GLuint shader, const char *str)
{
	int logSize, length;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 1) {
		GLchar infoLog[logSize];
		glGetShaderInfoLog(shader, logSize, &length, infoLog);
		LOGD("Compile Error in %s\n%s\n", str, infoLog);
	}
}

GLuint caLoadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &pSource, /*nullptr*/0);
	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) caPrintShaderInfo(shader, pSource);
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

GLuint caCreateObject(void *obj, GLuint num, GLuint att[3])
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CATGL_VERTEX) * num, obj, GL_STATIC_DRAW);

	int offset = 0;
	glEnableVertexAttribArray(att[0]);
	glVertexAttribPointer(att[0], 3, GL_FLOAT, GL_FALSE, sizeof(CATGL_VERTEX), (const void*)offset);
	offset += sizeof(float) * 3;

	if (att[1]<65535) {
		glEnableVertexAttribArray(att[1]);
		glVertexAttribPointer(att[1], 4, GL_FLOAT, GL_FALSE, sizeof(CATGL_VERTEX), (const void*)offset);
	}
	offset += sizeof(float) * 4;

	if (att[2]<65535) {
		glEnableVertexAttribArray(att[2]);
		glVertexAttribPointer(att[2], 2, GL_FLOAT, GL_FALSE, sizeof(CATGL_VERTEX), (const void*)offset);
	}
//	offset += sizeof(float) * 2;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
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


#ifdef __cplusplus
}
#endif

#endif // !CATGL_H_
