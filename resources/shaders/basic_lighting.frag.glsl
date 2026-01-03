#version 460 core

struct Material {
    sampler2D albedo;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 frag_color;

in vec3 v_normal;  
in vec3 v_frag_pos;  
in vec2 v_uv;
  
uniform Material u_material; 
uniform Light u_light; 
uniform vec3 u_view_pos; 

void main()
{
    vec3 material_color = vec3(texture(u_material.albedo, v_uv));

    // ambient
    vec3 ambient = u_light.ambient * material_color;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 light_dir = normalize(u_light.pos - v_frag_pos);
    float light_angle = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * light_angle * material_color;
    
    // specular
    vec3 view_dir = normalize(u_view_pos - v_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 material_specular = vec3(texture(u_material.specular, v_uv));
    vec3 specular = u_light.specular * spec * material_specular;  
      
    vec3 result = (ambient + diffuse + specular);
    frag_color = vec4(result, 1.0) * texture(u_material.albedo, v_uv);
} 
