//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

GLuint program;
GLuint textures[1];
GLuint vbo;

// バーテックスシェーダのソースプログラム
static const GLchar vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"
	"uniform mat4 modelviewMatrix;"
	"attribute vec3 position;"		// in
	"attribute vec2 texcoord;"		// in
	"varying vec2 texcoordVarying;"	// out
	"void main() {"
	"   gl_Position = projectionMatrix * modelviewMatrix * vec4(position, 1.0);"
	"   texcoordVarying = texcoord;"
	"}";

// フラグメントシェーダのソースプログラム
static const GLchar fsrc[] =
	"#version 120\n"
	"varying vec2 texcoordVarying;"
	"uniform sampler2D texture;"
	"void main() {"
	"   gl_FragColor = texture2D(texture, texcoordVarying);"
	"}";

CATGL_VERTEX obj[] =
{
	{ -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f },
	{  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f },
	{  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f },
	{ -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f },
};

GLuint tex;
GLuint pbo[2];
unsigned char image[640*480*4];

// 表示の初期化
void caInit(int width, int height)
{
	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");

	GLuint att[3];
	att[0] = glGetAttribLocation(program, "position");
	att[1] = 65535;
	att[2] = glGetAttribLocation(program, "texcoord");
	vbo = caCreateObject(obj, sizeof(obj)/sizeof(obj[0]), att);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// init 2 texture objects
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 640, 480, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)image);
	glBindTexture(GL_TEXTURE_2D, 0);

//	if (pboSupported) {
		// create 2 pixel buffer objects, you need to delete them when program exits.
		// glBufferDataARB with NULL pointer reserves only memory space.
		glGenBuffersARB(2, pbo);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[0]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, 640*480*4, 0, GL_STREAM_DRAW_ARB);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[1]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, 640*480*4, 0, GL_STREAM_DRAW_ARB);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
//	}
}

void caDirectRender(GLuint tex, GLuint pbo[2], int w, int h)
{
	// "index" is used to copy pixels from a PBO to a texture object
	// "nextIndex" is used to update pixels in the other PBO
	static int index;
	index = (index + 1) % 2;
	int nextIndex = (index + 1) % 2;

	// bind the texture and PBO
	glBindTexture(GL_TEXTURE_2D, tex);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[index]);

	// copy pixels from PBO to texture object
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, 0);

	// bind PBO to update texture source
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[nextIndex]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, w * h * 4, 0, GL_STREAM_DRAW_ARB);

	unsigned char* pix = (unsigned char*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	if (pix) {
		static int color = 0x20000070;
		int *ptr = (int*)pix;
		for(int y = 0; y < h ; ++y) {
			for(int x = 0; x < w; ++x) {
				*ptr = color;
				++ptr;
			}
			color += 257;
		}
		color++;
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);
	}
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

	glBindTexture(GL_TEXTURE_2D, tex);
	glUseProgram(program);

	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_FALSE, m);
//	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, /*projectionMatrix*/m);
	glDrawArrays(GL_QUADS, 0, sizeof(obj)/sizeof(obj[0]));

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// 描画
void caRender()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glScalef(1.0f, 1.0f, 1.0f);

	// tramsform camera
//	glTranslatef(0, 0, -cameraDistance);
//	glRotatef(cameraAngleX, 1, 0, 0);   // pitch
//	glRotatef(cameraAngleY, 0, 1, 0);   // heading
//	glRotatef(30, 1, 0, 0);
//	glTranslatef(50.0f, 60.0f, 0.0f);
	static float a = 0;
	glRotatef( a++/*30.0f*/, 0.0f, 0.0f, 1.0f );
	glRotatef( 20.0f, 1.0f, 0.0f, 0.0f );
	glRotatef( 50.0f, 0.0f, 1.0f, 0.0f );
	if (a>=360) a=0;

	caDirectRender(tex, pbo, 640, 480);

/*	glUseProgram(program);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(obj)/sizeof(obj[0]));
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);*/
}

void caEnd()
{
	glDeleteProgram(program);
}