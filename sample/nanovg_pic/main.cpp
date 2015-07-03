//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include "catgl.h"
#include "catgl_nanovg.h"

struct Image {
	Image(NVGcontext *c)
		: context2D(c),
		  handle(0),
		  width(0),
		  height(0)
	{
	}
	~Image() {
		nvgDeleteImage(context2D, handle);
		handle = 0;
		context2D = 0;
	}
	void load(const char *filename) {
		handle = nvgCreateImage(context2D, filename, 0);
		nvgImageSize(context2D, handle, &width, &height);
		pattern = nvgImagePattern(context2D, 0, 0, width, height, 0, handle, 1);
	}
	void render() {
		nvgBeginPath(context2D);
		nvgRect(context2D, 0, 0, width, height);
		nvgFillPaint(context2D, pattern);
		nvgFill(context2D);
	}
	NVGcontext *context2D;
	NVGpaint pattern;
	int handle;
	int width;
	int height;
};

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

	nvgBeginFrame(vg, width, height, pixelRatio);

	Image i(vg);
	i.load("assets/cat.jpg");
	i.render();

	nvgEndFrame(vg);
}

void caEnd()
{
	nvgDelete(vg);
}
