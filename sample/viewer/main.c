//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <math.h>

#define CATGL_IMPLEMENTATION
#include "catgl.h"

/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"
	"uniform mat4 modelviewMatrix;"
	"attribute vec3 position;"		// in
	"void main() {"
	"   gl_Position = projectionMatrix * modelviewMatrix * vec4(position, 1.0);"
	"}";
char fsrc[] =
	"#version 120\n"
	"void main() {"
	"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
	"}";*/

// Lighting 1
/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// param
	"uniform mat4 modelviewMatrix;"	// param
	"attribute vec3 position;"		// in
	"attribute vec3 normal;"		// in
	"varying vec3 color;"		// out
	"void main() {"
	"  vec3 light = normalize(vec3(4.0, 4.0, 4.0));"
	"  color = vec3(1.0, 1.0, 1.0) * clamp(dot(normal, light), 0, 1);"
	"  gl_Position = projectionMatrix * modelviewMatrix * vec4(position, 1.0);"
	"}";*/
// Lighting 2
/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// param
	"uniform mat4 viewMatrix;"		// param
	"uniform mat4 modelviewMatrix;"	// param
	"attribute vec3 position;"		// in
	"attribute vec3 normal;"		// in
	"varying vec3 color;"		// out
	"void main() {"
	"  vec4 n = normalize(modelviewMatrix * viewMatrix * vec4(normal, 1));"	// 法線 (!!本当は1ではなく0)
	"  vec4 light = normalize(viewMatrix * vec4(4.0, 4.0, 4.0, 1.0));"		// 光
	"  vec4 pos = viewMatrix * modelviewMatrix * vec4(position, 1.0);"		// ワールド座標位置
	"  vec4 eye = normalize(vec4(0, 0, 0, 0) - pos);"				// 視線ベクトル
	"  float cosa = clamp(dot(eye, reflect(-light, n)), 0, 1);"			// 反射ベクトル
	"  color = vec3(0.1,0.1,0.1)"							// 環境光
	"    + vec3(1.0, 1.0, 1.0) * clamp(dot(n, light), 0, 1)"			// 拡散光
	"    + vec3(1.0, 1.0, 1.0) * pow(cosa, 5);"					// 反射光
	"  gl_Position = projectionMatrix * pos;"
	"}";*/
// Lighting 3 (環境光+拡散光)
/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// 錐台(frustum)変換行列
	"uniform mat4 viewMatrix;"		// param
	"uniform mat4 modelviewMatrix;"	// 平行回転行列
	"attribute vec3 position;"		// 頂点座標
	"attribute vec3 normal;"		// 頂点の法線ベクトル
	"varying vec3 color;"		// out
	"void main() {"
	"  vec4 n = normalize(modelviewMatrix * viewMatrix * vec4(normal, 0));"	// 法線
	"  vec4 light = normalize(viewMatrix * vec4(4.0, 4.0, 4.0, 1.0));"		// 光
	"  vec4 pos = viewMatrix * modelviewMatrix * vec4(position, 1.0);"		// ワールド座標位置
	"  vec4 eye = normalize(vec4(0, 0, 0, 0) - pos);"				// 視線ベクトル
	"  float cosa = clamp(dot(eye, reflect(-light, n)), 0, 1);"			// 反射ベクトル
	"  color = vec3(0.1, 0.1, 0.1)"							// 環境光
	"    + vec3(1.0, 1.0, 1.0) * clamp(dot(n, light), 0, 1);"			// 拡散光
	"  gl_Position = projectionMatrix * pos;"
	"}";*/
// Lighting 4 (環境光+拡散光+反射光)
/*char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// 錐台(frustum)変換行列
	"uniform mat4 viewMatrix;"		// param
	"uniform mat4 modelviewMatrix;"	// 平行回転行列
	"attribute vec3 position;"		// 頂点座標
	"attribute vec3 normal;"		// 頂点の法線ベクトル
	"varying vec3 color;"		// out
//	"attribute vec2 texcoord;"		// in
//	"varying vec2 texcoordVarying;"	// out
	"void main() {"
	"  vec4 n = normalize(modelviewMatrix * viewMatrix * vec4(normal, 0));"	// 法線
	"  vec4 light = normalize(viewMatrix * vec4(4.0, 4.0, 4.0, 1.0));"		// 光
	"  vec4 pos = viewMatrix * modelviewMatrix * vec4(position, 1.0);"		// ワールド座標位置
	"  vec4 eye = normalize(vec4(0, 0, 0, 0) - pos);"				// 視線ベクトル
	"  float cosa = clamp(dot(eye, reflect(-light, n)), 0, 1);"			// 反射ベクトル
	"  color = vec3(0.1, 0.1, 0.1)"							// 環境光
	"    + vec3(1.0, 1.0, 1.0) * clamp(dot(n, light), 0, 1)"			// 拡散光
	"    + vec3(1.0, 1.0, 1.0) * pow(cosa, 5);"					// 反射光
	"  gl_Position = projectionMatrix * pos;"
//	"  texcoordVarying = texcoord;"
	"}";*/
// Lighting 5
char vsrc[] =
	"#version 120\n"
	"uniform mat4 projectionMatrix;"	// 錐台(frustum)変換行列
	"uniform mat4 viewMatrix;"		// param
	"uniform mat4 modelviewMatrix;"	// 平行回転行列
	"attribute vec3 position;"		// 頂点座標
	"attribute vec3 normal;"		// 頂点の法線ベクトル
	"varying vec3 color;"		// out
	"void main() {"
	"  vec4 pos = viewMatrix * modelviewMatrix * vec4(position, 1.0);"		// ワールド座標位置
	"  gl_Position = projectionMatrix * pos;"
	"  vec4 n = normalize(modelviewMatrix * viewMatrix * vec4(normal, 0));"	// 法線ベクトルを正規化(＝長さを1にする)
	"  float l = abs(dot(n, normalize(vec4(-1, 1, 1, 0))));"			// 法線ベクトルと光ベクトル-1, 1, 1)との内積で光量を決定
	"  color = vec3(l, l, l);"
	"}";
char fsrc[] =
	"#version 120\n"
	"varying vec3 color;"		// in
	"void main() {"
	"  gl_FragColor = vec4(color, 1.0);"
	"}";

GLuint program;
GLuint vbo[2];
CATGL_MODEL m;

float aspect;
float x_angle, y_angle, z_angle;

void load_obj(GLuint vbo[], CATGL_MODEL *m, char *file)
{
	caLoadObj(m, file);

	int len = 1, size = 3;
	GLuint d[6];
	d[0] = glGetAttribLocation(program, "position");
	d[1] = 3;
	if (m->flag & 1) {
		d[2] = glGetAttribLocation(program, "normal");
		d[3] = 3;
		len++;
		size += 3;
	}
	if (m->flag & 2) {
		d[4] = glGetAttribLocation(program, "tex");
		//d[4] = -1;
		d[5] = 2;
		len++;
		size += 2;
	}
	vbo[0] = caCreateObject_(m->vertices, sizeof(float)*size, m->num_vertices*3, d, len);
}

#include <dirent.h>
char *getObjFile(int c)
{
	char *path = "./";
	char *ext = ".obj";

	static char s[BUFSIZ];
	int n = -1, e = strlen(ext);

	//printf("+%s\n", path);
	DIR *dir = opendir(CATGL_ASSETS(path));
	struct dirent *d;
	while ((d = readdir(dir)) != NULL) {
		char *p = strchr(d->d_name, 0);
		//printf("-%s,%s\n", d->d_name, p-e);
		if (strstr(p-e, ext)) {
			n++;
			if (n==c) {
				strncpy(s, d->d_name, BUFSIZ);
				break;
			}
		}
	}
	closedir(dir);

	if (n==c) return s;
	return 0;
}

void keyEvent(int key, int action)
{
	static int obj;
	char *s = 0;

	if (action == CATGL_ACTION_DOWN) {
		switch (key) {
		case CATGL_KEY_P:
			caMode = CATGL_MODE_POINT;
			break;
		case CATGL_KEY_W:
			caMode = CATGL_MODE_LINE;
			break;
		case CATGL_KEY_T:
			caMode = CATGL_MODE_TRIANGLES;
			break;
		case CATGL_KEY_RIGHT:
			obj++;
			s = getObjFile(obj);
			if (!s) {
				obj = 0;
				s = getObjFile(obj);
			}
			break;
		case CATGL_KEY_LEFT:
			obj--;
			if (obj<0) obj = 0;
			s = getObjFile(obj);
			break;
		case CATGL_KEY_C:
		{
			// Toggle front, back, and no culling
			int cullMode;
			glGetIntegerv(GL_CULL_FACE_MODE, &cullMode);
			if (glIsEnabled(GL_CULL_FACE)) {
				if (cullMode == GL_FRONT) {
					glCullFace(GL_BACK);
					LOGD("Culling: Culling back faces; drawing front faces\n");
				} else {
					glDisable(GL_CULL_FACE);
					LOGD("Culling: No culling; drawing all faces.\n");
				}
			} else {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				LOGD("Culling: Culling front faces; drawing back faces\n");
			}
			break;
		}
		case CATGL_KEY_PLUS: // increase size of points and width of lines
		{
			GLfloat currentPtSize;
			GLfloat sizeRange[2];
			glGetFloatv(GL_POINT_SIZE, &currentPtSize);
			glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, sizeRange);
			GLfloat temp = currentPtSize+1;
			if (temp > sizeRange[1]) temp = sizeRange[1];
			glPointSize(temp);
			LOGD("Point size is %f (can be between %f and %f)\n", temp, sizeRange[0], sizeRange[1]);

			GLfloat currentLineWidth;
			GLfloat widthRange[2];
			glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);
			glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, widthRange);
			temp = currentLineWidth+1;
			if (temp > widthRange[1]) temp = widthRange[1];
			glLineWidth(temp);
			LOGD("Line width is %f (can be between %f and %f)\n", temp, widthRange[0], widthRange[1]);
			break;
		}
		case CATGL_KEY_MINUS: // decrease size of points and width of lines
		{
			GLfloat currentPtSize;
			GLfloat sizeRange[2];
			glGetFloatv(GL_POINT_SIZE, &currentPtSize);
			glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, sizeRange);
			GLfloat temp = currentPtSize-1;
			if (temp < sizeRange[0]) temp = sizeRange[0];
			glPointSize(temp);
			LOGD("Point size is %f (can be between %f and %f)\n", temp, sizeRange[0], sizeRange[1]);

			GLfloat currentLineWidth;
			GLfloat widthRange[2];
			glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);
			glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, widthRange);
			temp = currentLineWidth-1;
			if (temp < widthRange[0]) temp = widthRange[0];
			glLineWidth(temp);
			LOGD("Line width is %f (can be between %f and %f)\n", temp, widthRange[0], widthRange[1]);
			break;
		}
		}
	}

	if (s) {
		printf("-- %s\n", s);
		glDeleteBuffers(1, vbo);
		caUnloadObj(&m);

		load_obj(vbo, &m, s);
	}
}
void mouseEvent(int button, int action, int x, int y)
{
	static int lx, ly;

	//keyEvent(CATGL_KEY_RIGHT, action);
	//LOGD("(%d,%d)\n", x, y);
	if (action == CATGL_ACTION_DOWN) {
		lx = x;
		ly = y;
	} else if (action == CATGL_ACTION_MOVE && lx>0) {
		x_angle += (y-ly)/8;
		y_angle += (x-lx)/8;
		//LOGD("%f (%d)\n", x_angle, (x-lx));
	} else {
		lx = 0;
	}
}

void caInit(int width, int height)
{
	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	load_obj(vbo, &m, "teapot.obj");
//	load_obj(vbo, &m, "elephal.obj");

	aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glClearColor(0.6, 0.8, 0.8, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	// カリングの有効化

//	glFrontFace(GL_CCW);	// 裏面を描画しない
//	glCullFace(GL_BACK);

	caKeyEvent = keyEvent;
	caMouseEvent = mouseEvent;
}

void caRender()
{
	float mat[16], r[4][16];

	// 透視投影変換行列
	caMakeProjectionMatrix(mat, 1, 1000, 53, aspect);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, mat);

	caMakeUnit(mat);
	caSetPosition(mat, 0, 0, -2);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, mat);

	caMakeUnit(r[0]);
	caRotationX(r[0], x_angle);
	caMakeUnit(r[1]);
	caRotationY(r[1], y_angle);
	caMakeUnit(r[2]);
	caRotationZ(r[2], z_angle);
	caMultMatrix(r[0], r[1], r[3]);
	caMultMatrix(r[3], r[2], mat);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_FALSE, mat);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	caDrawObject(vbo, &m);
}

void caEnd()
{
	glDeleteBuffers(1, vbo);
	caUnloadObj(&m);
}
