//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

// Shader sources
const GLchar* vsrc =
	"#version 120\n"
	"attribute vec4 position;"
	"attribute vec2 texcoord;"
	"varying vec2 texcoordVarying;"
	"void main() {"
	"   gl_Position = position;"
	"   texcoordVarying = texcoord;"
	"}";
const GLchar* fsrc =
	"#version 120\n"
	"varying vec2 texcoordVarying;"
	"uniform sampler2D texture;"
	"void main() {"
	"   gl_FragColor = texture2D(texture, texcoordVarying);"
	"}";

// ダミーテクスチャ
/*static unsigned char tex[] = {
	255, 255, 255, 255,     0,   0,   0, 255,   255, 255, 255 ,255,     0,   0,   0, 255,
	255,   0,   0, 255,     0, 255,   0, 255,     0,   0, 255 ,255,   255, 255, 255, 255,
	128,   0,   0, 255,     0, 128,   0, 255,     0,   0, 128 ,255,   128, 128, 128, 255,
	255, 255,   0, 255,   255,   0, 255, 255,     0, 255, 255 ,255,   255, 255, 255, 255,
};*/

/*GLfloat vertexs[] = {
	-0.8f, -0.8f,	// left top
	-0.8f,  0.8f,	// left bottom
	 0.8f, -0.8f,	// right top
	 0.8f,  0.8f,	// right bottom
};*/
/*GLfloat texcoords[] = {
	-1.0f, -1.0f,	// left top
	-1.0f,  1.0f,	// left bottom
	 1.0f, -1.0f,	// right top
	 1.0f,  1.0f,	// right bottom
};*/
GLfloat vertexs[] = {	// 数学の座標
	-0.8f,  0.8f,	// left top
	-0.8f, -0.8f,	// left bottom
	 0.8f,  0.8f,	// right top
	 0.8f, -0.8f,	// right bottom
};
GLfloat texcoords[] = {	// 数学の座標
	 0.0f,  0.0f,	// left top
	 0.0f,  1.0f,	// left bottom
	 1.0f,  0.0f,	// right top
	 1.0f,  1.0f,	// right bottom
};

GLuint vbo;
GLuint program, position, texcoord;
GLuint textures[1];

void caInit(int width, int height)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	// Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs)+sizeof(texcoords), vertexs, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexs), sizeof(texcoords), (const void*)texcoords);

	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	glUseProgram(program);

	// Specify the layout of the vertex data
	position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	texcoord = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(texcoord);
//	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, (const void *)sizeof(vertexs));

	textures[0] = caLoadTexture("cat.jpg");
}

void caRender()
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

//	glEnableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glBindTexture(GL_TEXTURE_2D, 0);
}

void caEnd()
{
	glDeleteTextures(1, textures);
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
}
