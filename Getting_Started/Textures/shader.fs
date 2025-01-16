#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec2 flipped = vec2(TexCoord.x*-1,TexCoord.y);
    FragColor = mix(texture(texture1, TexCoord),texture(texture2,flipped),0.2)*vec4(ourColor, 1.0);
}