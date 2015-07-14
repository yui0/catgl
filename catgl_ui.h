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
#define CATGL_UI_THUMBNAIL	9
#define CATGL_UI_WINDOW	10
#define CATGL_UI_EYES	100

typedef struct _CATGL_UI
{
	unsigned char type;
	//UIhandler handler;
	int x, y, w, h;
	char *s, *s2;
	int image;	// or icon
	int r, g, b, a;
} CATGL_UI;

#ifdef CATGL_IMPLEMENTATION
void caUiDraw(NVGcontext* vg, CATGL_UI *ui, int n, int action, int tx, int ty)
{
	static int t = 0; t++;
	int xpos = 0;
	int ypos = 0;
	int i, f;
	int nx = 0;
	int ny = 0;

	for (i=0; i<n; i++) {
		int x, y;
		if (ui->x >= 0) x = ui->x;
		else x = nx;
		if (ui->y >= 0) y = ui->y;
		else y = ny;
		nx = x/* + ui->w*/;
		ny = y + ui->h;
		x += xpos;
		y += ypos;

		f = 0;
		if (action==CATGL_ACTION_DOWN && tx >= x && tx <= x+ui->w && ty >= y && ty <= y+ui->h) f = 1;

		switch (ui->type) {
		case CATGL_UI_BUTTON:
			caDrawButton(vg, ui->image, ui->s, x, y, ui->w, ui->h, nvgRGBA(ui->r, ui->g, ui->b, ui->a), f);
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
			caDrawSlider(vg, (float)ui->image, x, y, ui->w, ui->h);
			break;
		case CATGL_UI_SPINNER:
//			caDrawSpinner(vg, x, y, ui->w/*r*/, /*ui->h*/1000.0/tx/*t*/);
			caDrawSpinner(vg, x, y, ui->w/*r*/, t);
			break;
		case CATGL_UI_THUMBNAIL:
			caDrawThumbnail(vg, x, y, ui->w, ui->h, ui->image);
			break;
		case CATGL_UI_WINDOW:
			caDrawWindow(vg, ui->s, ui->x, ui->y, ui->w, ui->h);
			xpos = x;
			ypos = y;
			nx = 0;
			ny = 40;
			break;
		case CATGL_UI_EYES:
			caDrawEyes(vg, x, y, ui->w, ui->h, tx, ty, 1/*t*/);
			break;
		}
		ui++;
	}
}

/*#define foreach(item, array) \
    for (int keep = 1, \
        count = 0,\
        size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++)  \
        for (item = (array) + count; keep; keep = !keep)

#define CATGL_SIZEOF_ARRAY(ary)	(sizeof(ary)/sizeof(ary[0]))
#define CATGL_ARRAY_END(ary)	(ary + (SIZEOF_ARRAY(ary)))*/

// Iterator
typedef struct _iterator {
	void* element;
	int n;
	struct _iterator* next;
} Iterator;
// Linked List
typedef struct _list {
	int element_size;
	struct _iterator* head;
	struct _iterator* tail;
} List;
#define xalloc(sz) calloc(1, sz)
#define list_alloc(list, type) \
    list = xalloc(sizeof(List)); \
    list->element_size = sizeof(type);
void list_add(List* list, void* new_element, int n)
{
	if (list->tail == NULL) {
		// 最初の要素
		list->tail = list->head = xalloc(sizeof(Iterator));
	} else {
		Iterator* old_tail = list->tail;
		old_tail->next = xalloc(sizeof(Iterator));
		list->tail = old_tail->next;
	}
	list->tail->element = new_element;
	list->tail->n = n;
}
void list_free(List* list)
{
	//Iterator* it = get_iterator(list);
	Iterator* it = list->head;
	while (1) {
		Iterator* old_it = it;
		if (it == NULL) break;
		free(it->element);
		it = it->next;
		free(old_it);
	}
}
//#define foreach(it, list) for(it = get_iterator(list); iterator_has_value(it); it = iterator_next(it))
#define list_new_element(list) xalloc(list->element_size)
#define foreach(it, list) for (it = list->head; it!=NULL; it = it->next)

List* list;
Iterator* it;
void caUiRegister(CATGL_UI *ui, int n)
{
	if (!list) list_alloc(list, CATGL_UI*);
	CATGL_UI **c = list_new_element(list);
	*c = ui;
	list_add(list, c, n);
}

void caUiDraws(NVGcontext* vg, int action, int tx, int ty)
{
	foreach (it, list) {
		CATGL_UI **c = it->element;
		caUiDraw(vg, *c, it->n, action, tx, ty);
	}
}
#endif
