#version 330

in vec3 FragPos;
in vec3 Normal;
in vec2 fTexcoord;


layout(location=0) out vec4 color;

uniform sampler2D uSampler;
uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	float specularAmount = 0.0;
	float gamma = 1.5;

	if(lightColor.x>0){
		vec3 halfwayDir = normalize(lightDir + viewDir);
		specularAmount = pow(max(dot(normal, halfwayDir),0.0),16.0);
	}
	else{
		vec3 reflectVec = reflect(-lightDir,normal);
		specularAmount = pow(max(dot(reflectVec, viewDir),0.0),64.0);
	}

	vec3 specular = specularAmount * lightColor;
	vec3 diffuse = max(dot(lightDir, normal), 0) * lightColor;

	color = vec4((ambientColor + diffuse + specular) * objColor,1.0)*texture( uSampler,fTexcoord);
}
