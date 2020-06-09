#version 330

// Default color buffer location is 0
// If you create framebuffer your own, you need to take care of it
in vec3 FragPos;
in vec3 Normal;

layout(location=0) out vec4 color;

uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

//glBindTexture in main set value to sampler2D
void main()
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 diffuse = dot(lightDir,Normal)*lightColor;
	//color = vec4(objColor*ambinentColor,1.0);
	color = vec4((ambientColor+diffuse)*objColor,1.0);
}
