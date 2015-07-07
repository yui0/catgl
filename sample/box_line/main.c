//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

GLuint program;
GLuint vao;

// バーテックスシェーダのソースプログラム
static const GLchar vsrc[] =
	"#version 120\n"
	"attribute vec2 position;"
	"void main() {"
	"   gl_Position = vec4(position, 0.0, 1.0);"
	"}";

// フラグメントシェーダのソースプログラム
static const GLchar fsrc[] =
	"#version 120\n"
	"void main() {"
	"   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
	"}";

// 図形データ
static const GLfloat position[][2] =
{
	{ -0.5f, -0.5f },
	{  0.5f, -0.5f },
	{  0.5f,  0.5f },
	{ -0.5f,  0.5f }
};

int vertices;

GLuint _caCreateObject(const GLfloat *position, int size, GLuint num)
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

// 表示の初期化
void caInit(int width, int height)
{
	vertices = sizeof position / sizeof position[0];

	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	vao = _caCreateObject(position, 2, vertices);
}

// 描画
void caRender()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_LOOP, 0, vertices);
	glBindVertexArray(0);

	glUseProgram(0);
}

void caEnd()
{
	glDeleteProgram(program);
}
