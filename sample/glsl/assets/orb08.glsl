//precision mediump float;
uniform float time;
uniform vec2  resolution;

void main()
{
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
	float f = abs(p.x) * abs(p.y);
	gl_FragColor = vec4(vec3(f), 1.0);
}
