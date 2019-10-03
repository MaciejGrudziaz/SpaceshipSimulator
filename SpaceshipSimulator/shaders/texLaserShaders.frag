uniform sampler2D tex;

in vec2 UV;
in vec4 fragColor;

out vec4 color;

void main(void)
{
	color = texture(tex, UV) * fragColor;
	color.a = color.a * color.a;

	if(color.a < 0.05)
		discard;
}