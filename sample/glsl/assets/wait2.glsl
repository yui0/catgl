#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

vec2 position;

vec3 ball(vec3 colour, float sizec, float xc, float yc)
{
	return colour * (sizec / distance(position, vec2(xc, yc)));
}

vec3 grid(vec3 colour, float linesize, float xc, float yc)
{
	float xmod = mod(position.x, xc);
	float ymod = mod(position.y, yc);
	return xmod < linesize || ymod < linesize ? vec3(0) : colour;
}

vec3 circle(vec3 colour, float size, float linesize, float xc, float yc)
{
	float dist = distance(position, vec2(xc, yc));
	return colour * clamp(-(abs(dist - size)*linesize * 50.0) + 0.5, 0.1, 1.0);
}

vec3 red = vec3(2, 1, 1);
vec3 green = vec3(1, 2, 1);
vec3 blue = vec3(1, 1, 2);

void main()
{

	position = ( gl_FragCoord.xy / resolution.xy );
	position.y = position.y * resolution.y/resolution.x + 0.25;

	vec3 color = vec3(0.0);
	float ratio = resolution.x / resolution.y;
	color += circle(blue, 0.085, 0.6, 0.5, 0.5);

	//color += grid(blue * 0.1, 0.001, 0.06, 0.06);
	//color *= 1.0 - distance(position, vec2(0.5, 0.5));
	color += ball(green, 0.01, sin(time*4.0) / 12.0 + 0.5, cos(time*4.0) / 12.0 + 0.5);
	color *= ball(red, 0.02, -sin(time*-8.0) / 12.0 + 0.5, -cos(time*-8.0) / 12.0 + 0.5) + 0.5;
	gl_FragColor = vec4(color, 1.0 );
}
