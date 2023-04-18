#ifndef OBJLOAD
#define OBJLOAD


#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "constants.h"

void load_obj(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<GLushort>& elements);

#endif