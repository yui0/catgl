void caDrawSpinner(NVGcontext* vg, float cx, float cy, float r, float t)
{
	float a0 = 0.0f + t*6;
	float a1 = NVG_PI + t*6;
	float r0 = r;
	float r1 = r * 0.75f;
	float ax,ay, bx,by;
	NVGpaint paint;

	nvgSave(vg);

	nvgBeginPath(vg);
	nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
	nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
	nvgClosePath(vg);
	ax = cx + cosf(a0) * (r0+r1)*0.5f;
	ay = cy + sinf(a0) * (r0+r1)*0.5f;
	bx = cx + cosf(a1) * (r0+r1)*0.5f;
	by = cy + sinf(a1) * (r0+r1)*0.5f;
	paint = nvgLinearGradient(vg, ax,ay, bx,by, nvgRGBA(0,0,0,0), nvgRGBA(0,0,0,128));
	nvgFillPaint(vg, paint);
	nvgFill(vg);

	nvgRestore(vg);
}
