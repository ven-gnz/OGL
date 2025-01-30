#version 330 core

out vec4 FragColor;

in vec3 LightColor;

uniform vec3 objectColor;

void main(){

  FragColor = vec4(objectColor * LightColor, 1.0);
}