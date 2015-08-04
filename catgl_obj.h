#ifndef CATGL_OBJ_H_
#define CATGL_OBJ_H_

/* モデルデータ */
typedef struct {
	float *vertices;
	int num_vertices;
} CATGL_MODEL;

int caLoadObj(CATGL_MODEL *m, char *file_name);
void caUnloadObj(CATGL_MODEL *m);


#ifdef CATGL_IMPLEMENTATION
//#include <stdio.h>
#include <stdlib.h>

/* 初期バッファサイズ */
#define DEF_BUF_SIZE 20

/* 浮動小数点数バッファ */
typedef struct {
	int buf_size;
	int current_index;
	float *buf;
} float_buffer;

/* 整数バッファ */
typedef struct {
	int buf_size;
	int current_index;
	int *buf;
} int_buffer;

/*------------------------------*
** 浮動小数点数バッファの割り当て
**------------------------------*/
float_buffer *alloc_float_buffer()
{
	float_buffer *fbuf;

	fbuf = malloc(sizeof(float_buffer));
	if (!fbuf) {
		fprintf(stderr, "Memory allocation error.\n");
		//exit(EXIT_FAILURE);
	}

	fbuf->buf_size = DEF_BUF_SIZE;
	fbuf->current_index = 0;
	fbuf->buf = malloc(sizeof(float) * fbuf->buf_size);

	return fbuf;
}

/*------------------------------*
** 浮動小数点数バッファの解放
**------------------------------*/
void free_float_buffer(float_buffer *fbuf)
{
	free(fbuf->buf);
	free(fbuf);
}

/*------------------------------*
** バッファに浮動小数点数を追加
**------------------------------*/
void add_float(float_buffer *fbuf, float value)
{
	fbuf->buf[fbuf->current_index] = value;
	fbuf->current_index++;
	if (fbuf->current_index >= fbuf->buf_size) {
		fbuf->buf_size *= 2;
		fbuf->buf = realloc(fbuf->buf, sizeof(float) * fbuf->buf_size);
		if (!fbuf->buf) {
			fprintf(stderr, "Memory allocation error.\n");
			//exit(EXIT_FAILURE);
		}
	}
}

/*------------------------------*
** バッファから浮動小数点数を取り出し
**------------------------------*/
float get_float(float_buffer *fbuf, int index)
{
	return fbuf->buf[index];
}

/*------------------------------*
** 整数バッファの割り当て
**------------------------------*/
int_buffer *alloc_int_buffer()
{
	int_buffer *ibuf;

	ibuf = malloc(sizeof(int_buffer));
	if (!ibuf) {
		fprintf(stderr, "Memory allocation error.\n");
		//exit(EXIT_FAILURE);
	}

	ibuf->buf_size = DEF_BUF_SIZE;
	ibuf->current_index = 0;
	ibuf->buf = malloc(sizeof(int) * ibuf->buf_size);

	return ibuf;
}

/*------------------------------*
** 整数バッファの解放
**------------------------------*/
void free_int_buffer(int_buffer *ibuf)
{
	free(ibuf->buf);
	free(ibuf);
}

/*------------------------------*
** バッファに整数を追加
**------------------------------*/
void add_int(int_buffer *ibuf, int value)
{
	ibuf->buf[ibuf->current_index] = value;
	ibuf->current_index++;
	if (ibuf->current_index >= ibuf->buf_size) {
		ibuf->buf_size *= 2;
		ibuf->buf = realloc(ibuf->buf, sizeof(int) * ibuf->buf_size);
		if (!ibuf->buf) {
			fprintf(stderr, "Memory allocation error.\n");
			//exit(EXIT_FAILURE);
		}
	}
}

/*------------------------------*
** バッファから整数を取り出し
**------------------------------*/
int get_int(int_buffer *ibuf, int index)
{
	return ibuf->buf[index];
}

/*------------------------------*
** 頂点座標の読み込み
**------------------------------*/
void read_vertices(const char *line, float_buffer *vs)
{
	float x, y, z;
	int count;

	count = sscanf(line, "%*s%f%f%f", &x, &y, &z);

	if (count == 3) {
		add_float(vs, x);
		add_float(vs, y);
		add_float(vs, z);
	}
}

/*------------------------------*
** インデックスの読み込み
**------------------------------*/
void read_indices(const char *line, int_buffer *fs)
{
	int count;
//	count = sscanf(line, "%*s%d%d%d%d%d%d%d%d%d%d", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &v[9]);

	unsigned short pp[54+1];
	switch (1) {
	case 1:
		count = sscanf(line, "f"" %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu",
			pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11, pp+12, pp+13, pp+14, pp+15, pp+16, pp+17);
		if (count>=3) break;

		count = sscanf(line, "f"" %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu"
			" %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu"
			" %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu",
			pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11,
			pp+12, pp+13, pp+14, pp+15, pp+16, pp+17, pp+18, pp+19, pp+20, pp+21, pp+22, pp+23,
			pp+24, pp+25, pp+26, pp+27, pp+28, pp+29, pp+30, pp+32, pp+32, pp+33, pp+34, pp+35, pp+36);
		if (count>=6) break;

		count = sscanf(line, "f"" %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu"
			" %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu"
			" %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu",
			pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11,
			pp+12, pp+13, pp+14, pp+15, pp+16, pp+17, pp+18, pp+19, pp+20, pp+21, pp+22, pp+23,
			pp+24, pp+25, pp+26, pp+27, pp+28, pp+29, pp+30, pp+32, pp+32, pp+33, pp+34, pp+35, pp+36);
		if (count>=6) break;

		count = sscanf(line, "f"" %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu"
			" %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu"
			" %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu",
			pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11, pp+12, pp+13, pp+14, pp+15, pp+16, pp+17,
			pp+18, pp+19, pp+20, pp+21, pp+22, pp+23, pp+24, pp+25, pp+26, pp+27, pp+28, pp+29, pp+30, pp+32, pp+32, pp+33, pp+34, pp+35,
			pp+36, pp+37, pp+38, pp+39, pp+40, pp+41, pp+42, pp+43, pp+44, pp+45, pp+46, pp+47, pp+48, pp+49, pp+50, pp+51, pp+52, pp+53, pp+54);
	}
	//printf("%d-%d ", count, pp[0]);

	switch (count) {
	case 3:
		add_int(fs, pp[0]);
		add_int(fs, pp[1]);
		add_int(fs, pp[2]);
		break;
	case 4:
		add_int(fs, pp[0]);
		add_int(fs, pp[1]);
		add_int(fs, pp[2]);
		add_int(fs, pp[0]);
		add_int(fs, pp[2]);
		add_int(fs, pp[3]);
		break;
	case 6:
		add_int(fs, pp[0]);
		add_int(fs, pp[2]);
		add_int(fs, pp[4]);
		break;
	case 8:
		add_int(fs, pp[0]);
		add_int(fs, pp[2]);
		add_int(fs, pp[4]);
		add_int(fs, pp[0]);
		add_int(fs, pp[4]);
		add_int(fs, pp[6]);
		break;
	}
}

/*------------------------------*
** モデルの作成
**------------------------------*/
void create_model(CATGL_MODEL *m, float_buffer *vs, int_buffer *fs)
{
	int i, j;

	m->num_vertices = fs->current_index * 3;
	m->vertices = malloc(sizeof(float) * m->num_vertices);

	if (!m->vertices) {
		fprintf(stderr, "Memory allocation error.\n");
		//exit(EXIT_FAILURE);
	}

	for (i = 0; i < fs->current_index; i++) {
		int idx = fs->buf[i] - 1;
		for (j = 0; j < 3; j++) {
			m->vertices[i * 3 + j] = vs->buf[idx * 3 + j];
		}
	}
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
void centre_and_rescale(float *verts, int numvertices)
{
	float cx=0.0f, cy=0.0f, cz=0.0f, scale=0.0f;
	float minx=0.0f, miny=0.0f, minz=0.0f;
	float maxx=0.0f, maxy=0.0f, maxz=0.0f;
	int i;
	float *v = verts;
	minx = maxx = verts[0];
	miny = maxy = verts[1];
	minz = maxz = verts[2];
	for (i=0; i<numvertices; i++) {
		float x = *v++;
		float y = *v++;
		float z = *v++;
		minx = MIN(minx, x);
		miny = MIN(miny, y);
		minz = MIN(minz, z);
		maxx = MAX(maxx, x);
		maxy = MAX(maxy, y);
		maxz = MAX(maxz, z);
		cx += x;
		cy += y;
		cz += z;
	}
	cx /= (float)numvertices;
	cy /= (float)numvertices;
	cz /= (float)numvertices;
	scale = 3.0f / (maxx-minx + maxy-miny + maxz-minz);
	v = verts;
	for (i=0; i<numvertices; i++) {
		*v = (*v-cx) * scale;
		v++;
		*v = (*v-cy) * scale;
		v++;
		*v = (*v-cz) * scale;
		v++;
	}
}
/*void renormalise(float *verts, int numvertices)
{
	int i;
	float *v = verts;
	for (i=0; i<numvertices; i++) {
		float x = v[0];
		float y = v[1];
		float z = v[2];
		float scale = 1.0f/sqrtf(x*x + y*y + z*z);
		*v++ = x * scale;
		*v++ = y * scale;
		*v++ = z * scale;
	}
}
void deindex(float *dst, const float *src, const unsigned short *indexes, GLsizei size, GLsizei count)
{
	int i;
	for (i=0; i<count; i++) {
		int ind = size * (indexes[0]-1);
		*dst++ = src[ind + 0];
		*dst++ = src[ind + 1];
		// todo: optimise - move out of loop
		if (size >= 3) {
			*dst++ = src[ind + 2];
		}
		indexes += 3;
	}
}*/

int caLoadObj(CATGL_MODEL *m, char *file_name)
{
	FILE *fp;
	char line[1024];
	float_buffer *vs;
	int_buffer *fs;

	fp = fopen(CATGL_ASSETS(file_name), "r");
	if (!fp) {
		fprintf(stderr, "Cannot open %s.\n", file_name);
		return 0;
	}

	vs = alloc_float_buffer();	// x,y,z and nx,ny,nz and ux,uy
	fs = alloc_int_buffer();	// index

	while (!feof(fp)) {
		fgets(line, sizeof(line), fp);
		if (line[0] == 'v' && (line[1] == ' ' || line[1] == '\t')) {
			read_vertices(line, vs);
		} else if (line[0] == 'f' && (line[1] == ' ' || line[1] == '\t')) {
			read_indices(line, fs);
		}
	}

	centre_and_rescale(vs->buf, vs->current_index/3);
	printf("vertices:%d\n", vs->current_index/3);
	create_model(m, vs, fs);
	//centre_and_rescale(m->vertices, m->num_vertices/3);
	printf("vertices:%d\n", m->num_vertices);

	free_float_buffer(vs);
	free_int_buffer(fs);

	fclose(fp);

	return 1;
}

void caUnloadObj(CATGL_MODEL *m)
{
	if (m->vertices) {
		free(m->vertices);
	}
}

#endif
#endif
