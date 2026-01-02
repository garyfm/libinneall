#version 460 core

out vec4 frag_color;

in vec3 v_normal;  
in vec3 v_frag_pos;  
in vec2 v_uv;
  
uniform vec3 u_light_pos; 
uniform vec3 u_view_pos; 
uniform vec3 u_light_color;

uniform sampler2D u_albedo;

void main()
{
    // ambient
    float ambient_strength = 0.5;
    vec3 ambient = ambient_strength * u_light_color;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_light_pos - v_frag_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_light_color;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_view_pos - v_frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_light_color;  
      
    vec3 result = (ambient + diffuse + specular);
    frag_color = vec4(result, 1.0) * texture(u_albedo, v_uv);
} 
