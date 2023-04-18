#define TINYOBJLOADER_IMPLEMENTATION
#include "ImportModel.h"

void Model3D::loadObj(std::string inputfile, std::string mtl = "./") {

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = mtl; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
                vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
                vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

                if (idx.normal_index >= 0) {
                    normals.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
                    normals.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
                    normals.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);
                }

                if (idx.texcoord_index >= 0) {
                    texcoords.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
                    texcoords.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
                }

                if (attrib.colors.size() > 0) {
                    colors.push_back(attrib.colors[3 * size_t(idx.vertex_index) + 0]);
                    colors.push_back(attrib.colors[3 * size_t(idx.vertex_index) + 1]);
                    colors.push_back(attrib.colors[3 * size_t(idx.vertex_index) + 2]);
                }
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
    std::cout << "Vertices: " << vertices.size() / 4 << std::endl;
    std::cout << "Normals: " << normals.size() / 4 << std::endl;
    std::cout << "Texcoords: " << texcoords.size() / 2 << std::endl;
    std::cout << "Colors: " << colors.size() / 3<< std::endl;
}

void Model3D::bindBuffers() {
    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Fill the vertex buffer object with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertices.size() + normals.size()), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), sizeof(float) * normals.size(), &normals[0]);

    // Set up the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * vertices.size()));

    // Generate and bind the index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Unbind the vertex array object
    glBindVertexArray(0);
}

void Model3D::draw() {
    // Bind the vertex array object
    glBindVertexArray(vao);

    // Draw the triangles using the indices vector
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the vertex array object
    glBindVertexArray(0);
}