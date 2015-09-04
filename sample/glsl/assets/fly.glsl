#ifdef GL_ES
precision highp float;
#endif
#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float iGlobalTime=time;
//Somewhere in 1993 by nimitz (twitter: @stormoid) // Compiled for glslsanbox By Gigatron ; HUD removed !

#define PALETTE 6.8

//3 to 5 works best
#define TERRAIN_COMPLEXITY 4.
#define ITR 60
#define FAR 700.
#define time mod(iGlobalTime,500.)

mat2 mm2(in float a)
{
	float c = cos(a), s = sin(a);
	return mat2(c,-s,s,c);
}
float smoothfloor(const in float x, const in float w)
{
	return floor(x)+smoothstep(w, 1.-w,fract(x));
}

vec3 enpos()
{
	return vec3(sin(time)*100.+50.,sin(time)*30.+30.,300.+sin(time*.9+sin(time*0.88+0.2))*100.);
}


float tri(in float x)
{
	return abs(fract(x)-0.5);
}

mat2 m2 = mat2( 0.80,  0.60, -0.60,  0.80 );
float tnoise(in vec2 p)
{
	p*=.008;
	float z=2.;
	float rz = 0.;
	for (float i= 1.; i < TERRAIN_COMPLEXITY; i++ ) {
		rz+= tri(p.x+tri(p.y*1.))/z;
		z = z*2.;
		p = p*1.8;
		p*= m2;
	}
	return rz*9.;
}

float oct(in vec3 p)
{
	return dot(vec3(0.5773),abs(p));
}
vec2 ou( vec2 d1, vec2 d2 )
{
	return (d1.x<d2.x) ? d1 : d2;
}

vec3 roty(vec3 p, float a)
{
	float s = sin(a), c = cos(a);
	return vec3(c*p.x + s*p.z, p.y, -s*p.x + c*p.z);
}

vec2 map(vec3 p)
{
	//terrain
	vec2 d = vec2(6.*tnoise(p.xz)+p.y+20.+(tri(p.z*0.001)-0.4)*22.,1.);
	//xlog(x) seems to work nicely for a valley
	d.x -= abs(p.x*0.5*log(abs(p.x)))*0.05-8.;
	//flat water
	d = ou(d,vec2(p.y+30., 2.));
	//"enemy"
	vec3 enp = enpos();
	enp.z += time*50.;
	d = ou(d,vec2((oct(roty(p-enp, time*2.5))-6.)*0.66,8.));

	return d;
}

vec2 march(in vec3 ro, in vec3 rd)
{
	float precis = .1;
	float h=precis*2.0;
	float d = 0.;
	float c = 1.;
	for ( int i=0; i<ITR; i++ ) {
		if ( abs(h)<precis || d>FAR ) {
			break;
		}
		d += h;
		vec2 res = map(ro+rd*d);
		h = res.x*1.4;
		c = res.y;
	}
	return vec2(d,c);
}

vec3 normal(const in vec3 p)
{
	vec2 e = vec2(-1., 1.)*.1;
	return normalize(e.yxx*map(p + e.yxx).x +
	                 e.xxy*map(p + e.xxy).x +
	                 e.xyx*map(p + e.xyx).x +
	                 e.yyy*map(p + e.yyy).x );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 p = fragCoord.xy/resolution.xy-0.5;
	vec2 bp = p+0.5;
	p.x*=resolution.x/resolution.y;
	vec2 um = vec2(0);
	um.x = 0.5+(smoothstep(-2.,2.,sin(time*.7-0.1))-0.5)*.1;
	um.y = sin(time+1.)*0.02;

	//camera
	vec3 ro = vec3((smoothstep(-2., 2., sin(time*0.7+1.57))-0.5)*50., sin(time)*5.-1., time*50.);
	um.x *= 3.;
	vec3 eye = normalize(vec3(cos(um.x),um.y*5.,sin(um.x)));
	vec3 right = normalize(vec3(cos(um.x+1.5708),0.,sin(um.x+1.5708)));
	mat2 ori = mm2( smoothstep(-.5,.5,sin(time*0.7+0.78))-.5 + smoothfloor(time*0.04,.45)*6.28 );
	right.xy *= ori;
	vec3 up = normalize(cross(right,eye));
	vec3 rd=normalize((p.x*right+p.y*up)*.75+eye);

	vec3 bg = sin(vec3(0.35,0.4,0.48)*11.3*PALETTE)*0.5+.5;
	vec3 col = bg*floor(-rd.y*50.+6.)*0.06;

	//march
	vec2 rz = march(ro,rd);
	if ( rz.x < FAR ) {
		vec3 pos = ro+rz.x*rd;
		vec3 nor = normal( pos );
		vec3 ligt = normalize(vec3(-.7,0.2, 0.1));
		float dif = clamp(dot(nor, ligt), 0., 1.);
		float fre = pow(clamp(1. + dot(nor, rd), 0., 1.), 2.);
		if (rz.y == 1.) {
			float mx = abs(pos.x*.1)-10.;
			mx = smoothstep(-20.,10.,mx);
			col = mix(vec3(0.,0.37,0),vec3(0.2,.17,0.15),mx);
		} else {
			col = sin(vec3(0.35,0.4,0.48)*rz.y*PALETTE)*0.5+.55;
		}
		col = col*dif + col*0.4 + .3*fre*col;
	}

	fragColor = vec4( col, 1.0 );
}

void main()
{
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	mainImage( color, gl_FragCoord.xy );
	color.w = 1.0;
	gl_FragColor = color;
}
