
//Procedural Ordering by nimitz (twitter: @stormoid)
// Gigatron for glslsandbox
/*
	Demonstration of a simple way to have multiple objects ordered
	by an arbitrary function, in this case depth. The nice thing about
	this algorithm is that the "sorting" is completely parametric
	so the complexity doesn't increase with the number of objects at all.

	There might be some way to make the algorithm more general?
*/
#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;


//Number of objects to order
#define NUM_OBJ 11.

//If defined, the darkest layer is the one on top
//and so on, with the bottom being lightest.
//#define COLOR_BY_LAYER

//Fixed alpha value (otherwise modulated by time)
//#define ALPHA 0.5




mat2 mm2(in float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	return mat2(c,-s,s,c);
}

//the function which defines the ordering
float f (const in float x)
{
	return mod(time-x,NUM_OBJ);
}

vec4 star(in vec2 p, const in float x, const in float num)
{
	p.x+=sin(num*1.)*.25+sin(time*0.4+num*3.)*0.3;
	p.y+=sin(num*2.)*0.1+cos(time*0.5)*0.09;
	p = p/exp(x*.4-3.);

	p *= mm2(time*0.6+num*2.);

	//I knew i would find a use for my pentagon function at some point :)
	//two subtracted inverted pentagons -> 5 pointed star
	vec2 q = abs(p);
	float pen1 = max(max(q.x*1.176-p.y*0.385, q.x*0.727+p.y), -p.y*1.237);
	float pen2 = max(max(q.x*1.176+p.y*0.385, q.x*0.727-p.y), p.y*1.237)*0.619;
	float a = (pen1-pen2)*4.;

	//animation of the "send to back"
	float mx = clamp(0.1+1./x*0.05,0.,10.);
	a = 1.-smoothstep(0.1,mx,a);

	vec3 col = a*(sin((vec3(.19,5.,2.)*(num+1.04)*8.01))*0.5+0.6);
	return vec4(col,a);
}

void main()
{
	//setup coordinates
	vec2 p = gl_FragCoord.xy / resolution.xy-0.5;
	p.x *= resolution.x/resolution.y;
	vec3 col = vec3(0);
	float r = length(p);

	for (float i = 0.; i<NUM_OBJ; i++) {
		//sart by getting an integer value for the current item placement
		float num = floor(f(i));

		//call the ordering function again to process based on newly defined order
		float x = f(num);

		//draw stuff
		vec4 nw = star(p,x,num);

		//blend
#ifdef COLOR_BY_LAYER
		col = col*(1.-smoothstep(0.,1.,nw.a))+vec3(0.1,0.2,0.3);
#else
#ifdef ALPHA
		col = mix(col,nw.rgb,nw.a*ALPHA);
#else
		col = mix(col,nw.rgb,nw.a*(sin(time*0.22)*0.4+0.55));
#endif
#endif
	}

	col = clamp(col,0.,1.);
	//vignetting
	col *= 1.-smoothstep(.4,1.8,r);
	gl_FragColor = vec4(col,1.0);
}
