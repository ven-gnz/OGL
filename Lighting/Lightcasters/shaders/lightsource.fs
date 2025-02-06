#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


void main()
{
    vec3 LightColor = light.ambient + light. diffuse + light.specular;
    FragColor = vec4(1.0); // White light
}