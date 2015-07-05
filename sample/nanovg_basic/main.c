//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

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

	nvgCreateFont(vg, "sans-bold", CATGL_ASSETS("Roboto-Bold.ttf"));
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

	/*nvgBeginFrame(vg, width, height, pxRatio);
	renderDemo(vg, 200,200, width,height,5,0,0);
	nvgEndFrame(vg);*/

	nvgBeginFrame(vg, width, height, pixelRatio);

/*	NVGpaint bg = nvgLinearGradient(vg, 0,0,0, height,nvgRGBA(0,0,255,255), nvgRGBA(255,0,0,255));
	nvgRect(vg, 0, 0, width, height);
	nvgFillPaint(vg, bg);
	nvgFill(vg);*/

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

	nvgBeginPath(vg);
	/* y は中心位置なことに注意。実際ここの部分は nvgTextBounds や nvgTextMetrics などを使って計算して求める必要があるだろう */
	//int x = 0;
	//int y = 10;
	nvgText(vg, /*x, y,*/100,100, "Hello!", NULL);
	nvgFill(vg);

	// GUI
	// Widgets
	caDrawWindow(vg, "Title", 10, 100, 100, 100);
	caDrawSearchBox(vg, "Search", 20, 120, 280, 25);
/*	y += 40;
	drawDropDown(vg, "Effects", x,y,280,28);
	popy = y + 14;
	y += 45;*/

	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
