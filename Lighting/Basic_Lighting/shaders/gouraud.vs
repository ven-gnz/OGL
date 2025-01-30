#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

gl_Position = projection * view * vec4(aPos, 1.0);

vec3 Position = vec3(model * vec4(aPos, 1.0));
vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

float ambiance = 0.11;
vec3 ambient = ambiance * lightColor;


vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos-Position);
vec3 reflectDir = reflect(-lightDir, norm);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

float specularStrength = 1.0;
float spec = pow(max(dot(norm, reflectDir), 0.0),16);
vec3 specular = specularStrength * spec * lightColor;

LightColor = ambient + diffuse + specular;
}
