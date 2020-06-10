#version 330

in vec3 FragPos;
in vec3 Normal;

layout(location=0) out vec4 color;

uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main()
{

	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectVec = reflect(-lightDir,Normal);
	vec3 cameraVec = normalize(cameraPos - FragPos);

	float specularAmount = pow(max(dot(reflectVec,cameraVec),0),256);
	vec3 specular = specularAmount*lightColor;

	vec3 diffuse = max(dot(lightDir,Normal),0) * lightColor;
	//color = vec4(objColor*ambinentColor,1.0);
	color = vec4((ambientColor + diffuse )*objColor,1.0);
}
