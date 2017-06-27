#version 330 core
layout (location = 0) in vec3 position;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	normal = mat3(transpose(inverse(model))) * (vec3(position));
	fragPos = vec3(model * vec4(position, 1.0));

	gl_Position = projection * view * vec4(fragPos, 1.0);
}