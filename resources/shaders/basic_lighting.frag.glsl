#version 460 core

struct Material {
    sampler2D albedo;
    sampler2D specular;
    float shininess;
};

struct LightDirectional {
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightPoint {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float atten_constant;
    float atten_linear;
    float atten_quadratic;
};

struct LightSpot {
    vec3 pos;
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float inner_cutoff_cosine;
    float outer_cutoff_cosine;
};

out vec4 frag_color;

in vec3 v_normal;  
in vec3 v_frag_pos;  
in vec2 v_uv;
  
uniform Material u_material; 
uniform LightDirectional u_light_dir; 
uniform LightPoint u_light_point; 
uniform LightSpot u_light_spot; 
uniform vec3 u_view_pos; 

vec3 CalculateLightPoint(LightPoint light, vec3 frag_pos, vec3 material_albedo, vec3 material_specular, 
                            float material_shininess, vec3 normal, vec3 view_dir)
{

    // ambient
    vec3 ambient = light.ambient * material_albedo;
  	
    // diffuse 
    vec3 light_dir = normalize(light.pos - frag_pos);
    float light_angle = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * light_angle * material_albedo;
    
    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material_shininess);
    vec3 specular = light.specular * spec * material_specular;  
     
    // attenuation
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.atten_constant + light.atten_linear * distance + 
    		    light.atten_quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalculateLightSpot(LightSpot light, vec3 frag_pos, vec3 material_albedo, vec3 material_specular, 
                            float material_shininess, vec3 normal, vec3 view_dir)
{
    // Pull out
    vec3 light_dir = normalize(light.pos - frag_pos);

    float theta = dot(light_dir, normalize(-light.dir));
    float epsilon = light.inner_cutoff_cosine - light.outer_cutoff_cosine;
    float intensity = clamp((theta - light.outer_cutoff_cosine) / epsilon, 0.0, 1.0);

    // ambient
    vec3 ambient = light.ambient * material_albedo;

    // diffuse
    float light_angle = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * light_angle * material_albedo;

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material_shininess);
    vec3 specular = light.specular * spec * material_specular;

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

void main()
{
    // TODO: pull out in ti CalcualteLigtDirectional
    //vec3 material_albedo = vec3(texture(u_material.albedo, v_uv));

    //// ambient
    //vec3 ambient = u_light_dir.ambient * material_albedo;
  	//
    //// diffuse 
    //vec3 norm = normalize(v_normal);
    //vec3 light_dir = normalize(-u_light_dir.dir);
    //float light_angle = max(dot(norm, light_dir), 0.0);
    //vec3 diffuse = u_light_dir.diffuse * light_angle * material_albedo;
    //
    //// specular
    //vec3 view_dir = normalize(u_view_pos - v_frag_pos);
    //vec3 reflect_dir = reflect(-light_dir, norm);  

    //float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    //vec3 material_specular = vec3(texture(u_material.specular, v_uv));
    //vec3 specular = u_light_dir.specular * spec * material_specular;  
    //vec3 result = (ambient + diffuse + specular);
    //frag_color = vec4(result, 1.0) * texture(u_material.albedo, v_uv);
      

    vec3 normal = normalize(v_normal);
    vec3 view_dir = normalize(u_view_pos - v_frag_pos);
    vec3 material_albedo = vec3(texture(u_material.albedo, v_uv));
    vec3 material_specular = vec3(texture(u_material.specular, v_uv));

    vec3 result = CalculateLightPoint(u_light_point, v_frag_pos, material_albedo, material_specular, u_material.shininess, normal, view_dir);

    result += CalculateLightSpot(u_light_spot, v_frag_pos, material_albedo, material_specular, u_material.shininess, normal, view_dir);

    frag_color = vec4(result, 1.0);
} 
