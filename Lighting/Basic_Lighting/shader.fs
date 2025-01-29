#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStength = 0.1f;
    vec3 ambient = ambientStength * lightColor;
    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}