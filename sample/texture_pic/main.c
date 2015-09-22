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
int t1;

GLfloat vertexs[] = {
	-0.5f, -0.5f, 0.0f,	// left top
	-0.5f,  0.5f, 0.0f,	// left bottom
	 0.5f, -0.5f, 0.0f,	// right top
	 0.5f,  0.5f, 0.0f,	// right bottom
};
GLfloat texcoords[] = {
	0.0f, 0.0f,	// left top
	0.0f, 1.0f,	// left bottom
	1.0f, 0.0f,	// right top
	1.0f, 1.0f,	// right bottom
};

GLuint vbo;
GLuint program, position, texcoord;
GLuint textures[1];

#define STB_IMAGE_IMPLEMENTATION
#include "nanovg/stb_image.h"
void caInit(int width, int height)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	// Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexs), sizeof(texcoord), (const void*)texcoord);

	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	glUseProgram(program);

	// Specify the layout of the vertex data
	position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);

//	glBindBuffer(GL_ARRAY_BUFFER, 0);// 0 を入れておかないといけない
//	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, vertices);

	texcoord = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(texcoord);
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);

		unsigned char* pixels;
		int _width;
		int _height;
		int bpp;
		pixels = stbi_load(CATGL_ASSETS("cat.jpg"), &_width, &_height, &bpp, /*0*/4);
		//LOGD("%dx%d\n", width, height);
		//stbi_image_free(pixels);

	textures[0] = glGetUniformLocation(program, "texture");
	glGenTextures(1, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glEnableVertexAttribArray(textures[0]);
//	glUniform1i(textures[0], 0);

//	t1 = caCreateTexture(tex, 4, 4);
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
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
}
