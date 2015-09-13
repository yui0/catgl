//
#ifdef GL_ES
precision mediump float;
#endif

/*uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D iChannel0;

void main()
{
	gl_FragColor = vec4(0.8, 0.9, 1.0, 1.0);
}*/
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D iChannel0;

float iGlobalTime = time;
vec4 iMouse = vec4(mouse*resolution.xy, 0.0, 0.0);
vec2 iResolution = resolution;

void main()
{
	vec2  px = 4.0*(-iResolution.xy + 2.0*gl_FragCoord.xy) / iResolution.y;

	float id = 0.5 + 0.5*cos(iGlobalTime + sin(dot(floor(px+0.5),vec2(113.1,17.81)))*43758.545);

	vec3  co = 0.5 + 0.5*cos(iGlobalTime + 3.5*id + vec3(0.0,1.57,3.14) );

	vec2  pa = smoothstep( 0.0, 0.2, id*(0.5 + 0.5*cos(6.2831*px)) );

	gl_FragColor = vec4( co*pa.x*pa.y, 1.0 );
}

