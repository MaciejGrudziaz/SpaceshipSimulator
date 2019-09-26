in vec2 fragTexCoord;

uniform sampler2D gameTex;

out vec4 fragColor;

void main()
{
	fragColor = texture(gameTex, fragTexCoord);
}