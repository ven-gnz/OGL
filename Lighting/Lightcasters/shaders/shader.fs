#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float discoloration;

// Some copy paste error happened. This is the source code version (until it wasn't)

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    
    // check if lighting is inside the spotlight cone
  //  float theta = dot(lightDir, normalize(-light.direction));
  //  float epsilon = light.cutOff - light.outerCutOff;
    // float intensity = 1.0 - clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); black spotlight hehe
    // float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
   
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
        vec3 colorWarp = vec3(0, discoloration, 0);
        if(distance < 4.0){
            colorWarp += vec3(1,0,0); 
        }

        diffuse += colorWarp;
        specular += colorWarp;

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
       // diffuse   *= intensity;
       // specular *= intensity;

       diffuse *= attenuation;
       specular *= attenuation;   
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    
    
} 