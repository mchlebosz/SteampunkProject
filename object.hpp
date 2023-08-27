#pragma once
#include "object_data.hpp"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include "shaderprogram.h"
#include "read_texture.h"

class object {
public:
	inline object()
		: m_data(), sp(nullptr)
	{ }

	inline object(const object_data& obj_data, const std::vector<std::string_view>& textures, shader_program* shader)
		: m_data(obj_data), sp(shader)
	{ 
		_load_textures(textures);
	}

	inline object(object_data&& obj_data, const std::vector<std::string_view>& textures, shader_program* shader)
		: m_data(std::move(obj_data)), sp(shader)
	{ 
		_load_textures(textures);
	}
	
	inline object(const object& other)
		: m_data(other.m_data), sp(other.sp), m_textures(other.m_textures)
	{ }

	inline object(object&& other) noexcept
		: m_data(std::move(other.m_data)), sp(other.sp), m_textures(std::move(other.m_textures))
	{ }


	inline ~object() { }

	inline object& operator=(const object& other) {
		m_data = other.m_data;
		sp = other.sp;
		m_textures = other.m_textures;
		return *this;
	}

	inline object& operator=(object&& other) noexcept {
		m_data = std::move(other.m_data);
		sp = other.sp;
		m_textures = std::move(other.m_textures);
		return *this;
	}

	inline void draw() const {
		glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, m_data.vertices()); //Wska¿ tablicê z danymi dla atrybutu vertex

		glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
		glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, m_data.colors()); //Wska¿ tablicê z danymi dla atrybutu color

		glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, m_data.normals()); //Wska¿ tablicê z danymi dla atrybutu normal

		glEnableVertexAttribArray(sp->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu texCoord0
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, m_data.texCoords()); //Wska¿ tablicê z danymi dla atrybutu texCoord0

		for (size_t i = 0; i < m_textures.size(); ++i) {
			std::string texName = "textureMap" + std::to_string(i);
			glUniform1i(sp->u(texName.c_str()), i);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		}

		glDrawArrays(GL_TRIANGLES, 0, m_data.vertexCount()); //Narysuj obiekt

		glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
		glDisableVertexAttribArray(sp->a("color"));  //Wy³¹cz przesy³anie danych do atrybutu color
		glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
		glDisableVertexAttribArray(sp->a("texCoord0"));  //Wy³¹cz przesy³anie danych do atrybutu texCoord0
	}

	const auto& textures() const {
		return m_textures;
	}

private:
	inline void _load_textures(const std::vector<std::string_view>& textures) {
		for (const auto& tex : textures) {
			m_textures.push_back(read_texture(tex.data()));
		}
	}

	object_data m_data;
	std::vector<GLuint> m_textures;
	shader_program* sp;
};