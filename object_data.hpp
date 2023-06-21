#pragma once
#include <algorithm>

class object_data {
public:
	inline object_data()
		: m_vertexCount(0)
		, m_vertices(nullptr)
		, m_normals(nullptr)
		, m_texCoords(nullptr)
		, m_colors(nullptr)
	{ }

	inline object_data(size_t vertex_count)
		: m_vertexCount(vertex_count)
		, m_vertices(new float[vertex_count * 4])
		, m_normals(new float[vertex_count * 4])
		, m_texCoords(new float[vertex_count * 3])
		, m_colors(new float[vertex_count * 4])
	{ }

	inline object_data(const object_data& other) {
		m_vertexCount = other.m_vertexCount;
		m_vertices = new float[m_vertexCount * 4];
		m_normals = new float[m_vertexCount * 4];
		m_texCoords = new float[m_vertexCount * 3];
		m_colors = new float[m_vertexCount * 4];

		std::copy(other.m_vertices, other.m_vertices + m_vertexCount * 4, m_vertices);
		std::copy(other.m_normals, other.m_normals + m_vertexCount * 4, m_normals);
		std::copy(other.m_texCoords, other.m_texCoords + m_vertexCount * 3, m_texCoords);
		std::copy(other.m_colors, other.m_colors + m_vertexCount * 4, m_colors);
	}

	inline object_data(object_data&& other) noexcept {
		m_vertexCount = other.m_vertexCount;
		m_vertices = other.m_vertices;
		m_normals = other.m_normals;
		m_texCoords = other.m_texCoords;
		m_colors = other.m_colors;

		other.m_vertexCount = 0;
		other.m_vertices = nullptr;
		other.m_normals = nullptr;
		other.m_texCoords = nullptr;
		other.m_colors = nullptr;
	}

	inline ~object_data() {
		free();
	}

	inline const float* vertices() const {
		return m_vertices;
	}

	inline float* vertices() {
		return m_vertices;
	}

	inline const float* normals() const {
		return m_normals;
	}

	inline float* normals() {
		return m_normals;
	}

	inline const float* texCoords() const {
		return m_texCoords;
	}

	inline float* texCoords() {
		return m_texCoords;
	}

	inline const float* colors() const {
		return m_colors;
	}

	inline float* colors() {
		return m_colors;
	}

	inline size_t vertexCount() const {
		return m_vertexCount;
	}

	inline object_data& operator=(const object_data& other) {
		if (this == &other) return *this;
		free();
		
		m_vertexCount = other.m_vertexCount;
		copy_array(m_vertices, other.m_vertices, m_vertexCount * 4);
		copy_array(m_normals, other.m_normals, m_vertexCount * 4);
		copy_array(m_texCoords, other.m_texCoords, m_vertexCount * 3);
		copy_array(m_colors, other.m_colors, m_vertexCount * 4);

		return *this;
	}

	inline object_data& operator=(object_data&& other) noexcept {
		if (this == &other) return *this;
		free();

		m_vertexCount = other.m_vertexCount;
		m_vertices = other.m_vertices;
		m_normals = other.m_normals;
		m_texCoords = other.m_texCoords;
		m_colors = other.m_colors;

		other.m_vertexCount = 0;
		other.m_vertices = nullptr;
		other.m_normals = nullptr;
		other.m_texCoords = nullptr;
		other.m_colors = nullptr;

		return *this;
	}

private:
	void copy_array(float* dest, const float* src, size_t count) {
		std::copy(src, src + count, dest);
	}

	void free() {
		if (!m_vertices) return;

		delete[] m_vertices;
		delete[] m_normals;
		delete[] m_texCoords;
		delete[] m_colors;

		m_vertices = nullptr;
	}

	float* m_vertices;
	float* m_normals;
	float* m_texCoords;
	float* m_colors;
	size_t m_vertexCount;
};