/*
  Daily an hour GLSL sketch by @chimanaco 1/30

  References:
  http://pixelshaders.com/examples/quasicrystal.html
  http://www.atmarkit.co.jp/ait/articles/1211/26/news012_3.html
*/

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main()
{
	vec2 position = (gl_FragCoord.xy * 2. -resolution) / resolution.y;
	vec2 p = (position - 0.5) * 5.;
	float angle = time;
	vec2 direction = vec2(cos(angle), sin(angle));

	float brightness = cos(dot(p * sin(angle * 0.25), direction) + sin(distance(direction, tan(p))));
	gl_FragColor.rgb = vec3(.2, brightness, cos(brightness));
	gl_FragColor.a = 1.;
}
