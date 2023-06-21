#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <glew/include/GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "object_data.hpp"

inline object_data load_obj(const char* file_name) {
	std::vector<glm::fvec4> vertex_positions;
	std::vector<glm::fvec4> vertex_normals;
	std::vector<glm::fvec3> vertex_texcoords;
	
	std::vector<GLint> vertex_indices;
	std::vector<GLint> vertex_normals_indices;
	std::vector<GLint> vertex_texcoords_indices;
	
	std::ifstream file(file_name);
	
	if (!file) {
		std::cout << "Cannot open file: " << file_name << std::endl;
		exit(1);
	}

	GLint tmp_glint = 0;
	std::stringstream ss;
	std::string prefix = "";
	std::string line = "";
	glm::vec4 tmp_vec3;
	glm::vec3 tmp_vec2;
	
	while (std::getline(file, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") continue;
		
		if (prefix == "o") {
			
		}
		else if (prefix == "s") {
			
		} 
		else if (prefix == "use_mtl") {
			
		}
		else if (prefix == "v") {
			ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
			tmp_vec3.w = 1;
			vertex_positions.push_back(tmp_vec3);
		}
		else if (prefix == "vt") {
			ss >> tmp_vec2.x >> tmp_vec2.y;
			tmp_vec2.z = 1;
			vertex_texcoords.push_back(tmp_vec2);
		}
		else if (prefix == "vn") {
			ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
			tmp_vec3.w = 0;
			vertex_normals.push_back(tmp_vec3);
		}
		else if (prefix == "f") {
			int counter = 0;
			while (ss >> tmp_glint) {
				if (counter == 0) {
					vertex_indices.push_back(tmp_glint);
				}
				else if (counter == 1) {
					vertex_texcoords_indices.push_back(tmp_glint);
				}
				else if (counter == 2) {
					vertex_normals_indices.push_back(tmp_glint);
				}

				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				}
				
				if (counter > 2) {
					counter = 0;
				}
			}
		}
		else {
			// ignore
		}

		
	}
	object_data obj(vertex_indices.size());
	auto vertices = obj.vertices();
	auto texCoords = obj.texCoords();
	auto normals = obj.normals();
	auto colors = obj.colors();

	for (size_t i = 0; i < vertex_indices.size(); ++i) {
		vertices[i * 4] = vertex_positions[vertex_indices[i] - 1].x;
		vertices[i * 4 + 1] = vertex_positions[vertex_indices[i] - 1].y;
		vertices[i * 4 + 2] = vertex_positions[vertex_indices[i] - 1].z;
		vertices[i * 4 + 3] = 1;
		
		texCoords[i * 3] = vertex_texcoords[vertex_texcoords_indices[i] - 1].x;
		texCoords[i * 3 + 1] = vertex_texcoords[vertex_texcoords_indices[i] - 1].y;
		texCoords[i * 3 + 2] = 1;
		
		normals[i * 4] = vertex_normals[vertex_normals_indices[i] - 1].x;
		normals[i * 4 + 1] = vertex_normals[vertex_normals_indices[i] - 1].y;
		normals[i * 4 + 2] = vertex_normals[vertex_normals_indices[i] - 1].z;
		normals[i * 4 + 3] = 0;

		colors[i * 4] = 1.0f;
		colors[i * 4 + 1] = 1.0f;
		colors[i * 4 + 2] = 1.0f;
		colors[i * 4 + 3] = 1.0f;
	}

	return obj;
}