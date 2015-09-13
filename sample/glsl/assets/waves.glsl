uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float iGlobalTime = time;
vec4 iMouse = vec4(mouse, 0.0, 0.0);
vec2 iResolution = resolution;

vec3 COLOR1 = vec3(0.0, 0.0, 0.3);
vec3 COLOR2 = vec3(0.5, 0.0, 0.0);
float BLOCK_WIDTH = 0.01;

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	
	// To create the BG pattern
	vec3 final_color = vec3(1.0);
	vec3 bg_color = vec3(0.0);
	vec3 wave_color = vec3(0.0);
	
	float c1 = mod(uv.x, 2.0 * BLOCK_WIDTH);
	c1 = step(BLOCK_WIDTH, c1);
	
	float c2 = mod(uv.y, 2.0 * BLOCK_WIDTH);
	c2 = step(BLOCK_WIDTH, c2);
	
	bg_color = mix(uv.x * COLOR1, uv.y * COLOR2, c1 * c2);
	
	
	// To create the waves
	float wave_width = 0.01;
	uv  = -1.0 + 2.0 * uv;
	uv.y += 0.1;
	for(float i = 0.0; i < 10.0; i++) {
		
		uv.y += (0.07 * sin(uv.x + i/7.0 + iGlobalTime ));
		wave_width = abs(1.0 / (150.0 * uv.y));
		wave_color += vec3(wave_width * 1.9, wave_width, wave_width * 1.5);
	}
	
	final_color = bg_color + wave_color;
	
	
	fragColor = vec4(final_color, 1.0);
}

void main()
{
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	mainImage( color, gl_FragCoord.xy );
	color.w = 1.0;
	gl_FragColor = color;
}
