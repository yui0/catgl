//precision mediump float;
uniform float time;
uniform vec2  resolution;

void main()
{
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
	vec2 q = mod(p, 0.2) - 0.1;
	float f = 0.2 / abs(q.x) * abs(q.y);
	gl_FragColor = vec4(vec3(f), 1.0);
}
