layout (location=0) in vec3 vertex;
layout (location=1) in vec3 speed;
layout (location=2) in vec3 centerPos;
layout (location=3) in float size;
layout (location=4) in float lifeTime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

uniform float shutDownTime;
uniform int continuous;
uniform float runTime;
uniform vec3 baseColor;
uniform vec3 destColor;

out vec2 UV;
out vec4 particleColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void)
{
	float particleLifesCount = floor(runTime / lifeTime);
	float particleCurrLifeTime;

	if(continuous == 1)
	{
		particleCurrLifeTime = runTime - particleLifesCount * lifeTime;
	}
	else 
	{
		particleCurrLifeTime = (runTime - shutDownTime) + (shutDownTime - (floor(shutDownTime/lifeTime) * lifeTime));
	}

	vec4 translate = model * vec4(0.0, 0.0, 0.0, 1.0);
	vec4 rotSpeed = model * vec4(speed, 0.0);
	vec3 vertexPos = centerPos + (vec3(rotSpeed.xyz) * particleCurrLifeTime) + vec3(translate.xyz);

	vertexPos = vertexPos
				+ cameraRight * vertex.x * size
				+ cameraUp * vertex.y * size;
	
	gl_Position = projection * view * vec4(vertexPos, 1.0f);

	float lifeTimeFrac = particleCurrLifeTime / lifeTime;

	if(continuous != 1)
	{
		if(lifeTimeFrac > 1.0f)
			lifeTimeFrac = 1.0;
	}

	particleColor = vec4(baseColor + (1.0 - lifeTimeFrac) * (destColor - baseColor), 1.0 - lifeTimeFrac);

	UV = vertex.xy + vec2(0.5,0.5);
}