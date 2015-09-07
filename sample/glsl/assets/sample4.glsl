// By @paulofalcao
//
// Blobs

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t)
{
	float xx=x+cos(t*fx*1.1)*sx;
	float yy=y+tan(t*fy*0.1)*sy;
	return 1.0/sqrt(xx*xx+yy*yy);
}

void main()
{
	vec2 p=(gl_FragCoord.xy/resolution.y)/1.0-vec2(1,sin(resolution.y*resolution.y)/20.0);

	//p=p*1.0;

	float x=p.x;
	float y=p.y;

	float a=
	        makePoint(x ,y,.5,.5,.5,.5,time);
	a=a+makePoint(x,y,1.9,2.0,0.4,0.4,time);
	a=a+makePoint(x,y,0.8,0.7/time,0.4,0.5,time);
	a=a+makePoint(x,y,2.3,0.1,0.6,0.3,time);
	a=a+makePoint(x,y,0.8,1.7,0.5,0.4,time);
	a=a+makePoint(x,y,0.3,1.0,0.4,0.4,time);
	a=a+makePoint(x,y,1.4,1.7,0.4,0.5,time);
	a=a+makePoint(x,y,1.3,2.1,0.6,0.3,time);
	a=a+makePoint(x,y,1.8,1.7,0.5,0.4,time);

	float b=
	        makePoint(x,y,1.2,1.9,0.1,0.2,time);
	b=b+makePoint(x,y,0.7,2.7,0.4,0.4,time);
	b=b+makePoint(x,y,1.4,0.6,0.4,0.5,time);
	b=b+makePoint(x,y,2.6,0.4,0.6,0.3,time);
	b=b+makePoint(x,y,0.7,1.4,0.5,0.4,time);
	b=b+makePoint(x,y,0.7,1.7,0.4,0.4,time);
	b=b+makePoint(x,y,0.8,0.5,0.4,0.5,time);
	b=b+makePoint(x,y,1.4,0.9,0.6,0.3,time);
	b=b+makePoint(x,y,0.7,1.3,0.5,0.4,time);

	float c=
	        makePoint(x,y,3.7,0.3,0.3,0.3,time);
	c=c+makePoint(x,y,1.9,1.3,0.4,0.4,time);
	c=c+makePoint(x,y,0.8,0.9,0.4,0.5,time);
	c=c+makePoint(x,y,1.2,1.7,0.6,0.3,time);
	c=c+makePoint(x,y,0.3,0.6,0.5,0.4,time);
	c=c+makePoint(x,y,0.3,0.3,0.4,0.4,time);
	c=c+makePoint(x,y,1.4,0.8,0.4,0.5,time);
	c=c+makePoint(x,y,0.2,0.6,0.6,0.3,time);
	c=c+makePoint(x,y,1.3,0.5,0.5,0.4,time);


	vec3 d = vec3(cos(a),sin(b),sin(c))/sin(2.0);

	gl_FragColor = vec4(d.y,d.y,d.y,0.2);

}
