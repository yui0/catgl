#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define SCREEN_W 0.7
#define SCROLL_SPEED 0.1

vec2 p;
vec2 myfighter_pos = mouse.xy - vec2(0.5, 0.5);


float rand(vec2 co)
{
	return fract(sin(dot(co.xy ,vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 background()
{
	float interval = 0.1;
	float thick = 0.005;

	if (mod(p.y + time * SCROLL_SPEED, interval) < thick || mod(p.x, interval) < thick) {
		return vec3(0, 0.2, 0.4);
	}
	return vec3(0, 0, 0);
}

vec2 get_boss_pos(float offset)
{
	float x = sin(time - offset) * 0.2;

	return vec2(x, 0.35);
}

vec3 bullet(vec2 v, float t)
{
	vec2 pos = get_boss_pos(t) + v * t;

	if (length(p - pos) < 0.008) {
		return vec3(1, 1, 1);
	}
	if (length(p - pos) < 0.01) {
		return vec3(1, 0.8, 0.3);
	}
	return vec3(0);
}

vec3 myfighter(vec2 pos)
{
	float radius = 0.03;

	float dx = pos.x - p.x;
	float dy = pos.y - p.y;

	int px = int(floor(abs(dx) / 0.02));
	int py = int(floor(dy / 0.02));

	bool pixel = false;

	if (px == 0) {
		if (py >= -1 && py <= 1) {
			pixel = true;
		}
	}
	if (px == 1) {
		if (py >= 1 && py <= 2) {
			pixel = true;
		}
	}

	if (pixel) {
		return vec3(1, 1, 1);
	}
	return vec3(0);
}

vec3 laser(vec2 pos)
{
	vec2 boss_pos = get_boss_pos(0.0);

	vec3 c = vec3(0);

	float a = 0.001 + rand( vec2(time) ) * 0.002;

	float d = abs(p.x - pos.x);
	float n = (a / d);

	if (p.y > pos.y) {
		c = vec3(n * 2.0, n * 2.0, n * 5.0);
	}

	if (abs(p.x - boss_pos.x) < 0.08) {
		if (p.y > 0.3) {
			c = vec3(0);
		}
	}
	return c;
}

vec3 boss()
{
	vec2 pos = get_boss_pos(0.0);

	bool pixel = false;

	float dx = pos.x - p.x;
	float dy = pos.y - p.y;

	int px = int(floor(abs(dx) / 0.02));
	int py = int(floor(dy / 0.02));

	if (px >= -5 && px <= 5) {
		if (py >= -5 && py <= 3) {
			if (rand(vec2(px, py)) > 0.5) {
				pixel = true;
			}
		}
	}

	bool collision = false;
	if (
	        abs(pos.x - myfighter_pos.x) < .1
	        &&	pos.y > myfighter_pos.y
	) {
		collision = true;
	}

	vec3 hit_mark = collision ?
	                vec3(rand(vec2(time)) * 0.05 / length(vec2(myfighter_pos.x, pos.y-0.05) - p))
	                :	vec3(0);
	if (pixel) {
		return (collision? vec3(rand(vec2(time))) : vec3(0)) + vec3(1, 0.5, 0);
	}
	return vec3(0) + hit_mark;
}

void main()
{
	p = (gl_FragCoord.xy - resolution * 0.5) / resolution.y;

	if (abs(p.x) > SCREEN_W / 2.0) {
		discard;
	}

	vec3 c = background();

	c += boss();

	for (int i = -4; i <= 2; i++) {
		float t = fract(time * 0.5 + float(i) * 0.03);
		vec2 v = normalize(vec2(float(i) * 0.1, -1.));
		c += bullet(v, t);
	}

	for (int i = -2; i <= 4; i++) {
		float t = fract(time * 0.5 + 0.5 + float(i) * -0.03);
		vec2 v = normalize(vec2(float(i) * 0.1, -1.));
		c += bullet(v, t);
	}

	c += myfighter(myfighter_pos);
	c += laser(myfighter_pos);

	gl_FragColor = vec4(c, 1);
}
