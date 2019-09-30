layout (location=0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec4 fragColor;

void main(){
	vec4 currPos=vec4(pos,1.0f);

	vec4 position=projection*view*model*currPos;
	gl_Position=position;

	fragColor=vec4(color,1.0f);
}