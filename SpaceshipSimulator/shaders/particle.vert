layout (location=0) in vec3 vertex;
layout (location=1) in vec4 color;
layout (location=2) in vec4 centerPosSize;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

out vec2 UV;
out vec4 particleColor;

void main(void)
{
	float size = centerPosSize.w;
	vec3 vertexPos = centerPosSize.xyz 
					 + cameraRight * vertex.x * size
					 + cameraUp * vertex.y * size;
	
	gl_Position = projection * view * vec4(vertexPos, 1.0f);

	particleColor = color;
	UV = vertex.xy + vec2(0.5,0.5);
}