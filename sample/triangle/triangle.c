//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

// Shader sources
static const GLchar vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"
	"uniform mat4 modelviewMatrix;"
	"attribute vec3 position;"		// in
	"attribute vec3 v_color;"		// in
	"varying vec3 f_color;"		// out
//	"attribute vec2 texcoord;"		// in
//	"varying vec2 texcoordVarying;"	// out
	"void main() {"
	"   gl_Position = projectionMatrix * modelviewMatrix * vec4(position, 1.0);"
//ok	"   gl_Position = gl_ModelViewMatrix * vec4(position, 1.0);"
//x	"   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"
	"   f_color = v_color;"
//	"   texcoordVarying = texcoord;"
	"}";
static const GLchar fsrc[] =
	"#version 120\n"
	"varying vec3 f_color;"
	"void main(void) {"
	"  gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);"
	"}";

CATGL_VERTEX obj[] =
{
	{  0.0f,  0.8f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f },
	{ -0.8f, -0.8f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f },
	{  0.8f, -0.8f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f },
};

GLuint program;
GLuint vbo;

// 表示の初期化
void caInit(int width, int height)
{
	glViewport(0, 0, width, height);

	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");

	GLuint att[3];
	att[0] = glGetAttribLocation(program, "position");
	att[1] = glGetAttribLocation(program, "v_color");
	att[2] = 65535;
	vbo = caCreateObject(obj, sizeof(obj)/sizeof(obj[0]), att);

	glUseProgram(program);
}

// 描画
void caRender()
{
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	//glTranslatef(0.0, 0.0, -50.0);
	static float angle;
	angle += 2;
	if (angle > 360) {
		angle = 0;
	}
//	glRotatef(angle, 0, 0, 1.0f);

/*	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	caPrintMatrix(m);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_FALSE, m);
	glGetFloatv(GL_PROJECTION_MATRIX, m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, m);*/

	float m[16];
	caMakeUnit(m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, m);
	caRotationZ(m, angle);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_FALSE, m);

	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw a triangle from the 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void caEnd()
{
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);
}
