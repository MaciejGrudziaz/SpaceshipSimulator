layout (location = 0) in vec3 pos;

uniform vec3 color;

uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;

void main(void)
{
	gl_Position = projection * view * vec4(pos,1.0);

	fragColor = vec4(color, 1.0);
}