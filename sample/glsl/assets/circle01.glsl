#ifdef GL_ES
precision mediump float;
#endif
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
void main()
{
	vec2 p = (gl_FragCoord.xy-.5*resolution)/min(resolution.x,resolution.y);	//-1~+1の座標系
	vec2 o = vec2(0,0);
	vec3 c = 0.01/(length(p-o))*vec3(1);
	gl_FragColor = vec4(c,1);
}
