//precision mediump float;
uniform float time;
uniform vec2  resolution;

void main()
{
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

	float s = sin(time);		// サインを求める
	float c = cos(time);		// コサインを求める
	mat2 m = mat2(c, s, -s, c);	// 行列に回転用の値をセット
	p *= m;				// 行列をベクトルに掛け合わせる

	vec2 q = vec2(p.x - 1.0, p.y);
	float f = 0.1 / length(q);
	gl_FragColor = vec4(vec3(f), 1.0);
}
