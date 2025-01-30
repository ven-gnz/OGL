#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

uniform vec3 lightPos; // world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0); // Get the model coordinate to clip space
	FragPos = vec3(view * model * vec4(aPos, 1.0)); // convert the fragment position to view space
	Normal = mat3(transpose(inverse( view * model ))) * aNormal; // Transform normal matrix to view space
  LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform light position to view space
  
}