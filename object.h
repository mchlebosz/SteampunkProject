#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "read_texture.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "shaderprogram.h"
#include <vector>
#include <string>
#include <string_view>

class object {
public:
	inline object() {

	}

	inline object(const char* obj_path, std::vector<const char*> texture_paths, shader_program* sp) {
		_shader = sp;
		_Load_object(obj_path);
		
		for (const char* path : texture_paths) {
			auto texture = read_texture(path);
			_textures.push_back(texture);
		}
	}
	
	inline object(const object& other) {
		_vertices = other._vertices;
		_colors = other._colors;
		_normals = other._normals;
		_texCoords = other._texCoords;
		_textures = other._textures;
		_numVertices = other._numVertices;
		_shader = other._shader;
	}

	inline object(object&& other) noexcept {
		_vertices = std::move(other._vertices);
		_colors = std::move(other._colors);
		_normals = std::move(other._normals);
		_texCoords = std::move(other._texCoords);
		_textures = std::move(other._textures);
		_numVertices = other._numVertices;
		_shader = other._shader;

		other._numVertices = 0;
		other._shader = nullptr;
	}

	inline object& operator=(const object& other) {
		if (this == &other) return *this;

		_vertices = other._vertices;
		_colors = other._colors;
		_normals = other._normals;
		_texCoords = other._texCoords;
		_textures = other._textures;
		_numVertices = other._numVertices;
		_shader = other._shader;

		return *this;
	}

	inline object& operator=(object&& other) noexcept {
		if (this == &other) return *this;
		
		_vertices = std::move(other._vertices);
		_colors = std::move(other._colors);
		_normals = std::move(other._normals);
		_texCoords = std::move(other._texCoords);
		_textures = std::move(other._textures);
		_numVertices = other._numVertices;
		_shader = other._shader;
		
		other._numVertices = 0;
		other._shader = nullptr;
		
		return *this;
	}

	inline ~object() {
		for (auto& texture : _textures) {
			glDeleteTextures(1, &texture);
		}
	}

	inline const float* vertices() const {
		return _vertices.data();
	}

	inline const float* normals() const {
		return _normals.data();
	}

	inline const float* colors() const {
		return _colors.data();
	}

	inline const float* tex_coords() const {
		return _texCoords.data();
	}

	void draw() const {
		_Draw();
	}

private:
	inline static Assimp::Importer importer;

	inline void _Draw() const {
		const float* vertices = _vertices.data();
		const float* colors = _colors.data();
		const float* normals = _normals.data();
		const float* texCoords = _texCoords.data();

		glEnableVertexAttribArray(_shader->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(_shader->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wska¿ tablicê z danymi dla atrybutu vertex

		glEnableVertexAttribArray(_shader->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
		glVertexAttribPointer(_shader->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska¿ tablicê z danymi dla atrybutu color

		glEnableVertexAttribArray(_shader->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
		glVertexAttribPointer(_shader->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wska¿ tablicê z danymi dla atrybutu normal

		glEnableVertexAttribArray(_shader->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu texCoord
		glVertexAttribPointer(_shader->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wska¿ tablicê z danymi dla atrybutu texCoord

		for (size_t i = 0; i < _textures.size(); ++i) {
			std::string textureName = "textureMap" + std::to_string(i);
			glUniform1i(_shader->u(textureName.c_str()), i);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
		}

		glDrawArrays(GL_TRIANGLES, 0, _numVertices); //Narysuj obiekt

		glDisableVertexAttribArray(_shader->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
		glDisableVertexAttribArray(_shader->a("color"));  //Wy³¹cz przesy³anie danych do atrybutu color
		glDisableVertexAttribArray(_shader->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
		glDisableVertexAttribArray(_shader->a("texCoord0"));  //Wy³¹cz przesy³anie danych do atrybutu texCoord0
	}

	inline void _Load_object(const char* path) {
		const aiScene* warehouse_scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		auto mesh = warehouse_scene->mMeshes[0];
		_numVertices = mesh->mNumVertices;
		
		for (int i = 0; i < (int)mesh->mNumVertices; ++i) {
			// Vertices
			_vertices.push_back(mesh->mVertices[i].x);
			_vertices.push_back(mesh->mVertices[i].y);
			_vertices.push_back(mesh->mVertices[i].z);
			_vertices.push_back(1.0f);

			_normals.push_back(mesh->mNormals[i].x);
			_normals.push_back(mesh->mNormals[i].y);
			_normals.push_back(mesh->mNormals[i].z);
			_normals.push_back(0.0f);

			// Colors (Assuming you have per-vertex colors)
			if (mesh->HasVertexColors(0)) {
				_colors.push_back(mesh->mColors[0][i].r);
				_colors.push_back(mesh->mColors[0][i].g);
				_colors.push_back(mesh->mColors[0][i].b);
				_colors.push_back(1.0f);
			}

			if (mesh->HasTextureCoords(0)) {
				_texCoords.push_back(mesh->mTextureCoords[0][i].x);
				_texCoords.push_back(mesh->mTextureCoords[0][i].y);
			}
		}
	}

	unsigned int _numVertices;
	std::vector<float> _vertices;
	std::vector<float> _normals;
	std::vector<float> _colors;
	std::vector<float> _texCoords;
	std::vector<GLuint> _textures;
	shader_program* _shader;
};