//---------------------------------------------------------
//	catgl
//
//		©2015,2018 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_NANOVG
#define CATGL_IMPLEMENTATION
#include "catgl.h"

struct NVGcontext* vg;
int width, height;
float pixelRatio;

void caInit(int w, int h)
{
	width = w;
	height = h;

	// Calculate pixel ration for hi-dpi devices.
	pixelRatio = (float)width / (float)height;

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

	// GUI
	int x = 10;
	int y = 100;

	// Widgets
	caDrawWindow(vg, "Title", x, y, 300, 360);
	caDrawSearchBox(vg, "Search", x+10, y+40, 280, 25);
	caDrawDropDown(vg, "Effects", x+10, y+70, 280, 28);

	// Form
	caDrawLabel(vg, "Login", x+10, y+110, 280, 20);
	caDrawEditBox(vg, "Email",  x+10, y+135, 280, 28);
	caDrawEditBox(vg, "Password", x+10, y+170, 280, 28);
	caDrawCheckBox(vg, "Remember me", x+10, y+200, 140, 28, 0);
	caDrawButton(vg, CATGL_ICON_LOGIN, "Sign in", x+148, y+200, 140, 28, nvgRGBA(0,96,128,255), 0);

	// Slider
	caDrawLabel(vg, "Diameter", x+10, y+245, 280, 20);
	caDrawEditBoxNum(vg, "123.00", "px", x+190, y+270, 100, 28);
	caDrawSlider(vg, 0.4f, x+10, y+270, 170, 28);

	caDrawButton(vg, CATGL_ICON_TRASH, "Delete", x+10, y+325, 160, 28, nvgRGBA(128,16,8,255), 0);
	caDrawButton(vg, 0, "Cancel", x+180, y+325, 110, 28, nvgRGBA(0,0,0,0), 0);

	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
