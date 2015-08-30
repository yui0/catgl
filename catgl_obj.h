#ifndef CATGL_OBJ_H_
#define CATGL_OBJ_H_

// モデルデータ
typedef struct {
	float *vertices;
	int num_vertices;//, num;
	unsigned char flag;
//	int *index;
//	int num_indexes;
} CATGL_MODEL;

int caLoadObj(CATGL_MODEL *m, char *file_name);
void caUnloadObj(CATGL_MODEL *m);


#ifdef CATGL_IMPLEMENTATION
//#include <stdio.h>
#include <stdlib.h>

// 初期バッファサイズ
#define DEF_BUF_SIZE 20

// 浮動小数点数バッファ
typedef struct {
	int buf_size;
	int current_index;
	float *buf;
} float_buffer;

// 整数バッファ
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
		LOGE("Memory allocation error.\n");
		exit(EXIT_FAILURE);
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
			LOGE("Memory allocation error.\n");
			exit(EXIT_FAILURE);
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
		LOGE("Memory allocation error.\n");
		exit(EXIT_FAILURE);
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
			LOGE("Memory allocation error.\n");
			exit(EXIT_FAILURE);
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
** UV座標の読み込み
**------------------------------*/
void read_uvs(const char *line, float_buffer *vts)
{
	float u, v;
	int count;

	count = sscanf(line, "%*s%f%f", &u, &v);

	if (count == 2) {
		add_float(vts, u);
		add_float(vts, v);
	}
}

/*------------------------------*
** インデックスの読み込み
**------------------------------*/
void read_indices(const char *line, int_buffer *f[3])
{
	int i, count;
	unsigned short pp[54+1];

	// f v1 v2 v3 ...
	count = sscanf(line, "%*s%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu%hu", pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11, pp+12, pp+13, pp+14, pp+15, pp+16, pp+17);
	if (count>=3) {
		if (count>3) count = (count-3)*3 +3;
		for (i=0; i<count; i++) {
			if (i%3) add_int(f[0], pp[i]);
			else add_int(f[0], pp[0]);
		}
		return;
	}

	// f v1//n1 v2//n2 v3//n3 ...
//	count = sscanf(line, "%*s %d%*[/]%d %d%*[/]%d %d%*[/]%d", &v1, &n1, &v2, &n2, &v3, &n3);
	count = sscanf(line, "f"" %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu"
		" %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu"
		" %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu//%hu %hu",
		pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11,
		pp+12, pp+13, pp+14, pp+15, pp+16, pp+17, pp+18, pp+19, pp+20, pp+21, pp+22, pp+23,
		pp+24, pp+25, pp+26, pp+27, pp+28, pp+29, pp+30, pp+32, pp+32, pp+33, pp+34, pp+35, pp+36);
	if (count>=6) {
		if (count>6) count = ((count/2-3)*3 +3)*2;
		for (i=0; i<count/2; i++) {
			add_int(f[0], pp[i*2]);
			add_int(f[1], pp[i*2+1]);
		}
		return;
	}

	// f v1/t1 v2/t2 v3/t3 ...
	count = sscanf(line, "f"" %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu"
		" %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu"
		" %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu/%hu %hu",
		pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11,
		pp+12, pp+13, pp+14, pp+15, pp+16, pp+17, pp+18, pp+19, pp+20, pp+21, pp+22, pp+23,
		pp+24, pp+25, pp+26, pp+27, pp+28, pp+29, pp+30, pp+32, pp+32, pp+33, pp+34, pp+35, pp+36);
	if (count>=6) {
		if (count>6) count = ((count/2-3)*3 +3)*2;
		for (i=0; i<count/2; i++) {
			add_int(f[0], pp[i*2]);
			add_int(f[2], pp[i*2+1]);
		}
		return;
	}

	// f v1//t1//n1 v2//t2//n2 v3//t3//n3 ...
	count = sscanf(line,
		"%*s %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu"
		" %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu"
		" %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu%*c%hu%*c%hu %hu",
		pp+ 0, pp+ 1, pp+ 2, pp+ 3, pp+ 4, pp+ 5, pp+ 6, pp+ 7, pp+ 8, pp+ 9, pp+10, pp+11, pp+12, pp+13, pp+14, pp+15, pp+16, pp+17,
			pp+18, pp+19, pp+20, pp+21, pp+22, pp+23, pp+24, pp+25, pp+26, pp+27, pp+28, pp+29, pp+30, pp+32, pp+32, pp+33, pp+34, pp+35,
			pp+36, pp+37, pp+38, pp+39, pp+40, pp+41, pp+42, pp+43, pp+44, pp+45, pp+46, pp+47, pp+48, pp+49, pp+50, pp+51, pp+52, pp+53, pp+54);
	if (count>=9) {
		if (count>9) count = ((count/3-3)*3 +3)*3;
		for (i=0; i<count/3; i++) {
			add_int(f[0], pp[i*3]);
			add_int(f[2], pp[i*3+1]);
			add_int(f[1], pp[i*3+2]);
		}
		return;
	}
}

/*------------------------------*
** モデルの作成
**------------------------------*/
void create_model(CATGL_MODEL *m, float_buffer *v[3], int_buffer *f[3])
{
	int len[] = { 3, 3, 2 };
	int i, j, n;

	m->num_vertices = f[0]->current_index;
//	m->num = f[0]->current_index *3 + f[1]->current_index *3 + f[2]->current_index *2;
//	m->vertices = malloc(sizeof(float) * m->num);
	int size = sizeof(float) * m->num_vertices * (3+3+2);
	m->vertices = malloc(size);
//	memset(m->vertices, 0, size);

	if (!m->vertices) {
		LOGE("Memory allocation error.\n");
		exit(EXIT_FAILURE);
	}

	m->flag = 0;
	if (v[1]->current_index) m->flag = 1;	// normal
	if (v[2]->current_index) m->flag |= 2;	// texture

	float *a = m->vertices;
	for (i=0; i < f[0]->current_index; i++) {
		for (n=0; n<3; n++) {
			if (!v[n]->current_index) continue;

			int idx = f[n]->buf[i] - 1;
			for (j=0; j<len[n]; j++) {
				*a++ = v[n]->buf[idx*3+j];
			}
		}
	}
}

inline void caVec3Add(float a[3], float p1[3], float p2[3])
{
	a[0] = p1[0] + p2[0];
	a[1] = p1[1] + p2[1];
	a[2] = p1[2] + p2[2];
}
inline void caVec3Sub(float a[3], float p1[3], float p2[3])
{
	a[0] = p1[0] - p2[0];
	a[1] = p1[1] - p2[1];
	a[2] = p1[2] - p2[2];
}
// 外積
inline void caVec3Cross(float *a, float *s1, float *s2)
{
	a[0] = s1[1]*s2[2] - s1[2]*s2[1];
	a[1] = s1[2]*s2[0] - s1[0]*s2[2];
	a[2] = s1[0]*s2[1] - s1[1]*s2[0];
}
// 正規化
inline void caVec3Normalize(float *v)
{
	float m=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (m > 0.0f) { m = 1.0f / m; } else { m = 0.0f; }
	v[0]*=m;
	v[1]*=m;
	v[2]*=m;
}
int caVec3CalculateNormal(float n[3], float p1[3], float p2[3], float p3[3])
{
	float v1[3], v2[3];

	caVec3Sub(v1, p1, p2);		// v1 = p1-p2
	caVec3Sub(v2, p3, p2);		// v2 = p3-p2
	caVec3Cross(n, v2, v1);		// n = v2×v1 (外積)
	caVec3Normalize(n);

	return 1;
}
void create_model_ex(CATGL_MODEL *m, float_buffer *v[3], int_buffer *f[3])
{
	int len[] = { 3, 3, 2 };
	int i, j, n;

	m->num_vertices = f[0]->current_index;
	int size = sizeof(float) * m->num_vertices * (3+3+2);
	m->vertices = malloc(size);

	if (!m->vertices) {
		LOGE("Memory allocation error.\n");
		exit(EXIT_FAILURE);
	}

	m->flag = 1;	// normal
	if (v[2]->current_index) m->flag |= 2;	// texture

	float *a = m->vertices;
	for (i=0; i < f[0]->current_index; i++) {
		for (n=0; n<3; n++) {
			if (!v[n]->current_index) {
				if (n==1) a += 3;
				continue;
			}

			int idx = f[n]->buf[i] - 1;
			for (j=0; j<len[n]; j++) {
				*a++ = v[n]->buf[idx*3+j];
			}
		}
		if ((i%3)==2 && !v[1]->current_index) {
			int stripe = 6;
			if (m->flag & 2) stripe += 2;
			float *aa = a - stripe*3;
			caVec3CalculateNormal(aa+3,          aa, aa+stripe, aa+stripe*2);
			caVec3CalculateNormal(aa+3+stripe,   aa, aa+stripe, aa+stripe*2);
			caVec3CalculateNormal(aa+3+stripe*2, aa, aa+stripe, aa+stripe*2);
			//LOGD("v: (%2.2f, %2.2f, %2.2f)\n", *aa, *(aa+1), *(aa+2));
			//LOGD("v: (%2.2f, %2.2f, %2.2f)\n", *(aa+stripe), *(aa+1+stripe), *(aa+2+stripe));
			//LOGD("Normal: (%2.2f, %2.2f, %2.2f)\n", *(aa+3), *(aa+3+stripe), *(aa+3+stripe*2));
		}
	}
}
void calculate_normal(float_buffer *v[3], int_buffer *f[3])
{
	if (v[1]->current_index) return;

	int i;
	for (i=0; i < f[0]->current_index; i++) {
		add_int(f[1], f[0]->buf[i]);
	}
	for (i=0; i < v[0]->current_index; i++) {
		add_float(v[1], 0);
	}
	LOGD("created normals:%d/%d\n", v[1]->current_index/3, f[1]->current_index);

	for (i=0; i < f[0]->current_index; i+=3) {
		float *p[3], *n[3], normal[3];
		p[0] = &v[0]->buf[(f[0]->buf[i  ]-1)*3];
		p[1] = &v[0]->buf[(f[0]->buf[i+1]-1)*3];
		p[2] = &v[0]->buf[(f[0]->buf[i+2]-1)*3];
		//LOGD("v: (%2.2f, %2.2f, %2.2f)\n", p[0][0], p[0][1], p[0][2]);

		n[0] = &v[1]->buf[(f[1]->buf[i  ]-1)*3];
		n[1] = &v[1]->buf[(f[1]->buf[i+1]-1)*3];
		n[2] = &v[1]->buf[(f[1]->buf[i+2]-1)*3];
		//LOGD("v: (%2.2f, %2.2f, %2.2f)\n", n[0][0], n[0][1], n[0][2]);

		caVec3CalculateNormal(normal, p[0], p[1], p[2]);
#if 0
		memcpy(n[0], normal, sizeof(float)*3);
		memcpy(n[1], normal, sizeof(float)*3);
		memcpy(n[2], normal, sizeof(float)*3);
#else
		caVec3Add(n[0], n[0], normal);
		caVec3Add(n[1], n[1], normal);
		caVec3Add(n[2], n[2], normal);
#endif
	}
	for (i=0; i < v[1]->current_index/3; i++) {
		caVec3Normalize(&v[1]->buf[i*3]);
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
	float_buffer *v[3];
	int_buffer *f[3];

	fp = fopen(CATGL_ASSETS(file_name), "r");
	if (!fp) {
		LOGE("Cannot open %s.\n", file_name);
		return 0;
	}

	v[0] = alloc_float_buffer();	// x,y,z
	v[1] = alloc_float_buffer();	// nx,ny,nz
	v[2] = alloc_float_buffer();	// ux,uy
	f[0] = alloc_int_buffer();		// index
	f[1] = alloc_int_buffer();		// index for n
	f[2] = alloc_int_buffer();		// index for t

	while (!feof(fp)) {
		fgets(line, sizeof(line), fp);
		if (line[0]=='v' && (line[1]==' ' || line[1]=='\t')) {
			read_vertices(line, v[0]);
		} else if (line[0]=='v' && line[1]=='n' && (line[2]==' ' || line[2]=='\t')) {
			read_vertices(line, v[1]);
		} else if (line[0]=='v' && line[1]=='t' && (line[2]==' ' || line[2]=='\t')) {
			read_uvs(line, v[2]);
		} else if (line[0]=='f' && (line[1]==' ' || line[1]=='\t')) {
			read_indices(line, f);
		}
	}

	centre_and_rescale(v[0]->buf, v[0]->current_index/3);
	LOGD("vertices:%d/%d\n", v[0]->current_index/3, f[0]->current_index);
	LOGD("normals:%d/%d\n", v[1]->current_index/3, f[1]->current_index);
	LOGD("uvs:%d/%d\n", v[2]->current_index/2, f[2]->current_index);
	calculate_normal(v, f);
	create_model(m, v, f);
	//create_model_ex(m, v, f);
	LOGD("vertices:%d\n", m->num_vertices);
	//LOGD("(%f,%f,%f)-(%f,%f,%f)-(%f,%f)", m->vertices[0], m->vertices[1], m->vertices[2], m->vertices[3], m->vertices[4], m->vertices[5], m->vertices[6], m->vertices[7]);

	free_float_buffer(v[0]);
	free_float_buffer(v[1]);
	free_float_buffer(v[2]);
	free_int_buffer(f[0]);
	free_int_buffer(f[1]);
	free_int_buffer(f[2]);

	fclose(fp);

	return 1;
}

void caUnloadObj(CATGL_MODEL *m)
{
	if (m->vertices) {
		free(m->vertices);
		m->vertices = 0;
	}
	m->num_vertices = /*m->num =*/ 0;
}

#undef DEF_BUF_SIZE

#endif
#endif
