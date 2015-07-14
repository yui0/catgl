//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_NANOVG
#define CATGL_IMPLEMENTATION
#include "catgl.h"

struct NVGcontext* vg;
int width, height;
float pixelRatio;

int c_action, c_x, c_y;
void mouseEvent(int button, int action, int x, int y)
{
	c_action = action;
	c_x = x;
	c_y = y;
}

void caInit(int w, int h)
{
	width = w;
	height = h;
	pixelRatio = (float)width / (float)height;

	caMouseEvent = mouseEvent;

	nvgCreateEx(vg, NVG_ANTIALIAS);
}

void caRender()
{
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	nvgBeginFrame(vg, width, height, pixelRatio);
	static int c;
	caDrawEyes(vg, 320, 180, 640, 360, c_x, c_y, c/10);
	c++;
	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
