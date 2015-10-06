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
//vec4 iMouse = vec4(mouse.xy*resolution.xy, mouse.z., 0.);
uniform vec4 iMouse;
vec2 iResolution = resolution;

#define PI	3.14159265358979

float sdPlane(vec3 p, vec4 n)
{
	// n must be normalized
	return dot(p, n.xyz) + n.w;
}
float sdBox(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}
float sdSphere(vec3 p, float s)
{
	return length(p)-s;
}
float sdCylinder(vec3 p, vec2 r)
{
	vec2 d = abs(vec2(length(p.xy), p.z)) - r;
	return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - 0.1;
}

const float ADC		= 0.1;	// 0.0-1.0
const float alpha	= .25;	// 0.0-1.0
const float maxDist	= 80.;	// 0.0-?.?

float s, c;
#define rotate(p, a) mat2(c=cos(a), s=-sin(a), -s, c) * p

#define GLOBE	10.
void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = (gl_FragCoord.xy - .5*resolution) / resolution.y;
	vec2 mPos = 3.5 * (iMouse.xy-.5*resolution) / resolution.y;

	vec3 ro = vec3(.0, .0, -15.);
	//vec3 ro = vec3(.0, .0, -1.5);	// GLOBE 10.
	vec3 rd = normalize(vec3(uv, .5));

	//vec2 nav = iMouse.z>0. ? mPos.yx : vec2(.45, .3*iGlobalTime);
	vec2 nav = iMouse.z>0. ? mPos.yx : vec2(-.45, .3*iGlobalTime);
	ro.yz = rotate(ro.yz, nav.x);
	ro.xz = rotate(ro.xz, nav.y);
	rd.yz = rotate(rd.yz, nav.x);
	rd.xz = rotate(rd.xz, nav.y);

	// light pos, background
	vec3 lPos	= normalize(vec3(.5, 1., .25));
	vec3 bg	= vec3(.3, .5, 1.) + pow(length(rd+lPos)/2.5, 2.);

	vec4 RGBA	= vec4(vec3(0.), 1.);

	float dist;
	float t = 0.1;
	for (int i=0; i<150; i++) {
		vec3 pos = ro+rd*t;
//		vec4 r = map(ro, rd, t);

//		vec4 depth = texture2D(iChannel0, vec2(mod(pos.x, 5.)/5., 1.-mod(pos.y, 5.)/5.));	// 1/4
//		vec4 depth = texture2D(iChannel0, vec2(mod(pos.x+5., 10.)/10., 1.-mod(pos.y+5., 10.)/10.));
//		dist = sdBox(pos, vec3(5., 5., 5.)) - depth.r*2.;

//		vec2 tpos = vec2(mod(pos.x+5., 10.)/10., 1.-mod(pos.y+5., 10.)/10.);
//		vec2 tpos = vec2(mod(pos.x, 20.)/20., 1.-mod(pos.y, 10.)/10.);
		//vec2 tpos = vec2(mod(pos.x+5., (PI*5.))/(PI*5.), 1.-mod(pos.y, 10.)/10.);
//		vec2 tpos = vec2((atan(pos.z, pos.x)+PI)/(2.*PI), 1.-mod(pos.y, 10.)/10.);

		//vec2 tpos = vec2(atan(pos.z, pos.x)/(2.*PI)+.5, atan(-pos.y, sqrt(pos.z*pos.z+pos.x*pos.x))/PI+.5);
		//vec2 tpos = vec2(atan(pos.z, pos.x)/(2.*PI)+.5, asin(-pos.y/GLOBE)/PI+.5);
		dist = sdSphere(pos, GLOBE);

		//vec2 tpos = vec2(asin(pos.x/10.)/PI+.5, asin(pos.y/10.)/PI+.5);
		//vec2 tpos = vec2((pos.x/10.)*.5+.5, (pos.y/10.)*.5+.5);

		/*float phi = acos(pos.z);
		vec2 tpos = vec2(phi/PI, acos(pos.y/sin(phi)/(2.*PI)));*/

		if (dist>maxDist || RGBA.a<ADC) {
			// Sky
//			RGBA = vec4(vec3(1., 1., 1.), 1.);
//			RGBA.a *= 1.00001 - alpha;
			//RGBA.rgb += bg * RGBA.a;
			RGBA = vec4(vec3(0., 0., 0.), 1.);
			break;
		}
		if (dist < 0.0001) {
			// Objects
			vec2 tpos = vec2(atan(pos.z, pos.x)/(2.*PI)+.5, asin(-pos.y/GLOBE)/PI+.5);
			//RGBA = texture2D(iChannel0, tpos);

			// Clouds
			nav = vec2(-.45, .1*iGlobalTime);
			ro.yz = rotate(ro.yz, nav.x);
			ro.xz = rotate(ro.xz, nav.y);
			rd.yz = rotate(rd.yz, nav.x);
			rd.xz = rotate(rd.xz, nav.y);
			pos = ro+rd*t;
			vec2 tpos2 = vec2(atan(pos.z, pos.x)/(2.*PI)+.5, asin(-pos.y/GLOBE)/PI+.5);

			RGBA = mix(texture2D(iChannel0, tpos), texture2D(iChannel1, tpos2), .3);
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
