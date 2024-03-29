#pragma once

#include <vector>

#include <filesystem>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace fs = std::filesystem;

using uint = unsigned int;
using std::vector;

struct MeshInstance {
    MeshInstance();
    MeshInstance(uint num_vertices, uint num_normals, uint num_indices);
    ~MeshInstance();

    void Bind(); // Bind mesh and set up vertex attributes.
    void Clear();
    void Save(fs::path file_path) const; // Export the mesh to a .obj file.

    bool Empty() const { return Vertices.empty(); }

    void Flip(bool x, bool y, bool z);
    void Rotate(const glm::vec3 &axis, float angle);
    void Scale(const glm::vec3 &scale);
    void Center();

    void UpdateBounds(); // Updates the bounding box (`Min` and `Max`).
    void ExtrudeProfile(const vector<glm::vec2> &profile_vertices, uint slices, bool closed = false);

    vector<glm::vec3> Vertices, Normals;
    vector<uint> Indices;
    glm::vec3 Min, Max; // The bounding box of the mesh.

    uint VertexArray, VertexBuffer, NormalBuffer, IndexBuffer;
};
