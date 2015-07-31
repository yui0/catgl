void onOK(int action, int x, int y)
{
//	LOGD("(%d,%d) %d\n", x, y, action);
//	nvgBeginPath(vg);
	nvgText(vg, 350, 120, "Push OK button!", NULL);
//	nvgFill(vg);
}
void onCancel(int action, int x, int y)
{
//	LOGD("(%d,%d) %d\n", x, y, action);
//	nvgBeginPath(vg);
	nvgText(vg, 350, 120, "Push Cancel button!", NULL);
//	nvgFill(vg);
}
CATGL_UI confirm[] = {
	{ CATGL_UI_WINDOW,     350, 300, 200, 150, "Confirm", 0, 0, 0,0,0,0, 0 },
	{ CATGL_UI_LABEL,       -1,  -1, 180,  20, "これでよろしいですか？", 0, 0, 0,0,0,0, 0 },
	{ CATGL_UI_BUTTON,      -1,  -1, 180,  40, "OK!", 0, CATGL_ICON_HEART, 0,96,128,255, onOK },
	{ CATGL_UI_BUTTON,      -1,  -1, 180,  40, "CANCEL!", 0, CATGL_ICON_CIRCLED_CROSS, 128,16,8,255, onCancel },
};

