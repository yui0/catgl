//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <math.h>
#ifndef M_PI
#define M_PI 3.141592653589793238
#endif

#define CATGL_GLES_IMPLEMENTATION	// Use OpenGL ES1
#include "catgl.h"

static float angle = 0;

// 透視投影の設定
void gluPerspective(double fovy, double aspect, double zNear, double zFar)
{
	GLfloat xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
//	glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

// 頂点リスト
#define LENGTH (15)
short triangleBuffer[] = {
	/*        X                Y          Z */
	-LENGTH / 2,          -LENGTH / 2,          0,
	 LENGTH - LENGTH / 2, -LENGTH / 2,          0,
	-LENGTH / 2,           LENGTH - LENGTH / 2, 0,
	 LENGTH - LENGTH / 2,  LENGTH - LENGTH / 2, 0,
};

// 頂点カラーリスト
float colorBuffer[] = {
	/*   R    G    B    A  */
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.5, 0.5, 0.0, 1.0,
};

// 表示の初期化
void caInit(int width, int height)
{
	// 光源処理無効化
	glDisable(GL_LIGHTING);
	// 背面処理無効化
	glDisable(GL_CULL_FACE);
	// デプスバッファ無効化
	glDisable(GL_DEPTH_BUFFER_BIT);
	// デプステスト無効化
	glDisable(GL_DEPTH_TEST);
	// 塗りつぶし色設定
	glClearColor(.7f, .7f, .9f, 1.f);
	// 陰影モード設定
	glShadeModel(GL_SMOOTH);

	// 縦横比の設定
	float ratio = (float)width / (float)height;
	glViewport(0, 0, (int)width, (int)height);

	// 行列演算のモード設定
	glMatrixMode(GL_PROJECTION);
	// 単位行列のロード
	glLoadIdentity();
	// 透視投影の設定
	gluPerspective(40.0, ratio, 0.1, 100);
}

// 四角形の描画
void caRender()
{
	// 行列演算のモード設定
	glMatrixMode(GL_MODELVIEW);
	// 単位行列のロード
	glLoadIdentity();
	// 平行移動
	glTranslatef(0.0, 0.0, -50.0);
	// 角度を演算
	angle = (angle + 2);
	if (angle > 360) {
		angle = 0;
	}
	// 角度を設定
	glRotatef(angle, 0, 0, 1.0f);

	// バッファをクリアー
	glClear(GL_COLOR_BUFFER_BIT);
	// 頂点リスト指定
	glVertexPointer(3, GL_SHORT, 0, triangleBuffer);
	// 頂点リストの有効化
	glEnableClientState(GL_VERTEX_ARRAY);
	// 頂点カラーリスト指定
	glColorPointer(4, GL_FLOAT, 0, colorBuffer);
	// 頂点カラーリストの有効化
	glEnableClientState(GL_COLOR_ARRAY);
	// 三角形 描画
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void caEnd()
{
}
