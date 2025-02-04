#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // original implementation vec3 specular = -light.specular * spec *  texture(material.specular, TexCoords).rgb;
    // After rereading the relevant paragraph, I'm convinced that for the multiplication I need the antivector of the fragments
    // that are white. Doing the signum reversion, the outcome was excpected as we took the same set of values and inverted them.
    // This was further confirmed by the sample code snippet in the solution. Shame is temporary, learning hopefully lasts longer.
    vec3 specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords)));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}