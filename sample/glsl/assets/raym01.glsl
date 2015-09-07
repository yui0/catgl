#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main()
{
	vec2 pos = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;
	gl_FragColor = vec4(pos.xy, 1.0, 1.0);
}
