// プラズマ
#version 120
uniform vec2 resolution;
uniform float time;
float plasma(vec2 p)
{
	p*=10.0;
	return (sin(p.x)*0.25+0.25)+(sin(p.y)*0.25+0.25);
}
void main()
{
	vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
	gl_FragColor = vec4(plasma(pos));
}

