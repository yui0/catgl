#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define PI 3.14159265359

void main()
{
	vec2 position = ( gl_FragCoord.xy / resolution.xy );
	position.x = (position.x - 0.5) * (resolution.x / resolution.y) + 0.5;
	position.x = (position.x - 0.5) / (1.1 - 0.2 * position.y) + 0.5;

	position.y -= 0.05 * abs(sin(time*2.0*PI));
	float topleft = step(fract(position.x*4.0), 0.5) * step(fract(position.y*4.0), 0.5);
	float bottomright = step(0.5, fract(position.x*4.0)) * step(0.5, fract(position.y*4.0));
	float diamond = 1.0 - mod(floor((position.x+position.y)*4.0) + floor((position.x-position.y)*4.0), 2.0);

	float color = 0.75 + (0.5 + 0.4 * topleft - 0.4 * bottomright) * diamond;
	float flash = step(2.5, mod(time, 4.0)) * max(0.0, (1.0 - mod(time*2.0, 1.0)))*diamond;
	vec4 finalcolor = mix(vec4(1.0, 0.25, 0.25, 1.0), vec4(0.25, 0.25, 1.0, 1.0), step(4.0, mod(time, 8.0)));
	float whiteflash = max(0.0, 1.0 - mod(time, 4.0)*2.0) * step(1.0, time);

	gl_FragColor = mix((finalcolor * color) - vec4(0.25, 0.25, 0.25, 1.0) * flash*0.25, vec4(1.0, 1.0, 1.0, 1.0), whiteflash);
}
