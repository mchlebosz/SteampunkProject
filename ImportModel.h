#pragma once

#ifndef IMPORT_H
#define IMPORT_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include <iostream>
#include "tiny_obj_loader.h"
#include "tinyobj_loader_opt.h"

class Model3D {
public:
	void loadObj(std::string filename, std::string mtl);

	void draw();
	void bindBuffers();

private:
    GLuint vbo, vao, ibo;


    std::vector<unsigned int> indices;
    std::vector<tinyobj::real_t> vertices;
    std::vector<tinyobj::real_t> normals;
    std::vector<tinyobj::real_t> texcoords;
    std::vector<tinyobj::real_t> colors;

};


#endif  /* IMPORT_H */