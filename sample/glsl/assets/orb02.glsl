//precision mediump float;
uniform float time;
uniform vec2  resolution;

void main()
{
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
	p += vec2(cos(time), sin(time)) * 0.5;
	float l = 0.1 / length(p);
	gl_FragColor = vec4(vec3(l), 1.0);
}
