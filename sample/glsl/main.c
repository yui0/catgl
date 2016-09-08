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
long long start;

#define MAX_TEXTURE_STACK	(2+3)
//int ifb = 2;
//GLuint renderbuffer;
GLuint framebuffer[MAX_TEXTURE_STACK];
GLuint textures[MAX_TEXTURE_STACK];

float width, height, pixelRatio;
float x_angle, y_angle, z_angle;
float mx, my, mz;

GLuint vbo;
GLuint program[MAX_TEXTURE_STACK];
//GLuint textures[2];

char *name = "Hello! こんにちは！";
#ifdef CATGL_NANOVG
struct NVGcontext* vg;
#endif

char vsrc[] =
	"#version 120\n"
	"attribute vec2 position;"
	"void main() {"
	"   gl_Position = vec4(position, 0., 1.);"
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

#include <sys/time.h>
inline long long now_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

#include <dirent.h>
char *getFile(char *path, char *ext, int *c)
{
	static char s[BUFSIZ];
	int n = -1, e = strlen(ext);

	//printf("+%s\n", path);
	strncpy(s, path, BUFSIZ);
	DIR *dir = opendir(CATGL_ASSETS(path));
	if (!dir) return 0;
	struct dirent *d, *ld = 0, *fd = 0;
	while ((d = readdir(dir)) != NULL) {
		char *p = strchr(d->d_name, 0);
		//printf("-%s,%s\n", d->d_name, p-e);
		if (strstr(p-e, ext)) {
			n++;
			if (n==*c) {
				strncat(s, d->d_name, BUFSIZ);
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
			strncat(s, ld->d_name, BUFSIZ);
		} else {
			*c = 0;
			strncat(s, fd->d_name, BUFSIZ);
		}
	}
	return s;
}
/*char exts[2][4] = {".jpg", ".JPG"};
char *exts[4];
int selects(Dirent *dir)
{
	int *p;
	char a;

	a = dir->d_namlen;
	if (a<4) return 0;
	a -= 4;
	p = (int*)&(dir->d_name[a]);

	if ((*p == *(int*)&exts[0])) return 1;
	return 0;
}
char *getFile(char *ext, int *c)
{
	char *path = "./";
	struct dirent **namelist;

	exts[0] = ext;
	int r = scandir(dirname, &namelist, selects, alphasort);
	if (r==-1) {
		LOGD("%s", dirname);
		return 0;
	}

	for (int i=0; i<r; i++) {
		printf("%s\n", namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
}*/

GLuint caCreateProgramFromFile(char *s)
{
	GLuint program;
	LOGD("-- %s ", s);
	char *fsrc = caGetFileContents(CATGL_ASSETS(s));
	if (fsrc) {
		program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
		free(fsrc);
	} else {
		program = 0;
	}
	LOGD("..\n");
	return program;
}
#include <sys/stat.h>
/*typedef struct _SScript {
	GLuint program;
	GLuint in, out;
} SScript;
SScript sscript[20];*/
int filter;
void keyEvent(int key, int action)
{
	static int glsl, sscript;
	char *s = 0, *ss = 0;

	if (action == CATGL_ACTION_UP) {
		switch (key) {
		case CATGL_KEY_UP:
			glsl++;
			glsl++;
		case CATGL_KEY_DOWN:
			glsl--;
			s = getFile("./", ".glsl", &glsl);
			break;

		case CATGL_KEY_RIGHT:
			sscript++;
			sscript++;
		case CATGL_KEY_LEFT:
			sscript--;
			ss = getFile("./sscripts/", ".ss", &sscript);
			break;
		case CATGL_KEY_F:
			filter = filter ? 0 : 1;
			break;
		}
	}

	if (s) {
		glUseProgram(0);
		glDeleteProgram(program[0]);
		program[0] = caCreateProgramFromFile(s);

		// texture
		char fname[BUFSIZ], name[BUFSIZ];
		strcpy(name, s);
		name[strlen(s)-5] = 0;
		sprintf(fname, "%s.jpg", name);
		struct stat st;
		if (!stat(CATGL_ASSETS(fname), &st)) {
			glDeleteTextures(1, &textures[0]);
			textures[0] = caLoadTexture(fname);
			LOGD("---- %s[%d]\n", fname, textures[0]);
		}
		sprintf(fname, "%s+.jpg", name);
		if (!stat(CATGL_ASSETS(fname), &st)) {
			glDeleteTextures(1, &textures[1]);
			textures[1] = caLoadTexture(fname);
			LOGD("---- %s[%d]\n", fname, textures[1]);
		}

		sprintf(fname, "%s.png", name);
		if (!stat(CATGL_ASSETS(fname), &st)) {
			glDeleteTextures(1, &textures[0]);
			textures[0] = caLoadTexture(fname);
			LOGD("---- %s[%d]\n", fname, textures[0]);
		}
		sprintf(fname, "%s+.png", name);
		if (!stat(CATGL_ASSETS(fname), &st)) {
			glDeleteTextures(1, &textures[1]);
			textures[1] = caLoadTexture(fname);
			LOGD("---- %s[%d]\n", fname, textures[1]);
		}
	}

	if (ss) {
		// shader script
		char *script = caGetFileContents(CATGL_ASSETS(ss));
		char *p, *q, filter[256];
		int i = 1;
		for (p=script; (q=strtok(p, "\n")); p=0) {
			strcpy(filter, "filters/");
			sscanf(q, "%s", &filter[8]);
			strcat(filter, ".glsl");
			LOGD("%s\n", filter);

/*			in = textures[0];
			out = framebuffer[2];
			glDeleteProgram(sscript[n].program);
			sscript[n].program = caCreateProgramFromFile(filter);
			sscript[n].in = in;
			sscript[n].out = out;*/

			glDeleteProgram(program[i]);
			program[i++] = caCreateProgramFromFile(filter);
		}
		free(script);
	}
}

void mouseEvent(int button, int action, int x, int y)
{
	static int lx, ly;

	mx = x;
	my = height-y;

	if (action==CATGL_ACTION_DOWN) mz = 1.0;
	else if (action==CATGL_ACTION_UP) mz = 0.0;

	//keyEvent(CATGL_KEY_DOWN, action);
	if (x < 100) keyEvent(CATGL_KEY_UP, action);
	else if (x > width-100) keyEvent(CATGL_KEY_DOWN, action);

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

const float vertices[] = {
	-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 1.0f
};

/*void caCreateRBO(GLuint *renderbuffer, int w, int h)
{
	glGenRenderbuffers(1, renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}*/
void caCreateFBO(GLuint *framebuffer, GLuint *texture, int w, int h)
{
	glGenFramebuffers(1, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void caInit(int w, int h)
{
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	GLuint d[6];
	d[0] = CATGL_ATT_VERTEX;	d[1] = 3;
	d[2] = CATGL_ATT_TEXTURE;	d[3] = 2;
	vbo = caCreateObject_((void*)vertices, sizeof(float)*5, 4, d, 2);

	program[0] = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	program[2] = caCreateProgramFromFile("filters/gaussian5x5.glsl");
	program[1] = caCreateProgramFromFile("filters/edge.glsl");
//	program[1] = caCreateProgramFromFile("filters/negative.glsl");
//	program[1] = caCreateProgramFromFile("filters/sepia.glsl");
//	program[1] = caCreateProgramFromFile("filters/mono.glsl");
//	program[1] = caCreateProgramFromFile("filters/random_dither.glsl");
	caCreateFBO(&framebuffer[2], &textures[2], 640, 480);
//	caCreateRBO(&renderbuffer, 640, 480);
	caCreateFBO(&framebuffer[3], &textures[3], 640, 480);
//	caCreateRBO(&renderbuffer, 640, 480);

	width = w;
	height = h;
	pixelRatio = (float)width / (float)height;
	start = now_ms();

	caKeyEvent = keyEvent;
	caMouseEvent = mouseEvent;

#ifdef CATGL_NANOVG
	nvgCreateEx(vg, NVG_ANTIALIAS);
#endif
}

void render(GLuint program, GLuint texture)
{
	glClear(GL_COLOR_BUFFER_BIT);
#ifdef CATGL_NANOVG
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	nvgBeginFrame(vg, width, height, pixelRatio);
	nvgBeginPath(vg);
	nvgText(vg, 0, 10, name, NULL);
	nvgFill(vg);
	nvgEndFrame(vg);
#endif

	glUseProgram(program);

	glUniform1f(glGetUniformLocation(program, "time"), (now_ms() - start)/1000.0);
	glUniform2f(glGetUniformLocation(program, "mouse"), mx/width, my/height);
	glUniform2f(glGetUniformLocation(program, "resolution"), width, height);
	glUniform4f(glGetUniformLocation(program, "iMouse"), mx, my, mz, 0.0);
	//glUniform2f(glGetUniformLocation(program, "surfaceSize"), width, height);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "iChannel0"), 0);	// GL_TEXTURE0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(glGetUniformLocation(program, "iChannel1"), 1);	// GL_TEXTURE1

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
typedef struct _SScript {
//	GLuint program;			// proceed
//	GLuint framebuffer, texture;	// out
	int sh;
	int in;
	//float param[10];
	int out;
} SScript;
/*SScript ss[] = {
	{ 0, 0, 2 },
	{ 2, 2, 3 },
	{ 1, 3, 0 },
};*/
SScript ss[] = {
	{ 0, 0, 2 },
	{ 1, 2, 0 },
};
void caRender()
{
	if (filter) {
		SScript *p = ss;
		for (int i=sizeof(ss)/sizeof(SScript)-1; i>0; i--) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[p->out]);
			render(program[p->sh], textures[p->in]);
			p++;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render(program[p->sh], textures[p->in]);
	} else {
		render(program[0], textures[0]);
	}
}

void caEnd()
{
#ifdef CATGL_NANOVG
	nvgDelete(vg);
#endif

	glUseProgram(0);
	glDeleteProgram(program[0]);
}
