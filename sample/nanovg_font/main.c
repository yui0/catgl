//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

#include "mplus1pregular.h"

struct NVGcontext* vg;
int width, height;
float pixelRatio;

// 表示の初期化
void caInit(int w, int h)
{
	width = w;
	height = h;

	// Calculate pixel ration for hi-dpi devices.
	pixelRatio = (float)width / (float)height;

	vg = nvgCreate(NVG_ANTIALIAS);

	//nvgCreateFont(vg, "sans-bold", CATGL_ASSETS("Roboto-Bold.ttf"));
	nvgCreateFontMem(vg, "sans-bold", mplus1pregular, sizeof(mplus1pregular), 0);
	nvgFontFace(vg, "sans-bold");
}

// 描画
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

	nvgBeginFrame(vg, width, height, pixelRatio);

	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(255,192,0,255));
	nvgText(vg, 100, 100, "Hello! こんにちは！", NULL);
	nvgFill(vg);

	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
