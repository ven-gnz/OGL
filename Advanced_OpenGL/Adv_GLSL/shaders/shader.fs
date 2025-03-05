#version 420 core
out vec4 FragColor;

// layout (depth_greater) out float gl_FragDepth;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

//uniform vec3 cameraPos;
//uniform sampler2D frontTexture;
//uniform sampler2D backTexture;

in VS_OUT
{
vec2 TexCoords;
} fs_in;

uniform sampled2D texture;

void main()
{    
  // if(gl_FrontFacing){
//	   FragColor = texture(frontTexture, TexCoords);
//   } else {
//	   FragColor = texture(backTexture, TexCoords);
//   }
	FragColor = texture(texture, fs_in.TexCoords);
}