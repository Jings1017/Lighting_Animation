#version 330

in vec3 FragPos;
in vec3 Normal;

layout(location=0) out vec4 color;

uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform bool blinn;

void main()
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normal = normalize(Normal);
	vec3 cameraVec = normalize(cameraPos - FragPos);
	float specularAmount;

	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir+cameraVec);
		specularAmount = pow(max(dot(normal,halfwayDir),0.0),32.0);
	}
	else
	{
		vec3 reflectVec = reflect(-lightDir,normal);
		specularAmount = pow(max(dot(reflectVec,cameraVec),0.0),8.0);
	}
	
	vec3 specular = specularAmount*lightColor;

	vec3 diffuse = max(dot(lightDir,normal),0) * lightColor;

	color = vec4((ambientColor + diffuse )*objColor,1.0);
}
