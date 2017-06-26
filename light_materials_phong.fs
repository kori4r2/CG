#version 330 core

out vec4 color;

const int NONE = 0;
const int DIRECTIONAL = 1;
const int POINT = 2;
const int SPOTLIGHT = 3;

struct Material {
	vec3 colorRGB;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float transparency;
};

struct LightSource {
	vec3 position;
	vec3 direction;
	vec3 colorRGB;
	float cutOff;
	float outerCutOff;
	float constantParameter;
	float linearParameter;
	float quadraticParameter;
	int type;
};

// Maximum number of light sources allowed
#define MAX_N_LIGHTS 8

in vec3 fragPos;
in vec3 normal;

uniform Material material;
uniform int nLights;
uniform LightSource lightSources[MAX_N_LIGHTS];
uniform vec3 viewPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

vec3 dirLight(LightSource source, vec3 fragmentNormal, vec3 viewDirection, Material material);
vec3 pointLight(LightSource source, vec3 fragmentNormal, vec3 viewDirection, vec3 fragmentPos, Material material);
vec3 spotlight(LightSource source, vec3 fragmentNormal, vec3 viewDirection, vec3 fragmentPos, Material material);

void main()
{
	vec3 fragNormal = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 colorCalc = vec3(0.0, 0.0, 0.0);

	int i;
	for(i = 0; i < nLights; i++){
		if(lightSources[i].type == DIRECTIONAL){
			colorCalc += dirLight(lightSources[i], fragNormal, viewDir, material);
		}else if(lightSources[i].type == POINT){
			colorCalc += pointLight(lightSources[i], fragNormal, viewDir, fragPos, material);
		}else if(lightSources[i].type == SPOTLIGHT){
			colorCalc += spotlight(lightSources[i], fragNormal, viewDir, fragPos, material);
		}
	}

	color = vec4(colorCalc, material.transparency);
}

vec3 dirLight(LightSource source, vec3 fragmentNormal, vec3 viewDirection, Material material){
	// Calculate needed direction vectors
	vec3 lightDirection = normalize(-source.direction);
	vec3 reflectionDir = reflect(-lightDirection, fragmentNormal);
	// Calculate base ambient, diffuse and specular
	vec3 ambient = lightAmbient * material.ambient;
	float diffAux = max(dot(fragmentNormal, lightDirection), 0.0);
	vec3 diffuse = lightDiffuse * material.diffuse * diffAux;
	float specAux = pow(max(dot(viewDirection, reflectionDir), 0.0), material.shininess);
	vec3 specular = lightSpecular * (material.specular * specAux);

	return (source.colorRGB * material.colorRGB * (ambient + diffuse + specular));
}

vec3 pointLight(LightSource source, vec3 fragmentNormal, vec3 viewDirection, vec3 fragmentPos, Material material){
	// Calculate needed direction vectors
	vec3 lightDirection = normalize(source.position - fragmentPos);
	vec3 reflectionDir = reflect(-lightDirection, fragmentNormal);
	// Calculate attenuation based on distance
	float distance = length(source.position - fragmentPos);
	float attenuation = 1.0 / (source.quadraticParameter * (distance * distance) + source.linearParameter * distance + source.constantParameter);
	// Calculate base ambient, diffuse and specular
	vec3 ambient = lightAmbient * material.ambient;
	vec3 diffuse = lightDiffuse * material.diffuse * max(dot(fragmentNormal, source.direction), 0.0);
	vec3 specular = lightSpecular * material.specular * pow(max(dot(viewDirection, reflectionDir), 0.0), material.shininess);
	// Apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (source.colorRGB * material.colorRGB * (ambient + diffuse + specular));
}

vec3 spotlight(LightSource source, vec3 fragmentNormal, vec3 viewDirection, vec3 fragmentPos, Material material){
	// Calculate needed direction vectors
	vec3 lightDirection = normalize(source.position - fragmentPos);
	vec3 reflectionDir = reflect(-lightDirection, fragmentNormal);
	// Calculate attenuation based on distance
	float distance = length(source.position - fragmentPos);
	float attenuation = 1.0 / (source.quadraticParameter * (distance * distance) + source.linearParameter * distance + source.constantParameter);
	// Calculate spotlight intensity
	float tetha	= dot(lightDirection, normalize(-source.direction));
	float intensity = clamp((tetha - source.outerCutOff) / (source.cutOff - source.outerCutOff), 0.0, 1.0);
	// Calculate base ambient, diffuse and specular
	vec3 ambient = lightAmbient * material.ambient;
	vec3 diffuse = lightDiffuse * material.diffuse * max(dot(fragmentNormal, source.direction), 0.0);
	vec3 specular = lightSpecular * material.specular * pow(max(dot(viewDirection, reflectionDir), 0.0), material.shininess);
	// Apply attenuation and intensity
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (source.colorRGB * material.colorRGB * (ambient + diffuse + specular));
}