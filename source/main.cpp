#include <GL/glew.h>	//must be before glfw, because most header files we need are in glew
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <cstdbool>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "tiny_obj_loader.h"

#define GLM_FORCE_RADIANS

bool head_control = false;
float head_move = 0;
bool front_left_thigh_control = false;
float front_left_thigh_move = 0;
bool front_left_calf_control = false;
float front_left_calf_move = 0;
bool front_right_thigh_control = false;
float front_right_thigh_move = 0;
bool front_right_calf_control = false;
float front_right_calf_move = 0;
bool back_left_thigh_control = false;
float back_left_thigh_move = 0;
bool back_left_calf_control = false;
float back_left_calf_move = 0;
bool back_right_thigh_control = false;
float back_right_thigh_move = 0;
bool back_right_calf_control = false;
float back_right_calf_move = 0;

float jump_move = 0;
float walk_move = 0;
float rotate_move = 0;
float vertical_rotate_move = 0;
float test_move = 0;
float light_move = 0.6;
bool running = false;
bool scope_rotate = false;
float delta = 0;
bool blinnMode = false;
bool light_color_change = false;

struct object_struct{
	unsigned int program;
	unsigned int vao;
	unsigned int vbo[4];
	unsigned int texture;
	glm::mat4 model;
	object_struct() : model(glm::mat4(1.0f)){}
};

glm::vec3 cameraPos = glm::vec3(10.0f, 0.0f, 80.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(0, 0, 0);

bool firstMouse = true;
float pitch = 0.0f;
float yaw = -90.0f;
float lastX = 400.0f;
float lastY = 300.0f;
float fov = 45.0f;

std::vector<object_struct> objects;//vertex array object,vertex buffer object and texture(color) for objs
unsigned int program;
std::vector<int> indicesCount;//Number of indice of objs

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.01f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (float)yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void motion(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (head_control == false) {
			head_move -= 0.1;
			if (head_move <= -0.5) {
				head_control = true;
			}
		}
		else if (head_control == true) {
			head_move += 0.1;
			if (head_move >= 0.3) {
				head_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (front_left_thigh_control == false) {
			front_left_thigh_move -= 0.1;
			if (front_left_thigh_move <= -2) {
				front_left_thigh_control = true;
			}
		}
		else if (front_left_thigh_control == true) {
			front_left_thigh_move += 0.1;
			if (front_left_thigh_move >= 2) {
				front_left_thigh_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		if (front_left_calf_control == false) {
			front_left_calf_move -= 0.1;
			if (front_left_calf_move <= -0.5) {
				front_left_calf_control = true;
			}
		}
		else if (front_left_calf_control == true) {
			front_left_calf_move += 0.1;
			if (front_left_calf_move >= 0.5) {
				front_left_calf_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (front_right_thigh_control == false) {
			front_right_thigh_move -= 0.1;
			if (front_right_thigh_move <= -2) {
				front_right_thigh_control = true;
			}
		}
		else if (front_right_thigh_control == true) {
			front_right_thigh_move += 0.1;
			if (front_right_thigh_move >= 2) {
				front_right_thigh_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		if (front_right_calf_control == false) {
			front_right_calf_move -= 0.1;
			if (front_right_calf_move <= -0.5) {
				front_right_calf_control = true;
			}
		}
		else if (front_right_calf_control == true) {
			front_right_calf_move += 0.1;
			if (front_right_calf_move >= 0.5) {
				front_right_calf_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (back_left_thigh_control == false) {
			back_left_thigh_move -= 0.1;
			if (back_left_thigh_move <= -1) {
				back_left_thigh_control = true;
			}
		}
		else if (back_left_thigh_control == true) {
			back_left_thigh_move += 0.1;
			if (back_left_thigh_move >= 1) {
				back_left_thigh_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (back_left_calf_control == false) {
			back_left_calf_move -= 0.1;
			if (back_left_calf_move <= -0.5) {
				back_left_calf_control = true;
			}
		}
		else if (back_left_calf_control == true) {
			back_left_calf_move += 0.1;
			if (back_left_calf_move >= 0.5) {
				back_left_calf_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (back_right_thigh_control == false) {
			back_right_thigh_move -= 0.1;
			if (back_right_thigh_move <= -1) {
				back_right_thigh_control = true;
			}
		}
		else if (back_right_thigh_control == true) {
			back_right_thigh_move += 0.1;
			if (back_right_thigh_move >= 1) {
				back_right_thigh_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		if (back_right_calf_control == false) {
			back_right_calf_move -= 0.1;
			if (back_right_calf_move <= -0.5) {
				back_right_calf_control = true;
			}
		}
		else if (back_right_calf_control == true) {
			back_right_calf_move += 0.1;
			if (back_right_calf_move >= 0.5) {
				back_right_calf_control = false;
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (running == false) {
			running = true;
		}
		else {
			running = false;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotate_move -= 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotate_move += 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vertical_rotate_move += 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		vertical_rotate_move -= 0.01;
	}
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		walk_move -= 0.1;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		walk_move += 0.1;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		light_color_change = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		light_color_change = false;
	}

	// camera section
	GLfloat cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
		std::cout <<"camera pos: "<<cameraPos.x<<", "<<cameraPos.y<<", "<<cameraPos.z<<std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	//

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		test_move += 0.1;
		std::cout << test_move << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		light_move += 0.05;
	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		blinnMode = false;
		std::cout << "blinn = " << blinnMode << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		blinnMode = true;
		std::cout << "blinn = " << blinnMode << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		if (scope_rotate == false) {
			scope_rotate = true;
		}
		else {
			scope_rotate = false;
		}
	}
}

static void running_mode() {
	
	if ((int)(glfwGetTime() * 10) % 10 == 0) {
		front_left_thigh_move = 0.0;
		front_right_thigh_move = 0.0;
		back_left_thigh_move = 0.0;
		back_right_thigh_move = 0.0;
		front_left_calf_move = 0.0;
		front_right_calf_move = 0.0;
		back_left_calf_move = 0.0;
		back_right_calf_move = 0.0;
		head_move = 0.0;
		//jump_move = 0.0;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 1) {
		front_left_thigh_move = 0.5;
		front_right_thigh_move = -0.5;
		back_left_thigh_move = -0.5;
		back_right_thigh_move = 0.5;
	}
	
	else if ((int)(glfwGetTime() * 10) % 10 == 2) {
		front_left_thigh_move = 1.0;
		front_right_thigh_move = -1.0;
		back_left_thigh_move = -1.0;
		back_right_thigh_move = 1.0;
		//jump_move = 3.0;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 3) {
		front_left_calf_move = -0.5;
		front_right_calf_move = 0.5;
		back_left_calf_move = 0.5;
		back_right_calf_move = -0.5;
	}

	else if ((int)(glfwGetTime() * 10) % 10 == 4) {
		front_left_thigh_move = 0.5;
		front_right_thigh_move = -0.5;
		back_left_thigh_move = -0.5;
		back_right_thigh_move = 0.5;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 5) {
		front_left_thigh_move = 0.0;
		front_right_thigh_move = 0.0;
		back_left_thigh_move = 0.0;
		back_right_thigh_move = 0.0;
		front_left_calf_move = 0.0;
		front_right_calf_move = 0.0;
		back_left_calf_move = 0.0;
		back_right_calf_move = 0.0;
		//jump_move = 0.0;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 6) {
		front_left_thigh_move = -0.5;
		front_right_thigh_move = 0.5;
		back_left_thigh_move = 0.5;
		back_right_thigh_move = -0.5;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 7) {
		front_left_thigh_move = -1.0;
		front_right_thigh_move = 1.0;
		back_left_thigh_move = 1.0;
		back_right_thigh_move = -1.0;
		jump_move = 3.0;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 8) {
		front_left_calf_move = 0.5;
		front_right_calf_move = -0.5;
		back_left_calf_move = -0.5;
		back_right_calf_move = 0.5;
	}
	else if ((int)(glfwGetTime() * 10) % 10 == 9) {
		front_left_thigh_move = -0.5;
		front_right_thigh_move = 0.5;
		back_left_thigh_move = 0.5;
		back_right_thigh_move = -0.5;
	}
}

static unsigned int setup_shader(const char *vertex_shader, const char *fragment_shader)
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const GLchar**)&vertex_shader, nullptr);

	glCompileShader(vs);	//compile vertex shader

	int status, maxLength;
	char *infoLog = nullptr;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);		//get compile status
	if (status == GL_FALSE)								//if compile error
	{
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);	//get error message length

		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];

		glGetShaderInfoLog(vs, maxLength, &maxLength, infoLog);		//get error message

		fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] infoLog;
		return 0;
	}
	//	for fragment shader --> same as vertex shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const GLchar**)&fragment_shader, nullptr);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];

		glGetShaderInfoLog(fs, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Fragment Shader Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] infoLog;
		return 0;
	}

	unsigned int program = glCreateProgram();
	// Attach our shaders to our program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);


		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, NULL, infoLog);

		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Link Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] infoLog;
		return 0;
	}
	return program;
}

static std::string readfile(const char *filename)
{
	std::ifstream ifs(filename);
	if (!ifs)
		exit(EXIT_FAILURE);
	return std::string((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
}

// mini bmp loader written by HSU YOU-LUN
static unsigned char *load_bmp(const char *bmp, unsigned int *width, unsigned int *height, unsigned short int *bits)
{
	unsigned char *result = nullptr;
	FILE *fp = fopen(bmp, "rb");
	if (!fp)
		return nullptr;
	char type[2];
	unsigned int size, offset;
	// check for magic signature	
	fread(type, sizeof(type), 1, fp);
	if (type[0] == 0x42 || type[1] == 0x4d){
		fread(&size, sizeof(size), 1, fp);
		// ignore 2 two-byte reversed fields
		fseek(fp, 4, SEEK_CUR);
		fread(&offset, sizeof(offset), 1, fp);
		// ignore size of bmpinfoheader field
		fseek(fp, 4, SEEK_CUR);
		fread(width, sizeof(*width), 1, fp);
		fread(height, sizeof(*height), 1, fp);
		// ignore planes field
		fseek(fp, 2, SEEK_CUR);
		fread(bits, sizeof(*bits), 1, fp);
		unsigned char *pos = result = new unsigned char[size - offset];
		fseek(fp, offset, SEEK_SET);
		while (size - ftell(fp)>0)
			pos += fread(pos, 1, size - ftell(fp), fp);
	}
	fclose(fp);
	return result;
}

static int add_obj(unsigned int program, const char *filename, const char *texbmp)
{
	object_struct new_node;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, filename);

	if (!err.empty() || shapes.size() == 0)
	{
		std::cerr << err << std::endl;
		exit(1);
	}

	glGenVertexArrays(1, &new_node.vao);
	glGenBuffers(4, new_node.vbo);
	glGenTextures(1, &new_node.texture);

	glBindVertexArray(new_node.vao);

	// Upload postion array
	glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.positions.size(),
		shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // position
	if (shapes[0].mesh.texcoords.size()>0)
	{

		// Upload texCoord array
		glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.texcoords.size(),
			shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // texcoord
		glActiveTexture(GL_TEXTURE0);	//Activate texture unit before binding texture, used when having multiple texture
		glBindTexture(GL_TEXTURE_2D, new_node.texture);
		unsigned int width, height;
		unsigned short int bits;
		unsigned char *bgr = load_bmp(texbmp, &width, &height, &bits);
		GLenum format = (bits == 24 ? GL_BGR : GL_BGRA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, bgr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete[] bgr;
	}

	if (shapes[0].mesh.normals.size()>0)
	{
		// Upload normal array
		glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.normals.size(),
			shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Setup index buffer for glDrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_node.vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*shapes[0].mesh.indices.size(),
		shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	indicesCount.push_back(shapes[0].mesh.indices.size());

	glBindVertexArray(0);

	new_node.program = program;

	objects.push_back(new_node);
	return objects.size() - 1;
}

static void releaseObjects()
{
	for (int i = 0; i<objects.size(); i++){
		glDeleteVertexArrays(1, &objects[i].vao);
		glDeleteTextures(1, &objects[i].texture);
		glDeleteBuffers(4, objects[i].vbo);
	}
	glDeleteProgram(program);
}

static void setUniformMat4(unsigned int program, const std::string &name, const glm::mat4 &mat)
{
	// This line can be ignore. But, if you have multiple shader program
	// You must check if currect binding is the one you want
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, name.c_str());
	if (loc == -1) return;

	// mat4 of glm is column major, same as opengl
	// we don't need to transpose it. so..GL_FALSE
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

static void render()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 body_position;
	glm::mat4 head_position;
	glm::mat4 left_arm_position;
	glm::mat4 left_elbow_position;
	glm::mat4 right_arm_position;
	glm::mat4 right_elbow_position;
	glm::mat4 left_thigh_position;
	glm::mat4 left_calf_position;
	glm::mat4 light_src_position;
	
	for (int i = 0; i<objects.size(); i++){
		//VAO VBO are binded in add_Object function
		glUseProgram(objects[i].program);
		glBindVertexArray(objects[i].vao);
		glBindTexture(GL_TEXTURE_2D, objects[i].texture);
		//you should send some data to shader here
		GLint modelLoc = glGetUniformLocation(objects[i].program, "model");
		glm::mat4 mPosition;
		
		if (i == 0) {		//for main body
			mPosition = glm::scale(mPosition, glm::vec3(1.0f, 1.0f, 1.0f));
			mPosition = glm::translate(mPosition, glm::vec3(6.2+walk_move, 0, 0));
			mPosition = glm::rotate(mPosition, (float)rotate_move-1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			mPosition = glm::rotate(mPosition, (float)vertical_rotate_move, glm::vec3(0.0f, 0.0f, 1.0f));
			body_position = mPosition;
		}
		else if(i == 1){			//for head
			float radiusX = 0.5f;
			float radiusY = 7.5f;
			float radiusZ = -0.5f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = body_position;
			mPosition = glm::translate(mPosition, glm::vec3(X , Y, Z ));
			mPosition = glm::scale(mPosition, glm::vec3(1.0f, 1.0f, 1.0f));
			mPosition = glm::rotate(mPosition, (float)head_move, glm::vec3(0.0f, 0.0f, 1.0f));
			head_position = mPosition;
		}
		else if (i == 2) {			//for left arm
			float radiusX = 3.0f;
			float radiusY = 5.0f;
			float radiusZ = 0.0f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = body_position;
			mPosition = glm::translate(mPosition, glm::vec3(X , Y, Z));
			mPosition = glm::scale(mPosition, glm::vec3(1.0f, 1.0f, 1.0f));
			mPosition = glm::rotate(mPosition, (float)3.2+front_left_thigh_move, glm::vec3(0.0f, 0.0f, 1.0f));
			left_arm_position = mPosition;
		}
		else if (i == 3) {			//for left elbow
			float radiusX = 0.5f;
			float radiusY = 4.5f;
			float radiusZ = 0.0f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = left_arm_position;
			mPosition = glm::translate(mPosition, glm::vec3(X, Y , Z ));
			mPosition = glm::scale(mPosition, glm::vec3(0.8f, 0.8f, 0.8f));
			mPosition = glm::rotate(mPosition, (float)front_left_calf_move, glm::vec3(0.0f, 0.0f, 1.0f));
			left_elbow_position = mPosition;
		}
		else if (i == 4) {			//for right arm
			float radiusX = 3.0f;
			float radiusY = 5.0f;
			float radiusZ = -7.0f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = body_position;
			mPosition = glm::translate(mPosition, glm::vec3(X , Y, Z ));
			mPosition = glm::scale(mPosition, glm::vec3(1.0f, 1.0f, 1.0f));
			mPosition = glm::rotate(mPosition, (float)3.2+front_right_thigh_move, glm::vec3(0.0f, 0.0f, 1.0f));
			right_arm_position = mPosition;
		}
		else if (i == 5) {			//for right elbow
			float radiusX = 0.5f;
			float radiusY = 4.5f;
			float radiusZ = 0.0f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = right_arm_position;
			mPosition = glm::translate(mPosition, glm::vec3(X, Y, Z));
			mPosition = glm::scale(mPosition, glm::vec3(0.8f, 0.8f, 0.8f));
			mPosition = glm::rotate(mPosition, (float)front_right_calf_move, glm::vec3(0.0f, 0.0f, 1.0f));
			right_elbow_position = mPosition;
		}
		else if (i == 6) {			//for left thigh
			float radiusX = 3.0f;
			float radiusY = 0.5f;
			float radiusZ = -2.3f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = body_position;
			mPosition = glm::translate(mPosition, glm::vec3(X , Y , Z));
			mPosition = glm::scale(mPosition, glm::vec3(1.0f, 1.0f, 1.0f));
			mPosition = glm::rotate(mPosition, (float)3.2+back_left_thigh_move, glm::vec3(0.0f, 0.0f, 1.0f));
			left_thigh_position = mPosition;
		}
		else if (i == 7) {			//for left calf
			float radiusX = 0.0f;
			float radiusY = 4.5f;
			float radiusZ = 0.2f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = left_thigh_position;
			mPosition = glm::translate(mPosition, glm::vec3(X, Y, Z));
			mPosition = glm::scale(mPosition, glm::vec3(0.8f, 0.8f, 0.8f));
			mPosition = glm::rotate(mPosition, (float)back_left_calf_move, glm::vec3(0.0f, 0.0f, 1.0f));
			left_calf_position = mPosition;
		}
		else if (i == 8) {			//for right thigh
			float radiusX = 3.0f;
			float radiusY = 0.5f;
			float radiusZ = -4.7f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = body_position;
			mPosition = glm::translate(mPosition, glm::vec3(X, Y, Z));
			mPosition = glm::scale(mPosition, glm::vec3(1.0f, 1.0f, 1.0f));
			mPosition = glm::rotate(mPosition, (float)3.2+back_right_thigh_move, glm::vec3(0.0f, 0.0f, 1.0f));
			left_thigh_position = mPosition;
		}
		else if (i == 9) {			//for right calf
			float radiusX = 0.0f;
			float radiusY = 4.5f;
			float radiusZ = 0.2f;
			float X = radiusX;
			float Y = radiusY;
			float Z = radiusZ;
			mPosition = left_thigh_position;
			mPosition = glm::translate(mPosition, glm::vec3(X, Y, Z ));
			mPosition = glm::scale(mPosition, glm::vec3(0.8f, 0.8f, 0.8f));
			mPosition = glm::rotate(mPosition, (float)back_right_calf_move, glm::vec3(0.0f, 0.0f, 1.0f));
			left_calf_position = mPosition;
		}
		else if (i == 10) {			//for light source
			float radiusX = 10.0f;
			float radiusY = 3.0f;
			float radiusZ = 10.0f;
			float X = cos(light_move)*(radiusX);
			float Y = radiusY;
			float Z = sin(light_move)*(radiusZ);
			mPosition = body_position;
			mPosition = glm::translate(mPosition, glm::vec3(X, Y, Z));
			mPosition = glm::scale(mPosition, glm::vec3(0.2f, 0.2f, 0.2f));
			light_src_position = mPosition;
			lightPos.x = X ;
			lightPos.y = Y ;
			lightPos.z = Z ;
		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPosition));
		if (i == 10) {
			glUniform3f(glGetUniformLocation(modelLoc, "objColor"),1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform3f(glGetUniformLocation(modelLoc, "objColor"), 0.5f, 0.5f, 0.5f);
		}

		glUniform3f(glGetUniformLocation(modelLoc, "ambientColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(modelLoc, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		if (light_color_change == false) {
			if (blinnMode == false) {
				glUniform3f(glGetUniformLocation(modelLoc, "lightColor"), 0.0f, 1.0f, 1.0f);
			}
			else {
				glUniform3f(glGetUniformLocation(modelLoc, "lightColor"), 0.0001f, 1.0f, 1.0f);
			}
		}
		else {
			if (blinnMode == false) {
				glUniform3f(glGetUniformLocation(modelLoc, "lightColor"), 0.0f, sin(glfwGetTime()*0.7f), sin(glfwGetTime()*0.8f));
			}
			else {
				glUniform3f(glGetUniformLocation(modelLoc, "lightColor"), 0.0001f, sin(glfwGetTime()*0.7f), sin(glfwGetTime()*0.8f));
			}
		}
		
		glUniform3f(glGetUniformLocation(modelLoc, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
		glDrawElements(GL_TRIANGLES, indicesCount[i], GL_UNSIGNED_INT, nullptr);

	}
	glBindVertexArray(0);
}

static void scope_shift() {
	const float radius = 40.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	if (scope_rotate == true) {
		cameraPos = (glm::vec3(camX,0.0, camZ));
		cameraFront = (glm::vec3(-camX, 0.0, -camZ));
	}
}

static void createObj(unsigned int program) {
	int body = add_obj(program, "robot_body.obj", "moon.bmp");
	int head = add_obj(program, "robot_head.obj", "moon.bmp");
	int left_arm = add_obj(program, "robot_limb.obj", "moon.bmp");
	int left_elbow = add_obj(program, "robot_limb.obj", "moon.bmp");
	int right_arm = add_obj(program, "robot_limb.obj", "moon.bmp");
	int right_elbow = add_obj(program, "robot_limb.obj", "moon.bmp");
	int left_thigh = add_obj(program, "robot_limb.obj", "moon.bmp");
	int left_calf = add_obj(program, "robot_limb.obj", "moon.bmp");
	int right_thigh = add_obj(program, "robot_limb.obj", "moon.bmp");
	int right_calf = add_obj(program, "robot_limb.obj", "moon.bmp");
	int light_source = add_obj(program, "robot_head.obj", "moon.bmp");
}

int main(int argc, char *argv[])
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	// OpenGL 3.3, Mac OS X is reported to have some problem. However I don't have Mac to test
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//set hint to glfwCreateWindow, (target, hintValue)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//hint--> window not resizable,  explicit use core-profile,  opengl version 3.3
	// For Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "Simple Example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);	//set current window as main window to focus

	// This line MUST put below glfwMakeContextCurrent
	glewExperimental = GL_TRUE;		//tell glew to use more modern technique for managing OpenGL functionality
	glewInit();

	// Enable vsync
	glfwSwapInterval(1);

	// Setup input callback
	glfwSetKeyCallback(window, key_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	// load shader program
	program = setup_shader(readfile("light.vert").c_str(), readfile("light.frag").c_str());
	createObj(program);

	glEnable(GL_DEPTH_TEST);
	// prevent faces rendering to the front while they're behind other faces. 
	glCullFace(GL_BACK);
	// discard back-facing trangle
	// Enable blend mode for billboard
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//setUniformMat4(program2, "vp", glm::mat4(1.0));

	//setUniformMat4(program2, "vp", glm::mat4(1.0));
	//glm::mat4 tl = glm::translate(glm::mat4(), glm::vec3(15.0f, 0.0f, 0.0));
	//glm::mat4 rot;
	//glm::mat4 rev;
	
	float last, start;
	last = start = glfwGetTime();
	int fps = 0;
	objects[0].model = glm::scale(glm::mat4(1.0f), glm::vec3(0.85f));
	while (!glfwWindowShouldClose(window))
	{//program will keep draw here until you close the window
		float delta = glfwGetTime() - start;

		glm::mat4 see = glm::perspective(glm::radians(fov), 800.0f / 600, 1.0f, 100.f) *
			glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp) * glm::mat4(1.0f);

		setUniformMat4(program, "vp", see);

		motion(window);
		
		if (running == true) {
			running_mode();
		}

		scope_shift();

		render();
		glfwSwapBuffers(window);	//swap the color buffer and show it as output to the screen.
		glfwPollEvents();			//check if there is any event being triggered
		fps++;
		if (glfwGetTime() - last > 1.0)
		{
			fps = 0;
			last = glfwGetTime();
		}
	}

	releaseObjects();
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}