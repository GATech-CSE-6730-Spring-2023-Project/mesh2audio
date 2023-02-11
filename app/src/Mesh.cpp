#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#define TINYOBJLOADER_USE_MAPBOX_EARCUT // Optional. Gives robust triangulation.
#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <iostream>

using glm::mat4;
using glm::vec3;
using glm::vec4;

tinyobj::ObjReader reader;

void LoadMesh(const string path) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    if (!reader.ParseFromFile(path, reader_config)) {
        if (!reader.Error().empty()) std::cerr << "TinyObjReader: " << reader.Error();
        return;
    }

    if (!reader.Warning().empty()) std::cout << "TinyObjReader: " << reader.Warning();
}

// todo more like https://github.com/tinyobjloader/tinyobjloader/blob/release/examples/viewer/viewer.cc
void DrawMesh() {
    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    // Set up the 3D camera
    static auto cameraPos = vec3(0.0f, 0.0f, 3.0f);
    static auto cameraFront = vec3(0.0f, 0.0f, -1.0f);
    static auto cameraUp = vec3(0.0f, 1.0f, 0.0f);

    ImGui::Begin("3D Mesh");

    // Get window dimensions
    const auto window_size = ImGui::GetWindowSize();
    // Create a view matrix using the camera parameters
    const mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // Create a projection matrix to fit the window aspect ratio
    const mat4 projection = glm::perspective(glm::radians(45.0f), window_size.x / window_size.y, 0.1f, 100.0f);
    const mat4 pv = projection * view;

    auto *draw_list = ImGui::GetWindowDrawList();
    const auto color = ImGui::GetColorU32(ImGuiCol_Button);
    const float line_thickness = 2;
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                auto vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                auto vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                auto vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    auto nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    auto ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    auto nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    auto tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    auto ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                const auto v1 = pv * vec4(vx, vy, vz, 1.0f);
                draw_list->PathLineTo({v1.x, v1.y});
                if (v % 3 == 2) draw_list->PathStroke(color, ImDrawFlags_Closed, line_thickness);
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }

    ImGui::End();
}
