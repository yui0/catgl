// グラディーション
#version 120
uniform vec2 resolution;
uniform float time;
void main()
{
	vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
	gl_FragColor = vec4(pos, 0.0, 1.0);
}
