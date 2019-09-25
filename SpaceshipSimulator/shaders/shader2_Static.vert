#version 460 core

layout (location=0) in vec3 pos;
layout (location=1) in float idx;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;

void main(){
	vec4 currPos=vec4(pos,1.0f);

	vec4 position=projection*view*model*currPos;
	gl_Position=position;

	fragColor=vec4(1.0f,0.0f,0.0f,1.0f);
}