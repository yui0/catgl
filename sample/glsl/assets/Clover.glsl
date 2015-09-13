//
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float iGlobalTime = time;
vec4 iMouse = vec4(mouse, 0.0, 0.0);
vec2 iResolution = resolution;

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 q = 0.6 * (2.0*fragCoord.xy-iResolution.xy)/min(iResolution.y,iResolution.x);

    float a = atan(q.x,q.y);
    float r = length(q);
    float s = 0.5 + 0.5*sin(3.0*a + iGlobalTime);
    float g = sin(1.57+3.0*a+iGlobalTime);
    float d = 0.15 + 0.3*sqrt(s) + 0.15*g*g;
    float h = r/d;
    float f = 1.0-smoothstep( 0.95, 1.0, h );
    h *= 1.0-0.5*(1.0-h)*smoothstep(0.95+0.05*h,1.0,sin(3.0*a+iGlobalTime));
	
	vec3 bcol = vec3(0.9+0.1*q.y,1.0,0.9-0.1*q.y);
	bcol *= 1.0 - 0.5*r;
	h = 0.1 + h;
    vec3 col = mix( bcol, 1.2*vec3(0.6*h,0.2+0.5*h,0.0), f );

    fragColor = vec4( col, 1.0 );
}

void main()
{
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	mainImage( color, gl_FragCoord.xy );
	color.w = 1.0;
	gl_FragColor = color;
}
