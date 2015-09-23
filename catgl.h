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

#include <stdio.h>
#include <math.h>
#include <memory.h>

#ifdef CATGL_GLES_IMPLEMENTATION
#define CATGL_GLES
#define CATGL_IMPLEMENTATION
#endif

#define CATGL_PI			3.14159265358979

#define CATGL_MODE_POINT		GL_POINTS
#define CATGL_MODE_LINE		GL_LINES
#define CATGL_MODE_TRIANGLES	GL_TRIANGLES
int caMode = 4;// = CATGL_MODE_TRIANGLES;

typedef struct _CATGL_VERTEX
{
	float x, y, z;
	float r, g, b, a;
	float u, v;
} CATGL_VERTEX;

void (*caMouseEvent)(int button, int action, int x, int y);
void (*caKeyEvent)(int key, int action);

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

#ifdef CATGL_GLES
	#include <GLES/gl.h>
#endif
	#include <GLES2/gl2.h>
	//#include <GLES2/gl2ext.h>

	// for debug
	#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  TAG, __VA_ARGS__))
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

	#define CATGL_KEY_ESCAPE		AKEYCODE_ESCAPE
	#define CATGL_KEY_A			AKEYCODE_A
	#define CATGL_KEY_B			AKEYCODE_B
	#define CATGL_KEY_C			AKEYCODE_C
	#define CATGL_KEY_P			AKEYCODE_P
	#define CATGL_KEY_T			AKEYCODE_T
	#define CATGL_KEY_W			AKEYCODE_W
	#define CATGL_KEY_UP		AKEYCODE_F
	#define CATGL_KEY_DOWN		AKEYCODE_V
	#define CATGL_KEY_RIGHT		AKEYCODE_B
	#define CATGL_KEY_LEFT		AKEYCODE_C
	#define CATGL_KEY_PLUS		AKEYCODE_PLUS
	#define CATGL_KEY_MINUS		AKEYCODE_MINUS

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
	#define LOGE(...) (fprintf(stderr, __VA_ARGS__))
//	#ifdef DEBUG
	#define LOGD(...) (fprintf(stderr, __VA_ARGS__))
//	#else
//	#define LOGD(...)
//	#endif

//	#define CATGL_ASSETS(s)	("assets/" s)
	#define CATGL_ASSETS(s)	caGetPath(s)

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

	#define CATGL_KEY_ESCAPE		GLFW_KEY_ESCAPE
	#define CATGL_KEY_A			GLFW_KEY_A
	#define CATGL_KEY_B			GLFW_KEY_B
	#define CATGL_KEY_C			GLFW_KEY_C
	#define CATGL_KEY_P			GLFW_KEY_P
	#define CATGL_KEY_T			GLFW_KEY_T
	#define CATGL_KEY_W			GLFW_KEY_W
	#define CATGL_KEY_UP		GLFW_KEY_UP
	#define CATGL_KEY_DOWN		GLFW_KEY_DOWN
	#define CATGL_KEY_RIGHT		GLFW_KEY_RIGHT
	#define CATGL_KEY_LEFT		GLFW_KEY_LEFT
	#define CATGL_KEY_PLUS		GLFW_KEY_KP_ADD
	#define CATGL_KEY_MINUS		GLFW_KEY_KP_SUBTRACT

#elif __unix // all unices not caught above
	// Unix
#elif __posix
	// POSIX
#endif


#include "catgl_obj.h"

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

// ファイルの内容をメモリに割り当て
char *caGetFileContents(const char *file_name)
{
	char *buf;
	FILE *fp;
	size_t read_size, buf_size;

	fp = fopen(file_name, "r");
	if (!fp) {
		LOGE("Cannot open %s.\n", file_name);
		exit(EXIT_FAILURE);
	}

	buf_size = BUFSIZ;
	buf = malloc(sizeof(char) * buf_size);
	if (!buf) {
		LOGE("Memory allocation error.\n");
		exit(EXIT_FAILURE);
	}

	read_size = 0;
	for (;;) {
		size_t s;
		s = fread(buf + read_size, sizeof(char), BUFSIZ, fp);
		read_size += s;
		if (s < BUFSIZ) break;
		buf_size += BUFSIZ;
		buf = realloc(buf, sizeof(char) * buf_size);
		if (!buf) {
			LOGE("Memory allocation error.\n");
			exit(EXIT_FAILURE);
		}
	}
	*(buf + read_size) = '\0';

	return buf;
}

// 行列4x4
void caMultMatrix(float *a, float *b, float *r)
{
	r[ 0] = a[0] * b[0] + a[4] * b[1] +  a[8] * b[2] + a[12] * b[3];
	r[ 1] = a[1] * b[0] + a[5] * b[1] +  a[9] * b[2] + a[13] * b[3];
	r[ 2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	r[ 3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	r[ 4] = a[0] * b[4] + a[4] * b[5] +  a[8] * b[6] + a[12] * b[7];
	r[ 5] = a[1] * b[4] + a[5] * b[5] +  a[9] * b[6] + a[13] * b[7];
	r[ 6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	r[ 7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	r[ 8] = a[0] * b[8] + a[4] * b[9] +  a[8] * b[10]+ a[12] * b[11];
	r[ 9] = a[1] * b[8] + a[5] * b[9] +  a[9] * b[10]+ a[13] * b[11];
	r[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10]+ a[14] * b[11];
	r[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10]+ a[15] * b[11];
	r[12] = a[0] * b[12]+ a[4] * b[13]+  a[8] * b[14]+ a[12] * b[15];
	r[13] = a[1] * b[12]+ a[5] * b[13]+  a[9] * b[14]+ a[13] * b[15];
	r[14] = a[2] * b[12]+ a[6] * b[13]+ a[10] * b[14]+ a[14] * b[15];
	r[15] = a[3] * b[12]+ a[7] * b[13]+ a[11] * b[14]+ a[15] * b[15];
}
// 合成(glMultMatrixf)
/*void caMultMatrix(float *s1, float *s2, float *r)
{
	int x, y;
	for (y=0; y<4; y++) {
		for (x=0; x<4; x++) {
			r[y*4+x] = s2[y*4]*s1[x] + s2[y*4+1]*s1[x+4] + s2[y*4+2]*s1[x+8] + s2[y*4+3]*s1[x+12];
		}
	}
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
void caSetPosition(float *m, float x, float y, float z)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;
}
// 射影行列を近面、遠面、視野角、アスペクト比で指定
void caMakeProjectionMatrix(float *m, float n, float f, float hfov, float r)
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
	glShaderSource(shader, 1, &pSource, 0);
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
	//GLint linkStatus = GL_FALSE;
	//glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	glUseProgram(program);
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
GLuint caCreateObject_(void *obj, int size, GLuint num, GLuint d[], int len)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size * num, obj, GL_STATIC_DRAW);
//	LOGD("size:%d, num:%d, len:%d\n", size, num, len);

	int i, offset = 0;
	for (i=0; i<len; i++) {
		glEnableVertexAttribArray(d[i*2]);
		glVertexAttribPointer(d[i*2], d[i*2+1], GL_FLOAT, GL_FALSE, size, (const void*)offset);
		offset += sizeof(float) * d[i*2+1];
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

GLuint caCreateTexture(unsigned char *tex, int w, int h)
{
	GLuint id;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// テクスチャの設定を行う
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	return id;
}
#define STB_IMAGE_IMPLEMENTATION
#include "nanovg/stb_image.h"
GLuint caLoadTexture(char *name)
{
	unsigned char *pixels;
	int width, height, bpp;
	pixels = stbi_load(CATGL_ASSETS(name), &width, &height, &bpp, 4/*RGBA*/);
	//LOGD("%dx%d\n", width, height);
	GLuint id = caCreateTexture(pixels, width, height);
	stbi_image_free(pixels);
	return id;
}

// draw model for .obj
void caDrawObject(GLuint *vbo, CATGL_MODEL *m)
{
//	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(caMode, 0, m->num_vertices);
//	glDisableClientState(GL_VERTEX_ARRAY);
}
void caCreateObject_GL1(GLuint *vbo, CATGL_MODEL *m)
{
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertices *3, m->vertices, GL_STATIC_DRAW);
}
void caDeleteObject_GL1(GLuint *vbo)
{
	glDeleteBuffers(2, vbo);
}

#endif // !CATGL_IMPLEMENTATION


#ifdef __cplusplus
}
#endif

#endif // !CATGL_H_
