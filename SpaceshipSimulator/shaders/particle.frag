uniform sampler2D particleTexture;

in vec2 UV;
in vec4 particleColor;

out vec4 color;

void main(void)
{
	color = texture(particleTexture, UV) * particleColor;

	if(color.a < 0.05)
		discard;
}