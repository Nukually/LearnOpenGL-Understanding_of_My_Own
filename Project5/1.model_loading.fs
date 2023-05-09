#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
float specularStrength = 0.5;

void main()
{    
	vec3 objectColor=vec3(texture(texture_diffuse1, TexCoords));
	vec3 ambient = light.ambient * objectColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse *diff * objectColor;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir=normalize(viewDir+lightDir);
	//vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
	vec3 specular = specularStrength * spec * light.specular;
	
    vec3 result = diffuse+ambient+specular;
	FragColor = vec4(result, 1.0);
}