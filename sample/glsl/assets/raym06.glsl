#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

//反復回数(constで書く方も多い)
#define ITE_MAX      45

//t更新時の適当な係数。通常1で大丈夫です。
//複雑な形状だったりレイ突き抜けるような小さいオブジェクトは値を0.25位にすると良いです
#define DIST_COEFF   1.00

//打ち切り係数
#define DIST_MIN     0.01

//t最大
#define DIST_MAX     1000.0

//http://www.pouet.net/topic.php?which=7920&page=18&x=31&y=14
#define pi 3.14159265
float perlin(vec3 p)
{
	vec3 i = floor(p);
	vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	vec3 f = cos((p-i)*pi)*(-.5)+.5;
	a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = mix(a.xz, a.yw, f.y);
	return mix(a.x, a.y, f.z);
}


//形状を記述していきます
//参考 : http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float map(vec3 p)
{
	//t初期値
	float t = DIST_MAX;
	float w = 0.0;

	//球を出します(直径1.0)
	w = length(p) - 0.5 + perlin(p * 7.0) * 0.1;
	t = min(t, w);

	return t;
}

void main()
{
	//-1 ～ +1のテクスチャUVを作る
	vec2 uv = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;

	//視点ベクトルを作成する。
	float aspect = resolution.x / resolution.y;
	vec3  dir = normalize(vec3(uv * vec2(aspect, 1.0), 1.0));

	//eye座標
	vec3 pos = vec3(0.0, 0.0, -1.0);

	//map関数で定義した形状を反復法で解きます。ここではt初期値は0にしとく
	float t = 0.0;

	//SphereTracing。ここintersectって名前で別に作る人も多いです
	for (int i = 0 ; i < ITE_MAX; i++) {

		//形状表現した陰関数を反復しながら解く
		//0(DIST_MIN)に近くなったら解に近いので打ち切り
		float ttemp = map(t * dir + pos);
		if (ttemp < DIST_MIN) {
			break;
		}

		//tを更新。DIST_COEFFは複雑な形状を描画する際に小さく為につけています。
		//ちょっとずつレイを進める事ができます。
		t += ttemp * DIST_COEFF;
	}

	//option形状の近くの位置を出しておく
	vec3 ip = pos + dir * t;

	//色を作ります。ここでは進めたtの位置(深度)をただ出力するだけ
	vec3 color = vec3(t * 0.8);

	gl_FragColor = vec4(color, 1.0);
}
