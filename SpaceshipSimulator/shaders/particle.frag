uniform sampler2D particleTexture;

in vec2 UV;
in vec4 particleColor;

out vec4 color;

void main(void)
{
	color = texture(particleTexture, UV) * particleColor;
	color.a = color.a * color.a;

	if(color.a < 0.1)
		discard;
}