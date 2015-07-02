//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include "catgl.h"
#include "catgl_nanovg.h"

struct NVGcontext* vg;
int width, height;
float pixelRatio;

// 表示の初期化
void caInit(int w, int h)
{
	width = w;
	height = h;
	pixelRatio = (float)width / (float)height;

	vg = nvgCreate(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

//	if (loadDemoData(vg, &data) == -1) return -1;
}

// 四角形の描画
void caRender()
{
	glViewport(0, 0, width, height);

	//glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

/*	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);*/

	nvgBeginFrame(vg, width, height, pixelRatio);
	renderDemo(vg, 200, 200, width, height, 5, 0, 0);
	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
