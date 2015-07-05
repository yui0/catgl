//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

// Shader sources
const GLchar* vertexSource =
	"#version 120\n"
	"attribute vec2 position;"
	"void main() {"
	"   gl_Position = vec4(position, 0.0, 1.0);"
	"}";
const GLchar* fragmentSource =
	"#version 120\n"
	"void main() {"
	"   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
	"}";
/*const GLchar* fragmentSource =
	"out vec4 gl_FrontColor;"
	"void main() {"
//	"  // 位置座標を座標変換\n"
	"  gl_Position = ftransform();"
//	"  // 法線から色を決定\n"
	"  gl_FrontColor.rgb = 0.5 * gl_Normal.xyz + 0.5;"
	"  gl_FrontColor.a = 1.0;"
	"}";*/

// 表示の初期化
void caInit(int width, int height)
{
	glViewport(0, 0, width, height);

	GLuint program = caCreateProgram(vertexSource, "position", fragmentSource, "gl_FragColor");
	glUseProgram(program);

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

// 描画
void caRender()
{
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -50.0);
	static float angle;
	angle += 2;
	if (angle > 360) {
		angle = 0;
	}
	glRotatef(angle, 0, 0, 1.0f);*/

	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw a triangle from the 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void caEnd()
{
/*	glDeleteProgram(program);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);*/
}
