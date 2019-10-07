uniform vec3 maxColor;
uniform vec3 minColor;
uniform float frac;
uniform vec2 minPos;
uniform vec2 maxPos;

out vec4 fragColor;

void main(void)
{
	vec4 vertex = vec4(0,0,0,1);
	if(gl_VertexID == 0 || gl_VertexID == 4)
	{
		vertex.x = (minPos.x * 2.0) - 1.0;
		vertex.y = (minPos.y * 2.0) -1.0;
	}
	if(gl_VertexID == 1)
	{
		vertex.x = ((minPos.x + (maxPos.x - minPos.x) * frac)  * 2.0) - 1.0;
		vertex.y = (minPos.y * 2.0) - 1.0;
	}
	if(gl_VertexID == 2)
	{
		vertex.x = ((minPos.x + (maxPos.x - minPos.x) * frac)  * 2.0) - 1.0;
		vertex.y = (maxPos.y * 2.0) - 1.0;
	}
	if(gl_VertexID == 3)
	{
		vertex.x = (minPos.x * 2.0) - 1.0;
		vertex.y = (maxPos.y * 2.0) - 1.0;
	}

	gl_Position = vertex;

	fragColor = vec4(maxColor * frac + minColor * (1.0 - frac), 1.0);
}
