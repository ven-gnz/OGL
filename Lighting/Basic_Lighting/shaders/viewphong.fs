#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
 // uniform vec3 lightPos;
 // uniform vec3 viewPos;

void main()
{
    float ambiance = 0.11;
    vec3 ambient = ambiance * lightColor;


    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos); // (0,0,0) -FragPos
    vec3 lightDir = normalize(LightPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;



    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),16);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}