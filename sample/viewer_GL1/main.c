//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <math.h>

#define CATGL_GLES_IMPLEMENTATION	// Use OpenGL ES1
#include "catgl.h"
#include "catgl_obj.h"

CATGL_MODEL m;
GLuint vbo[2];

static float angle = 0;

#include <dirent.h>
char *getObjFile(/*char *path, char *ext, */int c)
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
		caDeleteObject_GL1(vbo);
		caUnloadObj(&m);
		caLoadObj(&m, s);
		caCreateObject_GL1(vbo, &m);
	}
}
void mouseEvent(int button, int action, int x, int y)
{
	keyEvent(CATGL_KEY_RIGHT, action);
}

// 透視投影の設定
void gluPerspective(double fovy, double aspect, double zNear, double zFar)
{
	GLfloat xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * CATGL_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
//	glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void caInit(int width, int height)
{
	// Set background color and clear buffers
	glClearColor((0.3922f+7*0.5f)/8, (0.1176f+7*0.5f)/8, (0.5882f+7*0.5f)/8, 1.0f);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0);
	glDepthFunc(GL_LEQUAL);

	float noAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, noAmbient);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnableClientState(GL_VERTEX_ARRAY);

	// 縦横比の設定
	float ratio = (float)width / (float)height;
	glViewport(0, 0, (int)width, (int)height);

	// 行列演算のモード設定
	glMatrixMode(GL_PROJECTION);
	// 単位行列のロード
	glLoadIdentity();
	// 透視投影の設定
	gluPerspective(40.0, ratio, 0.1, 100);

	caLoadObj(&m, "teapot.obj");
	caCreateObject_GL1(vbo, &m);

	caKeyEvent = keyEvent;
	caMouseEvent = mouseEvent;
}

void caRender()
{
	// 行列演算のモード設定
	glMatrixMode(GL_MODELVIEW);
	// 単位行列のロード
	glLoadIdentity();
	// 平行移動
	glTranslatef(0.0, 0.0, -3.0);
	// 角度を演算
	angle = (angle + 2);
	if (angle >= 360) {
		angle = 0;
	}
	// 角度を設定
	glRotatef(angle, angle, angle, 1.0f);

	// バッファをクリアー
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	caDrawObject(vbo, &m);
}

void caEnd()
{
	caDeleteObject_GL1(vbo);
	caUnloadObj(&m);
}
