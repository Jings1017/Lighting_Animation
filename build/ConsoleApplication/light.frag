#version 330

in vec3 FragPos;
in vec3 Normal;

layout(location=0) out vec4 color;


uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform bool blinnMode;

void main(void)
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	float specularAmount = 0.0;
	
	if(false){
		vec3 halfwayDir = normalize(lightDir+viewDir);
		specularAmount = pow(max(dot(normal,halfwayDir),0.0),2.0);
	}
	else{
		vec3 reflectVec = reflect(-lightDir,normal);
		specularAmount = pow(max(dot(reflectVec,viewDir),0.0),256.0);
	}

	vec3 specular = specularAmount*lightColor;

	vec3 diffuse = max(dot(lightDir,normal),0) * lightColor;

	color = vec4((ambientColor + diffuse + specular)*objColor,1.0);
}
