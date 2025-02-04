#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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


    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);


    // original implementation to invert the specular mapping
    // vec3 specular = -light.specular * spec *  texture(material.specular, TexCoords).rgb;
    // After rereading the relevant paragraph, I'm convinced that for the multiplication I need the antivector of the fragments
    // that are white. Doing the signum reversion, the outcome was excpected as we took the same set of values and inverted them.
    // This was further confirmed by the sample code snippet in the solution. Shame is temporary, learning hopefully lasts longer.
    // vec3 specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords)));

    // Hmmh, the emission is not working as shown in the image. The code matches the given. What gives?
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    //Workaround : For each texel component:
    // if component is positive, sign is 1 so expression is 0
    // if component is zero, sign is 0 so expression is 0
    // if component is negative, sign is -1 so expression is -2. Since textures are repeated, this does not pose a problem.
    
    vec3 specularTexel = texture(material.specular, TexCoords).rgb;
    vec3 emission = (1.0f - sign(specularTexel)) * vec3(texture(material.emission, TexCoords));
    vec3 result =  ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}