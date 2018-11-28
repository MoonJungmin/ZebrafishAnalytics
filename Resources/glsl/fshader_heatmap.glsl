#version 460
uniform sampler2D thumbnail;
uniform sampler2D heatmap;
uniform vec4 heatmap_color;
uniform int heatmap_max;
in vec2 vTexCoord;
out vec4 fragColor;

void main(void)
{
	float heatmap_value = texture(heatmap, vTexCoord).r / heatmap_max;
	vec4 value = texture(thumbnail, vTexCoord);
	vec4 color = heatmap_color * heatmap_value;

	fragColor = vec4(value.r+color.r, value.g+color.g , value.b+color.b ,1.0);
};