// first flower

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

// My first circle deformation Mahmud Yuldashev mahmud9935@gmail.com

const vec3 bgColor=vec3(0.9, 0.1, 0.05);

void main( void )
{
	vec2 p = gl_FragCoord.xy/resolution.xy -0.5;
	p.x *= resolution.x/resolution.y;

	// deformation of radius
	float time = time+length(p*16.);
	float r=.2*(1.+sin(time))*pow(cos(time-atan(p.x,p.y)*2.), 2.);

	// p distance - radius
	float s = length(p) - r;

	// step
	vec3 col = bgColor / smoothstep(0.1, 1.0, pow(s,0.3));
	col = col * 2.0 * mouse.x;
	gl_FragColor = vec4( col, 1.0 );
}
