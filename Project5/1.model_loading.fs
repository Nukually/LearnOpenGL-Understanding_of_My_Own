#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;


uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;
uniform vec3 viewPos;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
float specularStrength = 0.5;
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片段是否在阴影中
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
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
	
	float shadow=ShadowCalculation(FragPosLightSpace);
	
    vec3 result = ambient+(diffuse+specular)*(1.0-shadow);
	FragColor = vec4(result, 1.0);
}