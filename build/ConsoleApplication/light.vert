#version 330	//version declaration
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;
//layout(location=?) --> configure the vertex attributes on the CPU

// uniform variable can be viewed as a constant
// you can set the uniform variable by glUniformXXXXXXXX
// HINT: I do not use model matrix here, but you might need it
uniform mat4 model;
uniform mat4 vp;

// 'out' means vertex shader output for fragment shader
// fNormal will be interpolated before passing to fragment shader
//out vec2 fTexcoord;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	Normal = mat3(model)*normal;
	gl_Position=vp*model*vec4(position, 1.0);
}
