//
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D texture;
//uniform sampler2D iChannel0;
#define iChannel0 texture
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

float iGlobalTime = time;
vec4 iMouse = vec4(mouse*resolution.xy, 0.0, 0.0);
vec2 iResolution = resolution;

float Sign(vec2 p1, vec2 p2, vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool IsPointInTri(vec2 pt, vec2 v1, vec2 v2, vec2 v3)
{
	bool b1, b2, b3;

	b1 = Sign(pt, v1, v2) < 0.0;
	b2 = Sign(pt, v2, v3) < 0.0;
	b3 = Sign(pt, v3, v1) < 0.0;

	return ((b1 == b2) && (b2 == b3));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 r = 2.0*vec2(fragCoord.xy - 0.5*iResolution.xy) / iResolution.y;
	vec2 uv = fragCoord.xy/iResolution.xy;
	uv.y = 1.-uv.y;

	if (IsPointInTri(uv, vec2(0.5, 0.7), vec2(1.0, 0.3), vec2(0.5, 0.3))) {
		float prog = (0.5-uv.x)*2.;

		float hyp = sqrt(pow(0.5,2.)+pow(0.5,2.));

		float r = 0.5;

		float x = 0.0+uv.x + r * cos(60. * iGlobalTime*0.01);
		float y = 0.5+uv.y + r * sin(60. * iGlobalTime*0.01);

		uv.x = x;
		uv.y = y;
	}

	vec4 tex = texture2D(iChannel0, uv);
	vec3 pixel = tex.rgb;

	fragColor = vec4(pixel, 1.0);
}

void main()
{
	vec4 color;
	mainImage(color, gl_FragCoord.xy);
	gl_FragColor = color;
}
