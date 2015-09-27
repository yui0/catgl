//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

const char* vertex_shader =
	"attribute vec4 position;\n"
	"attribute vec2 texcoord;\n"
	"varying vec2 texcoordVarying;\n"
	"void main() {\n"
	"gl_Position = position;\n"
	"texcoordVarying = texcoord;\n"
	"}\n";

const char* fragment_shader =
//	"precision mediump float;\n"
	"varying vec2 texcoordVarying;\n"
	"uniform sampler2D texture;\n"
	"void main() {\n"
	"gl_FragColor = texture2D(texture, texcoordVarying);\n"
	"}\n";

/*const float vertices[] = {
	-1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};

const float texcoords[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};*/

const float vertices[] = {
	-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 1.0f
};

GLuint vbo[2];
GLuint program, position, texcoord;
GLuint textures[1];

// ダミーテクスチャ
static unsigned char tex[] = {
	255, 255, 255, 255,     0,   0,   0, 255,   255, 255, 255 ,255,     0,   0,   0, 255,
	255,   0,   0, 255,     0, 255,   0, 255,     0,   0, 255 ,255,   255, 255, 255, 255,
	128,   0,   0, 255,     0, 128,   0, 255,     0,   0, 128 ,255,   128, 128, 128, 255,
	255, 255,   0, 255,   255,   0, 255, 255,     0, 255, 255 ,255,   255, 255, 255, 255,
};

void caInit(int width, int height)
{
	glViewport(0, 0, width, height);

	program = caCreateProgram(vertex_shader, "position", fragment_shader, "gl_FragColor");
	if (program == 0) {
		return;
	}
	glUseProgram(program);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glEnableVertexAttribArray(CATGL_ATT_VERTEX);
//	glVertexAttribPointer(CATGL_ATT_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(CATGL_ATT_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);

//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glEnableVertexAttribArray(CATGL_ATT_TEXTURE);
//	glVertexAttribPointer(CATGL_ATT_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(CATGL_ATT_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, sizeof(float)*3);

	textures[0] = caCreateTexture(tex, 4, 4);
}

void caRender()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void caEnd()
{
	glDeleteTextures(1, textures);
	glDeleteProgram(program);
}
