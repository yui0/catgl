//precision mediump float;
uniform float time;
uniform vec2  resolution;

void main()
{
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
	float l = 0.1 * abs(sin(time)) / length(p);
	gl_FragColor = vec4(vec3(l), 1.0);
}
