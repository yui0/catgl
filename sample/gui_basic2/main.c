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

CATGL_UI win[] = {
	// Widgets
	{ CATGL_UI_WINDOW,      10, 100, 300, 360, "Title", 0, 0, 0,0,0,0 },
	{ CATGL_UI_SEARCHBOX,   10,  40, 280, 25, "Search", 0, 0, 0,0,0,0 },
	{ CATGL_UI_DROPDOWN,    10,  70, 280, 28, "Effects", 0, 0, 0,0,0,0 },
	// Form
	{ CATGL_UI_LABEL,       10, 110, 280, 20, "Login", 0, 0, 0,0,0,0 },
	{ CATGL_UI_EDITBOX,     10, 135, 280, 28, "Email", 0, 0, 0,0,0,0 },
	{ CATGL_UI_EDITBOX,     10, 170, 280, 28, "Password", 0, 0, 0,0,0,0 },
	{ CATGL_UI_CHECKBOX,    10, 200, 140, 28, "Remember me", 0, 0, 0,0,0,0 },
	{ CATGL_UI_BUTTON,     148, 200, 140, 28, "Sign in", 0, ICON_LOGIN, 0,96,128,255 },
	// Slider
	{ CATGL_UI_LABEL,       10, 245, 280, 20, "Diameter", 0, 0, 0,0,0,0 },
	{ CATGL_UI_EDITBOXNUM, 190, 270, 100, 28, "123.00", "px", 0, 0,0,0,0 },
	{ CATGL_UI_SLIDER,      10, 270, 170, 28, 0, 0, 0.4f, 0,0,0,0 },

	{ CATGL_UI_BUTTON,      10, 325, 160, 28, "Delete", 0, ICON_TRASH, 128,16,8,255 },
	{ CATGL_UI_BUTTON,     180, 325, 110, 28, "Cancel", 0, 0, 0,0,0,0 },
};

CATGL_UI confirm[] = {
	{ CATGL_UI_WINDOW, 350, 300, 200, 150, "Confirm", 0, 0, 0,0,0,0 },
	{ CATGL_UI_BUTTON, -1, -1, 180, 40, "OK!", 0, ICON_CIRCLED_CROSS, 0,96,128,255 },
	{ CATGL_UI_BUTTON, -1, -1, 180, 40, "CANCEL!", 0, ICON_CIRCLED_CROSS, 128,16,8,255 },
};

int c_action, c_x, c_y;
void mouseEvent(int button, int action, int x, int y)
{
//	LOGD("(%d,%d) %d\n", x, y, action);
	char s[256];
	sprintf(s, "(%d,%d) %d\n", x, y, action);
	nvgText(vg, 350, 170, s, NULL);

	c_action = action;
	c_x = x;
	c_y = y;
}

void caInit(int w, int h)
{
	width = w;
	height = h;

	// Calculate pixel ration for hi-dpi devices.
	pixelRatio = (float)width / (float)height;

	caMouseEvent = mouseEvent;
	caUiRegister(win, sizeof(win)/sizeof(CATGL_UI));
	caUiRegister(confirm, sizeof(confirm)/sizeof(CATGL_UI));

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

	caUiDraws(vg, c_action, c_x, c_y);

	nvgEndFrame(vg);
}
void caEnd()
{
	nvgDelete(vg);
}
