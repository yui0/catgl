// @paulofalcao
//
// Blue Pattern
//
// A old shader i had lying around
// Although it's really simple, I like the effect :)

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;

void main()
{
	vec2 u=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);
	float t=time*0.5;

	float tt=sin(t/8.0)*64.0;
	float x=u.x*tt+sin(t*2.1)*4.0;
	float y=u.y*tt+cos(t*2.3)*4.0;
	float c=sin(x)+sin(y);
	float zoom=sin(t);
	x=x*zoom*2.0+sin(t*1.1);
	y=y*zoom*2.0+cos(t*1.3);
	float xx=cos(t*0.7)*x-sin(t*0.7)*y;
	float yy=sin(t*0.7)*x+cos(t*0.7)*y;
	c=(sin(c+sin(xx)+sin(yy))+1.0)*0.5;
	gl_FragColor=vec4((1.0-length(u)*2.0)*vec3(c*1.1,c*1.4,c*1.9),1.0);
}
