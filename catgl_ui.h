//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_UI_BUTTON	1
#define CATGL_UI_CHECKBOX	2
#define CATGL_UI_DROPDOWN	3
#define CATGL_UI_EDITBOX	4
#define CATGL_UI_LABEL	5
#define CATGL_UI_SEARCHBOX	6
#define CATGL_UI_SLIDER	7
#define CATGL_UI_SPINNER	8
#define CATGL_UI_WINDOW	9
#define CATGL_UI_EYES	100

typedef struct _CATGL_UI
{
	unsigned char type;
	//UIhandler handler;
	int x, y, w, h;
	char *s, *s2;
	int icon;
	int r, g, b, a;
} CATGL_UI;

#ifdef CATGL_IMPLEMENTATION
void caUiDraw(NVGcontext* vg, CATGL_UI *ui, int n, int action, int tx, int ty)
{
	static float t = 0; t++;
	int xpos = 0;
	int ypos = 0;
	int i, f;
	int nx = 0;
	int ny = 0;

	for (i=0; i<n; i++) {
		int x = xpos;
		int y = ypos;
		if (ui->x >= 0) x += ui->x;
		else x += nx;
		if (ui->y >= 0) y += ui->y;
		else y += ny;
		nx = ui->x/* + ui->w*/;
		ny = ui->y + ui->h;

		f = 0;
		if (action==CATGL_ACTION_DOWN && tx >= x && tx <= x+ui->w && ty >= y && ty <= y+ui->h) f = 1;

		switch (ui->type) {
		case CATGL_UI_BUTTON:
			caDrawButton(vg, ui->icon, ui->s, x, y, ui->w, ui->h, nvgRGBA(ui->r, ui->g, ui->b, ui->a), f);
			break;
		case CATGL_UI_CHECKBOX:
			caDrawCheckBox(vg, ui->s, x, y, ui->w, ui->h);
			break;
		case CATGL_UI_DROPDOWN:
			caDrawDropDown(vg, ui->s, x, y, ui->w, ui->h);
			break;
		case CATGL_UI_EDITBOX:
			caDrawEditBox(vg, ui->s,  x, y, ui->w, ui->h);
			break;
		case CATGL_UI_LABEL:
			caDrawLabel(vg, ui->s, x, y, ui->w, ui->h);
			break;
		case CATGL_UI_SEARCHBOX:
			caDrawSearchBox(vg, ui->s, x, y, ui->w, ui->h);
			break;
		case CATGL_UI_SLIDER:
			caDrawSlider(vg, (float)ui->icon, x, y, ui->w, ui->h);
			break;
		case CATGL_UI_SPINNER:
//			caDrawSpinner(vg, x, y, ui->w/*r*/, /*ui->h*/1000.0/tx/*t*/);
			caDrawSpinner(vg, x, y, ui->w/*r*/, t);
			break;
		case CATGL_UI_WINDOW:
			caDrawWindow(vg, ui->s, ui->x, ui->y, ui->w, ui->h);
			xpos = x;
			ypos = y;
			break;
		case CATGL_UI_EYES:
			caDrawEyes(vg, x, y, ui->w, ui->h, tx, ty, 1/*t*/);
			break;
		}
		ui++;
	}
}

#if 0
void caUiProsess(NVGcontext* vg, CATGL_UI *ui, int n, int tx, int ty)
{
	int xpos = 0;
	int ypos = 0;

	for (int i=0; i<n; i++) {
		int x = xpos + ui->x;
		int y = ypos + ui->y;

		if (tx >= x && tx <= x+ui->w && ty >= y && ty <= y+ui->h) {
			switch (ui->type) {
			case CATGL_UI_BUTTON:
				caDrawButton(vg, ui->icon, ui->s, x, y, ui->w, ui->h, nvgRGBA(ui->r, ui->g, ui->b, ui->a), 1);
				break;
			/*case CATGL_UI_CHECKBOX:
				caDrawCheckBox(vg, ui->s, x, y, ui->w, ui->h);
				break;
			case CATGL_UI_DROPDOWN:
				caDrawDropDown(vg, ui->s, x, y, ui->w, ui->h);
				break;
			case CATGL_UI_EDITBOX:
				caDrawEditBox(vg, ui->s,  x, y, ui->w, ui->h);
				break;
			case CATGL_UI_LABEL:
				caDrawLabel(vg, ui->s, x, y, ui->w, ui->h);
				break;
			case CATGL_UI_SEARCHBOX:
				caDrawSearchBox(vg, ui->s, x, y, ui->w, ui->h);
				break;
			case CATGL_UI_SLIDER:
				caDrawSlider(vg, (float)ui->icon, x, y, ui->w, ui->h);
				break;
			case CATGL_UI_WINDOW:
				caDrawWindow(vg, ui->s, ui->x, ui->y, ui->w, ui->h);
				xpos = x;
				ypos = y;*/
			}
		}

		if (ui->type == CATGL_UI_WINDOW) {
			xpos = x;
			ypos = y;
		}
		ui++;
	}
}
#endif
#endif
