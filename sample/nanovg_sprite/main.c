//---------------------------------------------------------
//	catgl
//
//		©2018 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_NANOVG
#define CATGL_IMPLEMENTATION
#include "catgl.h"

NVGcontext* vg;
int width, height;
float pixelRatio;

CATGL_SPRITE s;

#define PAI 3.14159265358979323846
double cos_fast(double x)
{
	static double div[5] = {1.0/(3*4*5*6*7*8*9*10), -1.0/(3*4*5*6*7*8), 1.0/(3*4*5*6), -1.0/(3*4), 1.0};
	double y, *p = div;
	//int q = (int)(x/(2.0*PAI));
	//x = x-q*(2.0*PAI);
	x = x/32.0;
	x = x*x;
	y = -1.0/(3*4*5*6*7*8*9*10*11*12);
	do {
		y = y*x+(*p);
		p++;
	} while (p<div+5);
	y = y*x;
	for (int i=0; i<5; i++) y = y*(4.0-y);
	return 1.0-y/2.0;
}
double sin_fast(double r) {return cos_fast(r-PAI/2);}
double tan_fast(double r) {return cos_fast(r-PAI/2)/cos_fast(r);}
#undef PAI

// 表示の初期化
void caInit(int w, int h)
{
	width = w;
	height = h;

	// Calculate pixel ration for hi-dpi devices.
	pixelRatio = (float)width / (float)height;

	vg = nvgCreate(NVG_ANTIALIAS);
	caSpriteLoad(&s, "cat.jpg", vg);
}

// 描画
void caRender()
{
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	static int x = 0; x++; x = x>width ? 0: x;
	nvgBeginFrame(vg, width, height, pixelRatio);
	s.x = x;
	s.y = sin_fast(x/10.)*height/2+height/2;
	caSpriteRender(&s);
	nvgEndFrame(vg);
}

void caEnd()
{
	caSpriteDelete(&s);
	nvgDelete(vg);
}
