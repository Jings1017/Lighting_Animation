#version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;


uniform mat4 model;
uniform mat4 vp;

out vec3 FragPos;
out vec3 Normal;
out vec2 fTexcoord;

void main()
{
	Normal = mat3(transpose(inverse(model)))*normal;
	FragPos = vec3(model * vec4(position,1.0f));
	fTexcoord = texcoord;
	gl_Position = vp * model * vec4(position, 1.0);
}
