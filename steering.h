#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>


// camera
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern  glm::vec3 cameraUp;

extern  glm::vec3 objSpeed;
extern  float wheel_speed;

extern float fov;


void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);