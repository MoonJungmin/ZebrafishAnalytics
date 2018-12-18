#version 140
#extension GL_ARB_compatibility: enable

uniform vec3 eye_position;
uniform sampler3D tex;
uniform sampler3D heatmap_tex;

in vec3 pixel_position;
uniform vec3 box_min;
uniform vec3 box_max;
uniform vec3 up;
uniform float sample;
uniform sampler1D color_table;
uniform vec4 background_color;
uniform float Udiffu;
uniform float Uambi;
uniform float Uspecu;
uniform float Ushin;
uniform float l_t;
uniform vec3 v_size;
uniform vec4 heatmap_color;

vec3 cross_product(vec3 v1,vec3 v2){
    vec3 v;
    v.x=v1.y*v2.z-v2.y*v1.z;
    v.y=v1.z*v2.x-v2.z*v1.x;
    v.z=v1.x*v2.y-v2.x*v1.y;
    return v;
}

void main()
{
    vec3 right=normalize(cross_product(-eye_position,up));
    vec3 t_pixel_position=eye_position*0.5+right*pixel_position.x+up*pixel_position.y;

//    vec3 direction=normalize(t_pixel_position-eye_position);
    vec3 direction=normalize(-eye_position);

    int cn=0;
    vec3 a[2];
    float k;
    vec3 point;
    if(cn<2){
        k=(box_min.x-t_pixel_position.x)/direction.x;
        point=t_pixel_position+k*direction;
        if(point.y>=box_min.y && point.y<=box_max.y
            && point.z>=box_min.z && point.z<=box_max.z){
                a[cn++]=point;
        }
    }
    if(cn<2){
        k=(box_max.x-t_pixel_position.x)/direction.x;
        point=t_pixel_position+k*direction;
        if(point.y>=box_min.y && point.y<=box_max.y
            && point.z>=box_min.z && point.z<=box_max.z){
                a[cn++]=point;
        }
    }
    if(cn<2){
        k=(box_min.y-t_pixel_position.y)/direction.y;
        point=t_pixel_position+k*direction;
        if(point.x>=box_min.x && point.x<=box_max.x
             && point.z>=box_min.z && point.z<=box_max.z){
                 a[cn++]=point;
        }
    }
    if(cn<2){
        k=(box_max.y-t_pixel_position.y)/direction.y;
        point=t_pixel_position+k*direction;
        if(point.x>=box_min.x && point.x<=box_max.x
             && point.z>=box_min.z && point.z<=box_max.z){
                 a[cn++]=point;
        }
    }
    if(cn<2){
        k=(box_min.z-t_pixel_position.z)/direction.z;
        point=t_pixel_position+k*direction;
        if(point.x>=box_min.x && point.x<=box_max.x
             && point.y>=box_min.y && point.y<=box_max.y){
                 a[cn++]=point;
        }
    }
    if(cn<2){
        k=(box_max.z-t_pixel_position.z)/direction.z;
        point=t_pixel_position+k*direction;
        if(point.x>=box_min.x && point.x<=box_max.x
             && point.y>=box_min.y && point.y<=box_max.y){
                 a[cn++]=point;
        }
    }
    float alpha=0;
    vec4 cur_color=vec4(0,0,0,0);	

    if(cn==2) {
        if(length(a[0]-eye_position)>length(a[1]-eye_position)){
            vec3 t=a[0];
            a[0]=a[1];
            a[1]=t;   
        }

        int sampling_num=int(length(a[1]-a[0])*sample);
        vec3 normalize_box=1.0/(box_max-box_min);
        vec3 dir=(a[1]-a[0])*normalize_box/sampling_num;
        vec3 cur_location=(a[0]-box_min)*normalize_box;
        vec3 d_l=eye_position+up*2+right*2;
        for(int i=0;i<sampling_num;i++){
             cur_location=cur_location+dir;
             float value=texture3D(tex,cur_location).x;
             float heatmap_value=texture3D(heatmap_tex,cur_location).x;
             vec4 t_color=texture1D(color_table,value);
             float t_alpha=t_color.w;

             if(alpha<l_t && t_color.w>0.01){
                 float right_color=texture3D(tex,cur_location+vec3(1.0/v_size.x,0,0)).x;
                 float left_color=texture3D(tex,cur_location-vec3(1.0/v_size.x,0,0)).x;
                 float up_color=texture3D(tex,cur_location+vec3(0,1.0/v_size.y,0)).x;
                 float down_color=texture3D(tex,cur_location-vec3(0,1.0/v_size.y,0)).x;
                 float front_color=texture3D(tex,cur_location-vec3(0,0,1.0/v_size.z)).x;
                 float back_color=texture3D(tex,cur_location+vec3(0,0,1.0/v_size.z)).x;
                 vec3 gr=-vec3(right_color-left_color,up_color-down_color,front_color-back_color);

                 vec3 L=normalize(d_l-(cur_location/normalize_box+box_min));
                 vec3 E=normalize(eye_position-(cur_location/normalize_box+box_min));
                 vec3 R=normalize(-reflect(L,normalize(gr)));
                 vec4 diffu=vec4(Udiffu,Udiffu,Udiffu,Udiffu)*max(dot(normalize(gr),L),0.0)*2;
                 diffu=clamp(diffu,0.0,1.0);
                 vec4 specu=vec4(Uspecu,Uspecu,Uspecu,Uspecu)*pow(max(dot(R,E),0.0),Ushin*0.1);
                 specu=clamp(specu,0.0,1.0);
                 t_color=t_color+Uambi+diffu+specu;
             }
             cur_color=cur_color + heatmap_color*heatmap_value*0.5*(1-alpha) + t_color*t_alpha*0.5*(1-alpha);
             alpha=alpha+(1-alpha)*(heatmap_value*0.5 + t_alpha*0.5);
             if(alpha>0.9)break;
       }
    }
    cur_color=cur_color+background_color*(1-alpha);
	cur_color.w=1;
    gl_FragColor=cur_color;
};
