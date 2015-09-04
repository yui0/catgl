#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main()
{
	vec2 p = ( gl_FragCoord.xy / resolution.xy ) - 0.5;
	p.x = (resolution.x * p.x) / resolution.y;

	vec2 p1 = p * 8.;
	vec2 p2 = p1 + vec2( cos(time * 5.), sin(time * 12.) );
	float c = 1. - dot(p1,p2 * sin(time * .25));
	float a = atan( p.x, p.y);
	float w = .75 * sin(a * 10.) + .25 * sin(time * 13.);
	c += w * (.5 + .5 * sin(time* 6.));

	gl_FragColor = vec4( c, c * .5 , c * .25, 1. );
}
