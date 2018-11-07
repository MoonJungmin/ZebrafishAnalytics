#version 460
uniform sampler2D em_texture;
uniform usampler2D lb_texture;
uniform sampler2D cell_color_texture;
uniform int visualize_method;
uniform int cell_color_table_width;

in vec2 vTexCoord;
out vec4 fragColor;
float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
void main(void)
{
	float em_value = texture(em_texture, vTexCoord).r;
	uint lb_value = texture(lb_texture, vTexCoord).r;
	
	float cell_color_x;
	float cell_color_y;


	if(int(lb_value) != 0){
		vec3 emcolor = 0.5*vec3(em_value, em_value, em_value);
		if(visualize_method == 0){
			vec3 color = 0.5*vec3(rand(vec2(lb_value, 1)), rand(vec2(lb_value, 2)), rand(vec2(lb_value, 3)));
			fragColor = vec4(color+emcolor, 1.0f);	
		}
		else if(visualize_method == 1){
			cell_color_y = float(int(lb_value/cell_color_table_width)) / float(cell_color_table_width);
			cell_color_x = float(int(lb_value%cell_color_table_width)) / float(cell_color_table_width);
			vec4 color = texture(cell_color_texture, vec2(cell_color_x, cell_color_y));
			//vec3 color = vec3(1, 0, 0);
			fragColor = vec4(color.rgb+emcolor, 1.0f);
		}
		else if(visualize_method == 2){
			vec3 color = vec3(0, 1, 0);
			fragColor = vec4(color, 1.0f);	
		}
		
	}
	else{
		fragColor = vec4(em_value, em_value, em_value, 1.0f);
	}

//   if(vTexCoord.s < 0.01 || vTexCoord.t > 0.99){
//       fragColor = vec4(0, 0, 1, 1);
//   }

};