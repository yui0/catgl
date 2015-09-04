// Atmospheric scattering model
// Code adapted from Martins
// http://blenderartists.org/forum/showthread.php?242940-unlimited-planar-reflections-amp-refraction-%28update%29
// Martijn Steinrucken countfrolic@gmail.com

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

vec3 wPos;
vec3 sunPos = vec3(0.,0.,1.8);

uniform vec3 cameraPos;

vec3 sunDirection = normalize(sunPos);

float turbidity = 2.0;
float rayleighCoefficient = 2.5;

const float mieCoefficient = 0.005;
const float mieDirectionalG = 0.80;


// constants for atmospheric scattering
const float e = 2.71828182845904523536028747135266249775724709369995957;
const float pi = 3.141592653589793238462643383279502884197169;

const float n = 1.0003; // refractive index of air
const float N = 2.545E25; // number of molecules per unit volume for air at
// 288.15K and 1013mb (sea level -45 celsius)

// wavelength of used primaries, according to preetham
const vec3 primaryWavelengths = vec3(680E-9, 550E-9, 450E-9);

// mie stuff
// K coefficient for the primaries
const vec3 K = vec3(0.686, 0.678, 0.666);
const float v = 4.0;

// optical length at zenith for molecules
const float rayleighZenithLength = 8.4E3;
const float mieZenithLength = 1.25E3;
const vec3 up = vec3(0.0, 0.0, 1.0);

const float sunIntensity = 1000.0;
const float sunAngularDiameterCos = 0.99983194915; // 66 arc seconds -> degrees, and the cosine of that

// earth shadow hack
const float cutoffAngle = pi/1.95;
const float steepness = 1.5;

struct Camera {										// A camera struct that holds all relevant camera parameters
	vec3 position;
	vec3 lookAt;
	vec3 rayDir;
	vec3 forward, up, left;
};

vec3 TotalRayleigh(vec3 primaryWavelengths)
{
	vec3 rayleigh = (8.0 * pow(pi, 3.0) * pow(pow(n, 2.0) - 1.0, 2.0)) / (3.0 * N * pow(primaryWavelengths, vec3(4.0)));   // The rayleigh scattering coefficient

	return rayleigh;

	//  8PI^3 * (n^2 - 1)^2 * (6 + 3pn)     8PI^3 * (n^2 - 1)^2
	// --------------------------------- = --------------------
	//    3N * Lambda^4 * (6 - 7pn)          3N * Lambda^4
}

float RayleighPhase(float cosViewSunAngle)
{
	return (3.0 / (16.0*pi)) * (1.0 + pow(cosViewSunAngle, 2.0));
}

vec3 totalMie(vec3 primaryWavelengths, vec3 K, float T)
{
	float c = (0.2 * T ) * 10E-18;
	return 0.434 * c * pi * pow((2.0 * pi) / primaryWavelengths, vec3(v - 2.0)) * K;
}

float hgPhase(float cosViewSunAngle, float g)
{
	return (1.0 / (4.0*pi)) * ((1.0 - pow(g, 2.0)) / pow(1.0 - 2.0*g*cosViewSunAngle + pow(g, 2.0), 1.5));
}

float SunIntensity(float zenithAngleCos)
{
	return sunIntensity * max(0.0, 1.0 - exp(-((cutoffAngle - acos(zenithAngleCos))/steepness)));
}


float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 1000.0;

vec3 Uncharted2Tonemap(vec3 x)
{
	return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 ToneMap(vec3 color)
{
	vec3 toneMappedColor;

	toneMappedColor = color * 0.04;
	toneMappedColor = Uncharted2Tonemap(toneMappedColor);

	float sunfade = 1.0-clamp(1.0-exp(-(sunPos.z/500.0)),0.0,1.0);
	toneMappedColor = pow(toneMappedColor,vec3(1.0/(1.2+(1.2*sunfade))));

	return toneMappedColor;
}

void main()
{
	sunDirection = normalize(vec3(-(mouse.x-0.5)*2., -1, (mouse.y-0.5)*2.));

	// General parameter setup
	vec2 vPos = 2.0*gl_FragCoord.xy/resolution.xy - 1.0; 					// map vPos to -1..1
	float t = time*0.0;									// time value, used to animate stuff
	float screenAspectRatio = resolution.x/resolution.y;					// the aspect ratio of the screen (e.g. 16:9)
	vec3 finalColor = vec3(0.1);								// The background color, dark gray in this case

	//Camera setup
	Camera cam;										// Make a struct holding all camera parameters
	cam.lookAt = vec3(0,0,3);								// The point the camera is looking at
	cam.position = vec3(0, 5, 0);						// The position of the camera
	cam.up = vec3(0,0,1);									// The up vector, change to make the camera roll, in world space
	cam.forward = normalize(cam.lookAt-cam.position);					// The camera forward vector, pointing directly at the lookat point
	cam.left = cross(cam.forward, cam.up);							// The left vector, which is perpendicular to both forward and up
	cam.up = cross(cam.left, cam.forward);							// The recalculated up vector, in camera space

	vec3 screenOrigin = (cam.position+cam.forward); 					// Position in 3d space of the center of the screen
	vec3 screenHit = screenOrigin + vPos.x*cam.left*screenAspectRatio + vPos.y*cam.up; 	// Position in 3d space where the camera ray intersects the screen

	cam.rayDir = normalize(screenHit-cam.position);						// The direction of the current camera ray

	sunPos = vec3(0, -1, sin(time*0.5)*0.5+0.45);
	sunDirection = normalize(sunPos);//vec3(-(mouse.x-0.5)*2.*screenAspectRatio, -1, (mouse.y-0.5)*2.));
	vec3 viewDir = cam.rayDir;//normalize(wPos - cameraPos);

	// Cos Angles
	float cosViewSunAngle = dot(viewDir, sunDirection);
	float cosSunUpAngle = dot(sunDirection, up);
	float cosUpViewAngle = dot(up, viewDir);

	float sunE = SunIntensity(cosSunUpAngle);  // Get sun intensity based on how high in the sky it is

	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
//	vec3 rayleighAtX = TotalRayleigh(primaryWavelengths) * rayleighCoefficient;
	vec3 rayleighAtX = vec3(5.176821E-6, 1.2785348E-5, 2.8530756E-5);

	// mie coefficients
	vec3 mieAtX = totalMie(primaryWavelengths, K, turbidity) * mieCoefficient;

	// optical length
	// cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = max(0.0, cosUpViewAngle);

	float rayleighOpticalLength = rayleighZenithLength / zenithAngle;
	float mieOpticalLength = mieZenithLength / zenithAngle;


	// combined extinction factor
	vec3 Fex = exp(-(rayleighAtX * rayleighOpticalLength + mieAtX * mieOpticalLength));

	// in scattering
	vec3 rayleighXtoEye = rayleighAtX * RayleighPhase(cosViewSunAngle);
	vec3 mieXtoEye = mieAtX *  hgPhase(cosViewSunAngle, mieDirectionalG);

	vec3 totalLightAtX = rayleighAtX + mieAtX;
	vec3 lightFromXtoEye = rayleighXtoEye + mieXtoEye;

	vec3 somethingElse = sunE * (lightFromXtoEye / totalLightAtX);

	vec3 sky = somethingElse * (1.0 - Fex);
	sky *= mix(vec3(1.0),pow(somethingElse * Fex,vec3(0.5)),clamp(pow(1.0-dot(up, sunDirection),5.0),0.0,1.0));


	// composition + solar disc

	float sundisk = smoothstep(sunAngularDiameterCos,sunAngularDiameterCos+0.00002,cosViewSunAngle);
	vec3 sun = (sunE * 19000.0 * Fex)*sundisk;

	gl_FragColor.rgb = ToneMap(sky+sun);
	gl_FragColor.a = 1.0;
}
