layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fragTexCoord;
out vec4 fragBlendColor;

uniform vec3 hitColor;
uniform float hitColorRunTime;
uniform float hitColorMaxTime;

void main()
{
	gl_Position = projection*view*model*vec4(pos,1.0);

	float hitFactor = hitColorRunTime / hitColorMaxTime;
	if(hitFactor > 1.0)
		hitFactor = 1.0;

	fragBlendColor = vec4(hitColor, hitFactor);

	fragTexCoord = texCoord;
}