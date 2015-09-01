//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#define CATGL_IMPLEMENTATION
#include "catgl.h"

GLuint program;
GLuint vao;

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
/*char fsrc[] =
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
	"}";*/
// 回る円(残像)
char fsrc[] =
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
	"}";

static const GLfloat position[][2] =
{
	{ -0.9f, -0.9f },	// 0
	{  0.9f, -0.9f },	// 1
	{  0.9f,  0.9f },	// 2
	{ -0.9f, -0.9f },	// 0
	{  0.9f,  0.9f },	// 2
	{ -0.9f,  0.9f },	// 3
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

// 表示の初期化
void caInit(int width, int height)
{
	vertices = sizeof position / sizeof position[0];

	program = caCreateProgram(vsrc, "position", fsrc, "gl_FragColor");
	vao = _caCreateObject(position, 2, vertices);

	glUseProgram(program);
	
	float resolution[2];
	resolution[0] = width;
	resolution[1] = height;
	glUniform2fv(glGetUniformLocation(program, "resolution"), 1, resolution);
}

// 描画
void caRender()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	static float time;
	glUniform1f(glGetUniformLocation(program, "time"), time++);

	glBindVertexArray(vao);
	//glDrawArrays(GL_QUADS, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, vertices);
	glBindVertexArray(0);
}

void caEnd()
{
	glUseProgram(0);
	glDeleteProgram(program);
}
