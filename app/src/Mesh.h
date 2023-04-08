#pragma once

#include "tetMesh.h" // Vega
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "MeshProfile.h"

using glm::vec3, glm::mat4;

struct ImVec2;

// Currently, this class also handles things like camera and lighting.
// If there were more than one mesh, we would move that stuff out of here.
struct Mesh {
    // Load a 3D mesh from a .obj file, or a 2D profile from a .svg file.
    Mesh(fs::path file_path);
    ~Mesh();

    void Render(int mode) const;
    void RenderProfile();
    void RenderProfileConfig();
    void Save(fs::path file_path) const; // Export the mesh to a .obj file.

    int NumIndices() const { return Indices.size(); }

    void CreateTetraheralMesh();
    void Flip(bool x, bool y, bool z); // Flip vertices across the given axes, about the center of the mesh.
    void Rotate(const vec3 &axis, float angle);
    void Center(); // Center the mesh at the origin.

    static void SetCameraDistance(float distance);
    static void UpdateCameraProjection(const ImVec2 &size);

    static const int NumLights = 5;
    inline static float Ambient[4] = {0.05, 0.05, 0.05, 1};
    inline static float Diffusion[4] = {0.2, 0.2, 0.2, 1};
    inline static float Specular[4] = {0.5, 0.5, 0.5, 1};
    inline static float LightPositions[NumLights * 4] = {0.0f};
    inline static float LightColors[NumLights * 4] = {0.0f};
    inline static float Shininess = 10;
    inline static bool CustomColor = false;

    inline static mat4 ObjectMatrix{1.f}, CameraView, CameraProjection;

    inline static float CameraDistance = 4, fov = 27;
    inline static float Bounds[6] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};

    fs::path FilePath; // Most recently loaded file path.

private:
    static void InitializeStatic(); // Initialize variables shared across all meshes.

    // Generate an axisymmetric 3D mesh by rotating the current 2D profile about the y-axis.
    // _This will have no effect if `Load(path)` was not called first to load a 2D profile._
    void ExtrudeProfile();
    void UpdateBounds(); // Updates the bounding box (`Min` and `Max`).
    void Bind(); // Bind all buffers and set up vertex attributes. Also updates the bounding box.

    // Non-empty if the mesh was generated from a 2D profile:
    std::unique_ptr<MeshProfile> Profile;

    vector<vec3> Vertices, Normals;
    vector<unsigned int> Indices;
    unsigned int VertexArray, VertexBuffer, NormalBuffer, IndexBuffer;
    vec3 Min, Max; // The bounding box of the mesh.

    std::unique_ptr<TetMesh> VolumetricMesh;
};
