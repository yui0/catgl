void caDrawThumbnail(NVGcontext* vg, float x, float y, float w, float h, const int image/*, float t*/)
{
//	float cornerRadius = 3.0f;
	NVGpaint shadowPaint, imgPaint/*, fadePaint*/;
	float ix,iy,iw,ih;
	float thumb = 60.0f;
//	float arry = 30.5f;
	int imgw, imgh;
//	float stackh = (nimages/2) * (thumb+10) + 10;
//	int i;
//	float u = (1+cosf(t*0.5f))*0.5f;
//	float u2 = (1-cosf(t*0.2f))*0.5f;
//	float scrollh, dv;

	nvgSave(vg);
//	nvgScissor(vg, x,y,w,h);
//	nvgTranslate(vg, 0, -(stackh - h)*u);

//	dv = 1.0f / (float)(nimages-1);

	float tx, ty/*, v, a*/;
	tx = x+10;
	ty = y+10;
//	tx += (i%2) * (thumb+10);
//	ty += (i/2) * (thumb+10);
	nvgImageSize(vg, image, &imgw, &imgh);
	if (imgw < imgh) {
		iw = thumb;
		ih = iw * (float)imgh/(float)imgw;
		ix = 0;
		iy = -(ih-thumb)*0.5f;
	} else {
		ih = thumb;
		iw = ih * (float)imgw/(float)imgh;
		ix = -(iw-thumb)*0.5f;
		iy = 0;
	}

//	v = i * dv;
//	a = clampf((u2-v) / dv, 0, 1);

//	if (a < 1.0f)
//		drawSpinner(vg, tx+thumb/2,ty+thumb/2, thumb*0.25f, t);

	imgPaint = nvgImagePattern(vg, tx+ix, ty+iy, iw,ih, 0.0f/180.0f*NVG_PI, image, /*a*/1);
	nvgBeginPath(vg);
	nvgRoundedRect(vg, tx,ty, thumb,thumb, 5);
	nvgFillPaint(vg, imgPaint);
	nvgFill(vg);

	shadowPaint = nvgBoxGradient(vg, tx-1,ty, thumb+2,thumb+2, 5, 3, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	nvgBeginPath(vg);
	nvgRect(vg, tx-5,ty-5, thumb+10,thumb+10);
	nvgRoundedRect(vg, tx,ty, thumb,thumb, 6);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, tx+0.5f,ty+0.5f, thumb-1,thumb-1, 4-0.5f);
	nvgStrokeWidth(vg,1.0f);
	nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
	nvgStroke(vg);

	nvgRestore(vg);
}

