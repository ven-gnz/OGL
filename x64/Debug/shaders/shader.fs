#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1;
float far = 10.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // transform to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));


}



void main()
{    
    // float depth = LinearizeDepth(gl_FragCoord.z) / far;
    // I am embarassed to admit how hard this was. Had to refer to tutorial chapteres. Learning is not linear...
    FragColor = texture(texture1, TexCoords);
    
    
    
}