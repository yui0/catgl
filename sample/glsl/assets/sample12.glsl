/*
  Daily an hour GLSL sketch by @chimanaco 11/30
*/

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	vec2 p = ( gl_FragCoord.xy / resolution.xy ) - 0.5;
	vec2 direction = vec2(cos(time), sin(time));

	float sx = 0.15 * sin( 5.0 * p.x - time - length(p));
	float dy = 1.0 / ( 10.0 * abs(p.y - sx ));
	vec3 c = hsv2rgb(vec3( ( (p.x + 0.1) * dy) * 0.5 + cos(dot(p, direction)) * 5.0, 0.4, dy));
	gl_FragColor = vec4( c, 1.0 );
}
