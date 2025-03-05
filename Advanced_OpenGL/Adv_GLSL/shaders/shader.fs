#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{    
   if(gl_FrontFacing){
	   FragColor = texture(frontTexture, TexCoords);
   } else {
	   FragColor = texture(backTexture, TexCoords);
   }

}