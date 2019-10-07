in vec2 fragTexCoord;
in vec4 fragBlendColor;

uniform sampler2D gameTex;

out vec4 fragColor;

void main()
{
	vec4 blendColor = vec4(1.0,1.0,1.0,1.0) * (fragBlendColor.a) 
					+ vec4(fragBlendColor.xyz,1.0) * (1.0 - fragBlendColor.a);

	fragColor = texture(gameTex, fragTexCoord) * blendColor;
}