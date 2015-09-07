//precision mediump float;
uniform float time;
uniform vec2  resolution;

void main()
{
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
	vec2 q = mod(p, 0.2) - 0.1;
	float s = sin(time);
	float c = cos(time);
	q *= mat2(c, s, -s, c);
	float v = 0.1 / abs(q.y) * abs(q.x);
	float r = v * abs(sin(time * 6.0) + 1.5);
	float g = v * abs(sin(time * 4.5) + 1.5);
	float b = v * abs(sin(time * 3.0) + 1.5);
	gl_FragColor = vec4(r, g, b, 1.0);
}
