#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

//反復回数(constで書く方も多い)
#define ITE_MAX      64

//t更新時の適当な係数。通常1で大丈夫です。
//複雑な形状だったりレイ突き抜けるような小さいオブジェクトは値を0.25位にすると良いです
#define DIST_COEFF   1.00

//打ち切り係数
#define DIST_MIN     0.001

//t最大
#define DIST_MAX     1000.0

//rotate
vec2 rot(vec2 p, float a)
{
	return vec2(
	               cos(a) * p.x - sin(a) * p.y,
	               sin(a) * p.x + cos(a) * p.y);
}

//形状を記述していきます
//参考 : http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float map(vec3 p)
{
	//初期値
	float t = DIST_MAX;
	float w = 0.0;

	//平面を作ります。aX + bY + zZ = -D
	//前回計算より小さい値 == 解でtを更新
	w = 0.1 + dot((p), vec3(0, 1, 0));
	t = min(t, w);

	//球を出します(直径1.0)
	w = length(p) - 0.2;
	t = min(t, w);

	//箱を作ってみます
	w = length(max(abs(p) - vec3(0.5, 0.01, 0.5) , 0.0)) - 0.01;
	t = min(t, w);

	return t;
}

void main()
{
	//-1 ～ +1のテクスチャUVを作る
	vec2 uv = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;

	//up - yにしたい場合はする
	//uv.y = -uv.y;

	//レイを作成する。
	float aspect = resolution.x / resolution.y;
	vec3 dir = normalize(vec3(uv * vec2(aspect, 1.0), 1.0));

	//視点を適当に回転させます
	dir.yz = rot(dir.yz, 0.4);
	dir.xz = rot(dir.xz, 0.3);

	//eye座標(ちょっと動かしています)
	vec3 pos = vec3(0.2 + sin(time), 0.4, -1.0);

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
	vec3 color = vec3(map(vec3(ip.x, ip.y, -ip.z) + 0.1));

	//適当に味付けでdirつけています。消しても大丈夫です。
	gl_FragColor = vec4(color + dir * 0.1, 1.0);
}
