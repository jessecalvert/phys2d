
#define PI 3.14159265359

struct light
{
	vec3 ViewP;
	vec3 Direction;

	vec3 Color;

	float LinearAttenuation;
	float QuadraticAttenuation;
};
