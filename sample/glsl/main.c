//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

//#define CATGL_NANOVG
#define CATGL_IMPLEMENTATION
#include "catgl.h"

#include <time.h>
//clock_t start;
long long start;

float mx, my, width, height, pixelRatio;
float x_angle, y_angle, z_angle;

GLuint program;
GLuint vao;

char *name = "Hello! こんにちは！";
#ifdef CATGL_NANOVG
struct NVGcontext* vg;
#endif

char vsrc[] =
	"#version 120\n"
	"attribute vec2 position;"
	"void main() {"
	"   gl_Position = vec4(position, 0.0, 1.0);"
	"}";
// グラディーション
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(pos, 0.0, 1.0);"
	"}";*/
// プラズマ
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"float plasma(vec2 p) {"
	"  p*=10.0;"
	"  return (sin(p.x)*0.25+0.25)+(sin(p.y)*0.25+0.25);"
	"}"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(plasma(pos));"
	"}";*/
// 同心円状のリング
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"float rings(vec2 p) {"
	"  return sin(length(p)*16.0);"
	"}"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(rings(pos));"
	"}";*/
// 同心円状のリング改
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"float lengthN(vec2 v, float n) {"
	"  vec2 tmp = pow(abs(v), vec2(n));"
	"  return pow(tmp.x+tmp.y, 1.0/n);"
	"}"
	"float rings(vec2 p) {"
	"  return sin(lengthN(p, 4.0)*16.0);"
	"}"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(rings(pos));"
	"}";*/
// 回回回回回回
// 回（＠ω＠）回ｸﾞﾙｸﾞﾙ・・・
// 回回回回回回
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"float lengthN(vec2 v, float n) {"
	"  vec2 tmp = pow(abs(v), vec2(n));"
	"  return pow(tmp.x+tmp.y, 1.0/n);"
	"}"
	"float rings(vec2 p) {"
	"  vec2 p2 = mod(p*8.0, 4.0)-2.0;"
	"  return sin(lengthN(p2, 4.0)*16.0);"
	"}"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(rings(pos));"
	"}";*/
// xor演算子[GLSL 1.3]
/*char fsrc[] =
	"#version 130\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"float xor(vec2 p) {"
	"  ivec2 ip = ivec2(fract(p)*256.0);"
	"  return float(ip.x^ip.y)/256.0;"
	"}"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(xor(pos));"
	"}";*/
// 擬似乱数
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"vec2 rand(vec2 pos) {"
	"  return fract(pow(pos+2.0, pos.yx+2.0)*22222.0);"
	"}"
//	"vec2 rand2(vec2 pos) {"
//	"  return rand(rand(pos));"
//	"}"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  gl_FragColor = vec4(rand(pos).xyxy);"
	"}";*/
// パーリンノイズ(Perlin noise)
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"vec2 rand(vec2 pos) {"
	"  return fract(pow(pos+2.0, pos.yx+2.0)*22222.0);"
	"}"
	"vec2 rand2(vec2 pos) {"
	"  return rand(rand(pos));"
	"}"
	"float softnoise(vec2 pos, float scale) {"
	"  vec2 smplpos = pos*scale;"
	"  float c0 = rand2((floor(smplpos)+vec2(0.0,0.0))/scale).x;"
	"  float c1 = rand2((floor(smplpos)+vec2(1.0,0.0))/scale).x;"
	"  float c2 = rand2((floor(smplpos)+vec2(0.0,1.0))/scale).x;"
	"  float c3 = rand2((floor(smplpos)+vec2(1.0,1.0))/scale).x;"
	"  vec2 a = fract(smplpos);"
	"  return"	// mix(mix(c0, c1, a.x), mix(c2, c3, a.x), a.y);
	"  mix("
	"    mix(c0, c1, smoothstep(0.0, 1.0, a.x)),"
	"    mix(c2, c3, smoothstep(0.0, 1.0, a.x)),"
	"    smoothstep(0.0, 1.0, a.y)"
	"  );"
	"}"
	"void main() {"
	"  vec2 pos = gl_FragCoord.xy / resolution.y;"
	"  float color = 0.0;"
	"  float s = 1.0;"
	"  for (int i=0; i<6; ++i) {"
	"    color += softnoise(pos+vec2(i*0.02), s*4.0)/s/2.0;"
	"    s*=2.0;"
	"  }"
	"  gl_FragColor = vec4(color);"
	"}";*/
// 回る円
char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  float theta = time*3.0;"
	"  vec2 ballPos = vec2(cos(theta), sin(theta))*0.5;"
	"  vec2 texPos = vec2(gl_FragCoord.xy/resolution);"
	"  if (distance(pos, ballPos) < 0.3) {"
	"    gl_FragColor = vec4(1.0);"
	"  } else {"
	"    gl_FragColor = vec4(0.0);"
	"  }"
	"}";
// 回る円(残像)
/*char fsrc[] =
	"#version 120\n"
	"uniform vec2 resolution;"
	"uniform float time;"
	"uniform sampler2D backbuffer;"
	"void main() {"
	"  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;"
	"  float theta = time*3.0;"
	"  vec2 ballPos = vec2(cos(theta), sin(theta))*0.5;"
	"  vec2 texPos = vec2(gl_FragCoord.xy/resolution);"
	"  if (distance(pos, ballPos) < 0.3) {"
	"    gl_FragColor = vec4(1.0);"
	"  } else {"
	"    gl_FragColor = texture2D(backbuffer, texPos)*0.95;"
	"  }"
	"}";*/
// 複数個の円
/*char fsrc[] =
	"#version 120\n"
	"uniform float time;"
	"uniform vec2 mouse;"
	"uniform vec2 resolution;"
	"uniform sampler2D backbuffer;"
	"void main() {"
	"  vec2 p = gl_FragCoord.xy / resolution.xy;"
	"  const int nr_light = 36;"
	"  float b = 0.0;"
	"  for (int i=0; i<nr_light; ++i) {"
	"    float t = time;"
	"    float r = t+ 3.14159265358979*2.0* float(i) /float(nr_light);"
	"    vec2 lp = mouse + 0.5*cos(t)*(abs(cos(t+float(i)))*0.3+0.7)*vec2(cos(r), sin(r));"
	"    float d = pow(1.0 / (distance(p, lp)+1.0), 60.0);"
	"    b += d;"
	"  }"
	"  gl_FragColor = vec4(vec3(clamp(b,0.0,1.0)), 1);"
	"}";*/
// 複数個の円
/*char fsrc[] =
	"#version 120\n"
	"uniform float time;"
	"uniform vec2 mouse;"
	"uniform vec2 resolution;"
	"uniform sampler2D backbuffer;"
	"void main() {"
	"  vec2 p = gl_FragCoord.xy / resolution.xy;"
	"  p = mod(p*5.0, 1.0);"
	"  const int nr_light = 36;"
	"  float b = 0.0;"
	"  for (int i=0; i<nr_light; ++i) {"
	"    float t = time;"
	"    float r = t+ 3.14159265358979*2.0* float(i) /float(nr_light);"
	"    vec2 lp = mouse + 0.5*cos(t)*(abs(cos(t+float(i)))*0.3+0.7)*vec2(cos(r), sin(r));"
	"    float d = pow(1.0 / (distance(p, lp)+1.0), 60.0);"
	"    b += d;"
	"  }"
	"  gl_FragColor = vec4(vec3(clamp(b,0.0,1.0)), 1);"
	"}";*/

// from android samples
// return current time in milliseconds
/*inline double now_ms()
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}*/
#include <sys/time.h>
inline long long now_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

#include <dirent.h>
char *getFile(char *ext, int *c)
{
	char *path = "./";

	static char s[BUFSIZ];
	int n = -1, e = strlen(ext);

	//printf("+%s\n", path);
	DIR *dir = opendir(CATGL_ASSETS(path));
	if (!dir) return 0;
	struct dirent *d, *ld = 0, *fd = 0;
	while ((d = readdir(dir)) != NULL) {
		char *p = strchr(d->d_name, 0);
		//printf("-%s,%s\n", d->d_name, p-e);
		if (strstr(p-e, ext)) {
			n++;
			if (n==*c) {
				strncpy(s, d->d_name, BUFSIZ);
				break;
			}
			ld = d;
			if (!n) fd = d;
		}
	}
	closedir(dir);

	if (n!=*c) {
//		printf("%d %d %x\n", *c, n, ld);
		if (!ld) return 0;
		if (*c<0) {
			*c = n;
			strncpy(s, ld->d_name, BUFSIZ);
		} else {
			*c = 0;
			strncpy(s, fd->d_name, BUFSIZ);
		}
	}
	return s;
}

void keyEvent(int key, int action)
{
	static int glsl;
	char *s = 0;

	if (action == CATGL_ACTION_DOWN) {
		switch (key) {
		case CATGL_KEY_UP:
			glsl++;
			s = getFile(".glsl", &glsl);
			break;
		case CATGL_KEY_DOWN:
			glsl--;
			s = getFile(".glsl", &glsl);
			break;
		}
	}

	if (s) {
		LOGD("-- %s\n", s);
		glUseProgram(0);
		glDeleteProgram(program);

		char *fsrc = caGetFileContents(CATGL_ASSETS(s));
		program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
		glUseProgram(program);
		free(fsrc);

		name = s;
	}
}

void mouseEvent(int button, int action, int x, int y)
{
	static int lx, ly;

	mx = x / width;
	my = 1.0 - y / height;

	keyEvent(CATGL_KEY_DOWN, action);
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

static const GLfloat position[][2] =
{
	{ -1.0f, -1.0f },	// 0
	{  1.0f, -1.0f },	// 1
	{  1.0f,  1.0f },	// 2
	{ -1.0f, -1.0f },	// 0
	{  1.0f,  1.0f },	// 2
	{ -1.0f,  1.0f },	// 3
};

int vertices;

GLuint _caCreateObject(const GLfloat *position, int size, GLuint num)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size * num, position, GL_STATIC_DRAW);

	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vao;
}

void caInit(int w, int h)
{
	vertices = sizeof position / sizeof position[0];

	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	glUseProgram(program);

	vao = _caCreateObject(position, 2, vertices);

	width = w;
	height = h;
	pixelRatio = (float)width / (float)height;
//	start = clock();
	start = now_ms();

	caKeyEvent = keyEvent;
	caMouseEvent = mouseEvent;

#ifdef CATGL_NANOVG
	nvgCreateEx(vg, NVG_ANTIALIAS);
#endif
}

void caRender()
{
#ifdef CATGL_NANOVG
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);*/
	nvgBeginFrame(vg, width, height, pixelRatio);
	nvgBeginPath(vg);
	//nvgText(vg, 0, 10, "Hello! こんにちは！", NULL);
	nvgText(vg, 0, 10, name, NULL);
	nvgFill(vg);
	nvgEndFrame(vg);
#endif

	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	glUniform2f(glGetUniformLocation(program, "resolution"), width, height);
	glUniform2f(glGetUniformLocation(program, "mouse"), mx, my);

//	glUniform1f(glGetUniformLocation(program, "time"), ((double)clock() - start)/10000.0);
	glUniform1f(glGetUniformLocation(program, "time"), (now_ms() - start)/1000.0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices);
	glBindVertexArray(0);
}

void caEnd()
{
#ifdef CATGL_NANOVG
	nvgDelete(vg);
#endif

	glUseProgram(0);
	glDeleteProgram(program);
}
