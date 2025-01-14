#version 330 core
out vec4 FragColor;
in vec4 offsetPos;

void main()
{
    FragColor = vec4(offsetPos.x,offsetPos.y,offsetPos.z,1.0);
}