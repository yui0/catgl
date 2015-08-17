//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <math.h>

#define CATGL_IMPLEMENTATION
#include "catgl.h"

/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"
	"uniform mat4 modelviewMatrix;"
	"attribute vec3 position;"		// in
	"void main() {"
	"   gl_Position = projectionMatrix * modelviewMatrix * vec4(position, 1.0);"
	"}";
char fsrc[] =
	"#version 120\n"
	"void main() {"
	"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
	"}";*/

/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// param
	"uniform mat4 modelviewMatrix;"	// param
	"attribute vec3 position;"		// in
	"attribute vec3 normal;"		// in
	"varying vec3 color;"		// out
	"void main() {"
	"  vec3 light = normalize(vec3(4.0, 4.0, 4.0));"
	"  color = vec3(1.0, 1.0, 1.0) * clamp(dot(normal, light), 0, 1);"
	"  gl_Position = projectionMatrix * modelviewMatrix * vec4(position, 1.0);"
	"}";*/
char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// param
	"uniform mat4 viewMatrix;"		// param
	"uniform mat4 modelviewMatrix;"	// param
	"attribute vec3 position;"		// in
	"attribute vec3 normal;"		// in
	"varying vec3 color;"		// out
//	"attribute vec2 texcoord;"		// in
//	"varying vec2 texcoordVarying;"	// out
	"void main() {"
	"  vec4 n = normalize(modelviewMatrix * viewMatrix * vec4(normal, 1));"
	"  vec4 light = normalize(viewMatrix * vec4(4.0, 4.0, 4.0, 1.0));"
	"  color = vec3(1.0, 1.0, 1.0) * clamp(dot(n, light), 0, 1);"
	"  gl_Position = projectionMatrix * viewMatrix * modelviewMatrix * vec4(position, 1.0);"
//	"  texcoordVarying = texcoord;"
	"}";
char fsrc[] =
	"#version 120\n"
	"varying vec3 color;"		// in
	"void main() {"
	"  gl_FragColor = vec4(color, 1.0);"
	"}";

GLuint program;
GLuint vbo[2];
CATGL_MODEL m;

void load_obj(GLuint vbo[], CATGL_MODEL *m, char *file)
{
	caLoadObj(m, file);

	int len = 1, size = 3;
	GLuint d[4];
	d[0] = glGetAttribLocation(program, "position");
	d[1] = 3;
	if (m->normal) {
		d[2] = glGetAttribLocation(program, "normal");
		d[3] = 3;
		len++;
		size += 3;
	}
	vbo[0] = caCreateObject_(m->vertices, sizeof(float)*size, m->num_vertices/3, d, len);
}

#include <dirent.h>
char *getObjFile(int c)
{
	char *path = "./";
	char *ext = ".obj";

	static char s[BUFSIZ];
	int n = -1, e = strlen(ext);

	//printf("+%s\n", path);
	DIR *dir = opendir(CATGL_ASSETS(path));
	struct dirent *d;
	while ((d = readdir(dir)) != NULL) {
		char *p = strchr(d->d_name, 0);
		//printf("-%s,%s\n", d->d_name, p-e);
		if (strstr(p-e, ext)) {
			n++;
			if (n==c) {
				strncpy(s, d->d_name, BUFSIZ);
				break;
			}
		}
	}
	closedir(dir);

	if (n==c) return s;
	return 0;
}

void keyEvent(int key, int action)
{
	static int obj;
	char *s = 0;

	if (action == CATGL_ACTION_DOWN) {
		switch (key) {
		case CATGL_KEY_P:
			caMode = CATGL_MODE_POINT;
			break;
		case CATGL_KEY_W:
			caMode = CATGL_MODE_LINE;
			break;
		case CATGL_KEY_T:
			caMode = CATGL_MODE_TRIANGLES;
			break;
		case CATGL_KEY_RIGHT:
			obj++;
			s = getObjFile(obj);
			if (!s) {
				obj = 0;
				s = getObjFile(obj);
			}
			break;
		case CATGL_KEY_LEFT:
			obj--;
			if (obj<0) obj = 0;
			s = getObjFile(obj);
			break;
		}
	}

	if (s) {
		printf("%s\n", s);
		glDeleteBuffers(1, vbo);
		caUnloadObj(&m);

		load_obj(vbo, &m, s);
	}
}
void mouseEvent(int button, int action, int x, int y)
{
	keyEvent(CATGL_KEY_RIGHT, action);
}

float aspect;
void caInit(int width, int height)
{
	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	load_obj(vbo, &m, "teapot.obj");

	aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glClearColor(0.6, 0.8, 0.8, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	// カリングの有効化

//	glFrontFace(GL_CCW);	// 裏面を描画しない
//	glCullFace(GL_BACK);

	caKeyEvent = keyEvent;
	caMouseEvent = mouseEvent;
}

void caRender()
{
	static float angle;
	angle += 2;
	if (angle > 360) {
		angle = 0;
	}

	float mat[16];
	// 透視投影変換行列
	caMakeProjectionMatrix(mat, 1, 1000, 53, aspect);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, mat);
	caMakeUnit(mat);
	caSetPosition(mat, 0, 0, -1);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, mat);
	caRotationZ(mat, angle);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_FALSE, mat);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	caDrawObject(vbo, &m);
}

void caEnd()
{
	glDeleteBuffers(1, vbo);
	caUnloadObj(&m);
}
