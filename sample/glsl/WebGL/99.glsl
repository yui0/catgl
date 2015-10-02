//---------------------------------------------------------
//	Ray marching engine
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

float iGlobalTime = time;
uniform vec4 iMouse;
vec2 iResolution = resolution;

float sdBox(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}

const float ADC		= 0.1;	// 0.0-1.0
const float alpha	= .25;	// 0.0-1.0
const float maxDist	= 80.;	// 0.0-?.?

float s, c;
#define rotate(p, a) mat2(c=cos(a), s=-sin(a), -s, c) * p

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = (gl_FragCoord.xy - .5*resolution) / resolution.y;
	vec2 mPos = 3.5 * (iMouse.xy-.5*resolution) / resolution.y;

	vec3 ro = vec3(.0, .0, -15.);
	vec3 rd = normalize(vec3(uv, .5));

	vec2 nav = iMouse.z>0. ? mPos.yx : vec2(.45, -.3*iGlobalTime);
	ro.yz = rotate(ro.yz, nav.x);
	ro.xz = rotate(ro.xz, nav.y);
	rd.yz = rotate(rd.yz, nav.x);
	rd.xz = rotate(rd.xz, nav.y);

	vec3 adj, xV, yV, zV, V_;
	vec3 po = sign(rd);
	vec3 V	= ro, LV;
	float dist;

	// light pos, background
	vec3 lPos	= normalize(vec3(.5, 1., .25));
	vec3 bg	= vec3(.3, .5, 1.) + pow(length(rd+lPos)/2.5, 2.);

	vec4 RGBA	= vec4(vec3(0.), 1.);

	float t = 0.1;
	for (int i=0; i<150; i++) {
		vec3 pos = ro+rd*t;
//		vec4 r = map(ro, rd, t);

//		vec4 depth = texture2D(iChannel0, vec2(mod(pos.x, 5.)/5., 1.-mod(pos.y, 5.)/5.));	// 1/4
		vec4 depth = texture2D(iChannel0, vec2(mod(pos.x+5., 10.)/10., 1.-mod(pos.y+5., 10.)/10.));
		dist = sdBox(pos, vec3(5., 5., 5.)) - depth.r*2.;

		if (dist>maxDist || RGBA.a<ADC) {
			// Sky
//			RGBA = vec4(vec3(1., 1., 1.), 1.);
//			RGBA.a *= 1.00001 - alpha;

			RGBA.rgb += bg * RGBA.a;
			break;
		}
		if (dist < 0.0001) {
			// Objects
//			RGBA = vec4(vec3(1., 1., 1.), 1.);
			//RGBA = vec4(vec3(mod(pos.x, 1.), 1., 1.), 1.);

			//RGBA = depth;
			RGBA = texture2D(iChannel1, vec2(mod(pos.x+5., 10.)/10., 1.-mod(pos.y+5., 10.)/10.));
			break;
		}

		t += dist;
	}

//	RGBA.rgb += bg * RGBA.a;
	fragColor = vec4(RGBA.rgb, 1.);
}

void main()
{
	mainImage(gl_FragColor, gl_FragCoord.xy);
}
