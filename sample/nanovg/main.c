//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include "catgl.h"

#include "nanovg.h"
#include "nanovg_gl.h"

struct NVGcontext* vg;
int width, height;
float pixelRatio;

// 表示の初期化
void caInit(int w, int h)
{
	width = w;
	height = h;

	// Calculate pixel ration for hi-dpi devices.
	//pixelRatio = 1;//(float)fbWidth / (float)winWidth;
	pixelRatio = (float)width / (float)height;

	vg = nvgCreate(NVG_ANTIALIAS);
}

// 四角形の描画
void caRender()
{
	glViewport(0, 0, width, height);

	//glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	/*nvgBeginFrame(vg, width, height, pxRatio);
	renderDemo(vg, 200,200, width,height,5,0,0);
	nvgEndFrame(vg);*/

	nvgBeginFrame(vg, width, height, pixelRatio);

	nvgBeginPath(vg);
	nvgRect(vg, 100,100, 120,30);
	nvgFillColor(vg, nvgRGBA(255,192,0,255));
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, 200,200, 220,30);
	nvgCircle(vg, 220,220, 5);
	nvgPathWinding(vg, NVG_HOLE);   // Mark circle as a hole.
	//nvgPathWinding(vg, NVG_CW);
	nvgFillColor(vg, nvgRGBA(255,192,0,255));
	nvgFill(vg);

	/*nvgRect(vg, 0, 0, width, height);
	nvgFillColor(vg, nvgRGBA(110, 201, 235, 255));
	nvgStrokeColor(vg, nvgRGBA(110, 201, 235, 255));
	nvgStrokeWidth(vg, 10.0f);
	nvgFill(vg);
	nvgStroke(vg);*/

	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
