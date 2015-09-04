// Original: http://glslsandbox.com/e#22802.0
// Now without if statements. Using ternary like a cool kid.

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float triWave(float t)
{
	return abs(mod(t / 2.0, 2.0) - 1.0)-0.5;
}

bool inRange(float x, float a, float b)
{
	return x > min(a,b) && x < max(a,b);
}

bool inVectorRange(vec2 p, vec2 center, vec2 halfsize)
{
	return inRange(p.x, center.x - halfsize.x, center.x + halfsize.x) &&
	       inRange(p.y, center.y - halfsize.y, center.y + halfsize.y);
}

void main()
{
	float ballSize = 0.025;
	float ballRange = 0.8;
	vec2 paddleSize = vec2(0.0125, 0.1);

	vec2 position = ( gl_FragCoord.xy / resolution.xy ) - vec2(0.5, 0.5);
	position.x *= resolution.x / resolution.y;

	vec2 ballPos = vec2(triWave(time)*ballRange, triWave(time*0.725)*(1.0 - ballSize));

	float inter1 = 1.0 - (ballPos.x/ballRange + 0.5);
	vec2 paddle1 = vec2(-0.45 + ballSize, clamp(mix(0.0, ballPos.y, 0.25 + 0.75 * inter1), -0.5 + paddleSize.y, 0.5 - paddleSize.y));

	float inter2 = ballPos.x/ballRange + 0.5;
	vec2 paddle2 = vec2(0.45 - ballSize, clamp(mix(0.0, ballPos.y, 0.25 + 0.75 * inter2), -0.5 + paddleSize.y, 0.5 - paddleSize.y));

	float color = 0.0;
	color = (
	                length(position - ballPos) < ballSize || // ball
	                inVectorRange(position, paddle1, paddleSize) || // paddle left
	                inVectorRange(position, paddle2, paddleSize)) // paddle right
	        ? 1.0 : 0.0;

	gl_FragColor = vec4( color, color, color, 1.0 );
}
