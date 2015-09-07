/*
  Daily an hour GLSL sketch by @chimanaco 3/30

  References:
  http://tokyodemofest.jp/2014/7lines/index.html
*/

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  resolution;
uniform float time;
uniform vec2 mouse;

float PI = 3.1415926535;

void main()
{
	vec2 p=(gl_FragCoord.xy -.5 * resolution)/ min(resolution.x,resolution.y);
	vec3 c = vec3(0);

	for (int i = 0; i < 20; i++) {
		float t = 2.* PI * float(i) / 20. * fract(time*0.5);
		float x = cos(t) * sin(t);
		float y = sin(t);
		vec2 o = 0.45 * vec2(x,y);
		float r = fract(t);
		float g = 1.-r;
		//float b = 1.-r;
		c += 0.005/(length(p-o))*vec3(r,g,1);
	}
	gl_FragColor = vec4(c,1);
}
