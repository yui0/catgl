//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <math.h>
#define M_PI 3.141592653589793238

#include "catgl.h"

// キューブの頂点情報。
static const GLdouble aCubeVertex[][3] = {
    { 0.0, 0.0, 0.0 },
    { 1.0, 0.0, 0.0 }, 
    { 1.0, 1.0, 0.0 }, 
    { 0.0, 1.0, 0.0 }, 
    { 0.0, 0.0, 1.0 }, 
    { 1.0, 0.0, 1.0 }, 
    { 1.0, 1.0, 1.0 }, 
    { 0.0, 1.0, 1.0 } 
};
// キューブの面。
static const int aCubeFace[][4] = {
    { 0, 1, 2, 3 },
    { 1, 5, 6, 2 },
    { 5, 4, 7, 6 },
    { 4, 0, 3, 7 },
    { 4, 5, 1, 0 },
    { 3, 2, 6, 7 }
};
// キューブに対する法線ベクトル。
static const GLdouble aCubeNormal[][3] = {
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  {-1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};
// キューブの材質。光源の色と合わせて陰影が少し青みがかって表示される。
static const GLfloat aCubeMaterial[] = { 0.8, 0.2, 0.2, 1.0 };
// 光源。
static const GLfloat aLightColor[] = { 0.2, 0.2, 0.8, 1.0 };// 光源の色。
static const GLfloat aLight0pos[] = { 0.0, 3.0, 5.0, 1.0 };// 光源0の位置。
static const GLfloat aLight1pos[] = { 5.0, 3.0, 0.0, 1.0 };// 光源1の位置。

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

// 表示の初期化
void caInit(int width, int height)
{
	/* OpenGLの初期化  */
	glEnable( GL_DEPTH_TEST );// デプスバッファの有効化。
	glEnable( GL_CULL_FACE );// カリングの有効化。
	glEnable( GL_LIGHTING );// ライティングの有効化。
	glEnable( GL_LIGHT0 );// 光源0 を有効化。
	glEnable( GL_LIGHT1 );// 光源1 を有効化。
	glCullFace( GL_FRONT );// カリング。
	glLightfv( GL_LIGHT1, GL_DIFFUSE, aLightColor );// 光源1の色を設定。
	glLightfv( GL_LIGHT1, GL_SPECULAR, aLightColor );// 光源1の色を設定。
	glClearColor( 1, 1, 1, 1 );// glClear() で使用する色（RGBA）

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
	// 画面をクリア。
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// 変換行列の初期化。ここで初期化しないと延々と同じ行列に積算される。
	glLoadIdentity();

	/* 更新 */
	// 視点設定前の行列をすべて平行移動（視界に収める）。
	glTranslated( 0.0, 0.0, -2.0 );
	// 視点の設定。
	//gluLookAt( 3.0, 4.0, 5.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
	// 光源の位置設定 （＊重要 視点の位置を設定した後に行う） 
	glLightfv( GL_LIGHT0, GL_POSITION, aLight0pos );
	glLightfv( GL_LIGHT1, GL_POSITION, aLight1pos );

	// キューブの材質パラメータを設定。
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, aCubeMaterial );
	// キューブの頂点を描画。
	glBegin( GL_QUADS );
	for (size_t i = 0; i < 6; ++i)
	{
		glNormal3dv( aCubeNormal[i] );// 法線ベクトルをキューブに当てる。
		for (size_t j = 0; j < 4; ++j)
		{
			glVertex3dv( aCubeVertex[ aCubeFace[i][j] ] );
		}
	}
	glEnd();
}
