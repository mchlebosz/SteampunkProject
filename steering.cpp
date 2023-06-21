#include "steering.h"
#include "constants.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 objSpeed = glm::vec3(0.0f, 0.0f, 0.0f);
float wheel_speed = 0.0f;

bool cursorVisible = false;
const float modSpeed = 50.0f;
const float cameraSpeed = 10.0f;
const float sensitivity = 0.1f;

float yaw = -90.0f;
float pitch = 0.0f;

float fov = 60.0f;

glm::vec3 speedA = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 speedD = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 speedW = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 speedS = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 speedSpace = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 speedShift = glm::vec3(0.0f, 0.0f, 0.0f);


void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	float time = glfwGetTime() * 100;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		speedW = cameraSpeed * cameraFront * glm::vec3(1.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		speedS = -cameraSpeed * cameraFront * glm::vec3(1.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		speedA = glm::normalize(glm::cross(cameraFront, cameraUp)) * -cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		speedD = glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	if (action == GLFW_PRESS) {

		if (key == GLFW_KEY_SPACE) speedSpace = glm::vec3(0.0f, cameraSpeed, 0.0f);
		if (key == GLFW_KEY_LEFT_SHIFT) speedShift = glm::vec3(0.0f, -cameraSpeed, 0.0f);
		if (key == GLFW_KEY_RIGHT) wheel_speed = -modSpeed;
		if (key == GLFW_KEY_LEFT) wheel_speed = modSpeed;
		//if (key == GLFW_KEY_U) 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//if (key == GLFW_KEY_I) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//if (key == GLFW_KEY_O) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//if (key == GLFW_KEY_P) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//if (key == GLFW_KEY_H) 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//if (key == GLFW_KEY_J) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//if (key == GLFW_KEY_K) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//if (key == GLFW_KEY_L) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		/*if (key == GLFW_KEY_H) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,	GL_MIRRORED_REPEAT);
		if (key == GLFW_KEY_J) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		if (key == GLFW_KEY_K) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		if (key == GLFW_KEY_L) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (key == GLFW_KEY_V) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	GL_CLAMP_TO_EDGE);
		if (key == GLFW_KEY_B) glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_CLAMP_TO_EDGE);
		if (key == GLFW_KEY_N) glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_CLAMP_TO_BORDER);
		if (key == GLFW_KEY_M) glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_CLAMP_TO_BORDER);*/
		if (key == GLFW_KEY_U) 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (key == GLFW_KEY_I) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		if (key == GLFW_KEY_O) glTexParameterf(GL_TEXTURE_2D,	GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		if (key == GLFW_KEY_P) glTexParameterf(GL_TEXTURE_2D,	GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);



		
	}
	if (action == GLFW_RELEASE) {

		if (key == GLFW_KEY_W) speedW = glm::vec3(0.0f, 0.0f, 0.0f);
		if (key == GLFW_KEY_S) speedS = glm::vec3(0.0f, 0.0f, 0.0f);
		if (key == GLFW_KEY_A) speedA = glm::vec3(0.0f, 0.0f, 0.0f);
		if (key == GLFW_KEY_D) speedD = glm::vec3(0.0f, 0.0f, 0.0f);

		if (key == GLFW_KEY_SPACE) speedSpace = glm::vec3(0.0f, 0.0f, 0.0f);
		if (key == GLFW_KEY_LEFT_SHIFT) speedShift = glm::vec3(0.0f, 0.0f, 0.0f);
		if (key == GLFW_KEY_RIGHT) wheel_speed = 0.0f;
		if (key == GLFW_KEY_LEFT) wheel_speed = 0.0f;

		if (key == GLFW_KEY_ESCAPE) {
			if (cursorVisible) {
				cursorVisible = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				cursorVisible = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}

	}

	objSpeed = speedW + speedS + speedA + speedD + speedSpace + speedShift;

}

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);	
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 5.0f)
		fov = 5.0f;
	if (fov > 100.0f)
		fov = 100.0f;
}
