//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <math.h>

#define CATGL_GLES_IMPLEMENTATION	// Use OpenGL ES1
#include "catgl.h"

static float angle = 0;

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

#include "models.h"
MODEL_T m;
// 表示の初期化
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

	m = cube_wavefront();
}

// 四角形の描画
void caRender()
{
	// 行列演算のモード設定
	glMatrixMode(GL_MODELVIEW);
	// 単位行列のロード
	glLoadIdentity();
	// 平行移動
	glTranslatef(0.0, 0.0, -2.0);
	// 角度を演算
	angle = (angle + 2);
	if (angle >= 360) {
		angle = 0;
	}
	// 角度を設定
	glRotatef(angle, angle, angle, 1.0f);

	// バッファをクリアー
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_wavefront(m, 0);
}

void caEnd()
{
}
