#version 460

uniform sampler2D em_texture;
uniform float em_opacity;

uniform sampler2D subregion_texture[10];
uniform int subregion_opacity[10];
uniform vec4 subregion_color[10];

uniform usampler2D cell_texture;
uniform float cell_opacity;
uniform sampler2D cell_color_texture;


uniform vec4 selected_color;
uniform vec4 unselected_color;


uniform int visualize_method;


in vec2 vTexCoord;
out vec4 fragColor;
float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void)
{
	int i;

	float em_value = texture(em_texture, vTexCoord).r;
	vec4 emlayer = vec4(em_value, em_value, em_value, em_opacity);
	
	uint cell_value = texture(cell_texture, vTexCoord).r;
	vec4 celllayer = vec4(0.0, 0.0, 0.0, 0.0);
	float select_value = texture(cell_color_texture, vTexCoord).r;


	if(int(cell_value) != 0){
		if(visualize_method == 0){
			vec3 color = vec3(rand(vec2(cell_value, 1)), rand(vec2(cell_value, 2)), rand(vec2(cell_value, 3)));
			celllayer = vec4(color, cell_opacity);
		}
		else if(visualize_method == 1){
			if(select_value > 0.5){
				vec3 color = selected_color.rgb * select_value;
				celllayer = vec4(color, cell_opacity);
			}
			else{
				vec3 color = unselected_color.rgb * select_value;
				celllayer = vec4(color, cell_opacity);
			}	
		}
	}

	vec4 subregionlayer = vec4(0.0, 0.0, 0.0, 0.0);
	int subregion_count = 1;
	for(i=0;i<10;++i){
		int value = int(texture(subregion_texture[i], vTexCoord).r * 255);
		if(value > 0){
			if(subregion_opacity[i] > 0){
				float opacity = float(subregion_opacity[i]) / 100.0;
				subregionlayer = subregionlayer + vec4(subregion_color[i].rgb, opacity);
				subregion_count ++;
			}			
		}
	}

	subregionlayer = subregionlayer / subregion_count;

	
	//vec4 result = subregionlayer;
	vec4 result = emlayer*(1-subregionlayer.a) + subregionlayer* (subregionlayer.a);
	
	result = result*(1-celllayer.a) + celllayer * (celllayer.a);

	fragColor = vec4(result.rgb, 1.0f);
	//fragColor = vec4(subregion_opacity[0], subregion_opacity[0], subregion_opacity[0], 1.0f);
};