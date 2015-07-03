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

NVGpaint pattern;
int handle;
int _width;
int _height;

// 表示の初期化
void caInit(int w, int h)
{
	width = w;
	height = h;

	// Calculate pixel ration for hi-dpi devices.
	pixelRatio = (float)width / (float)height;

	vg = nvgCreate(NVG_ANTIALIAS);

	handle = nvgCreateImage(vg, CA_PATH("cat.jpg"), 0);
	nvgImageSize(vg, handle, &_width, &_height);
	pattern = nvgImagePattern(vg, 100, 100, _width, _height, 0, handle, 1);
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

	{
		nvgBeginPath(vg);
		nvgRect(vg, 100, 100, _width, _height);
		nvgFillPaint(vg, pattern);
		nvgFill(vg);
	}

	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDeleteImage(vg, handle);
	nvgDelete(vg);
}
