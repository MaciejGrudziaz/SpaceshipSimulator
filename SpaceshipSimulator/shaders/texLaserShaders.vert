layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 centerPos;
layout (location = 2) in vec4 rotationQuat;

uniform mat4 view;
uniform mat4 projection;

uniform vec2 size;
uniform vec4 color;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

out vec2 UV;
out vec4 fragColor;

void main(void)
{
	vec4 q = rotationQuat;
	mat4 transformMat;
	transformMat[0][0] = 1 - 2 * (q.y * q.y + q.z * q.z);
	transformMat[0][1] = 2 * (q.x * q.y + q.w * q.z);
	transformMat[0][2] = 2 * (q.x * q.z - q.w * q.y);
	transformMat[0][3] = 0;
	transformMat[1][0] = 2 * (q.x * q.y - q.w * q.z);
	transformMat[1][1] = 1 - 2 * (q.x * q.x + q.z * q.z);
	transformMat[1][2] = 2 * (q.y * q.z + q.w * q.x);
	transformMat[1][3] = 0;
	transformMat[2][0] = 2 * (q.w * q.y + q.x * q.z);
	transformMat[2][1] = 2 * (q.y * q.z - q.w * q.x);
	transformMat[2][2] = 1 - 2 * (q.x * q.x + q.y * q.y);
	transformMat[2][3] = 0;
	transformMat[3][0] = centerPos.x;
	transformMat[3][1] = centerPos.y;
	transformMat[3][2] = centerPos.z;
	transformMat[3][3] = 1;

	vec3 camTransform = cameraRight * vertex.x * size.x
						+ cameraUp * vertex.y * size.y;

	vec4 transformVert = transformMat * vec4(camTransform,1.0);

	gl_Position = projection * view * transformVert;

	fragColor = vec4(color);
	UV = vertex.xy + vec2(0.5,0.5);
}
