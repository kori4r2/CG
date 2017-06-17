#version 330 core

out vec4 color;

struct Material {
	vec3 colorRGB;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float transparency;
};

in vec3 fragPos;
in vec3 normal;

uniform Material material;

void main()
{
	color = vec4(material.colorRGB, material.transparency);
}