#pragma once

#include "tetMesh.h" // Vega
#include <glm/mat4x4.hpp>

#include <cinolib/geometry/vec_mat.h>

#include "MeshInstance.h"
#include "Material.h"
#include "MeshProfile.h"

#include "ImGuizmo.h"

using glm::vec2, glm::vec3, glm::vec4, glm::mat4;

struct ImVec2;

// Currently, this class also handles things like camera and lighting.
// If there were more than one mesh, we would move that stuff out of here.
struct Mesh {
    using Type = int;

    enum Type_ {
        MeshType_Triangular,
        MeshType_Tetrahedral,
    };

    enum RenderType_ {
        RenderType_Smooth,
        RenderType_Lines,
        RenderType_Points,
        RenderType_Mesh
    };
    using RenderType = int;

    // Load a 3D mesh from a .obj file, or a 2D profile from a .svg file.
    Mesh(fs::path file_path);
    ~Mesh();

    void Render();
    void RenderConfig();
    void RenderProfile();
    void RenderProfileConfig();
    void Save(fs::path file_path) const; // Export the active mesh to a .obj file.

    bool HasProfile() const { return Profile != nullptr; }
    void SaveProfile(fs::path file_path) const {
        if (Profile != nullptr) Profile->SaveTesselation(file_path);
    }

    const MeshInstance &GetActiveInstance() const;
    MeshInstance &GetActiveInstance();

    // Every time a tet mesh is generated, it is automatically saved to disk.
    void GenerateTetMesh();
    void LoadTetMesh(fs::path file_path);
    void LoadTetMesh(const vector<cinolib::vec3d> &vecs, const vector<vector<uint>> &polys);
    bool HasTetMesh() const { return !TetMeshPath.empty(); }
    static std::string GetTetMeshName(fs::path file_path);

    std::string GenerateDsp() const;
    std::string GenerateDspAxisymmetric() const {
        return Profile != nullptr ? Profile->GenerateDspAxisymmetric() : "";
    }
    int Num3DExcitationVertices() const { return NumExcitableVertices; }
    int Num2DExcitationVertices() const { return Profile != nullptr ? Profile->NumExcitationVertices() : 0; }

    void Flip(bool x, bool y, bool z); // Flip vertices across the given axes, about the center of the mesh.
    void Rotate(const vec3 &axis, float angle);
    void Scale(const vec3 &scale); // Scale the mesh by the given amounts.
    void Center(); // Center the mesh at the origin.

    static void SetCameraDistance(float distance);
    static void UpdateCameraProjection(const ImVec2 &size);

    inline static const int NumLights = 5;

    inline static RenderType RenderMode = RenderType_Smooth;
    inline static float Ambient[4] = {0.05, 0.05, 0.05, 1};
    inline static float Diffusion[4] = {0.2, 0.2, 0.2, 1};
    inline static float Specular[4] = {0.5, 0.5, 0.5, 1};
    inline static float LightPositions[NumLights * 4] = {0.0f};
    inline static float LightColors[NumLights * 4] = {0.0f};
    inline static float Shininess = 10;
    inline static bool CustomColor = false;
    inline static bool ShowCameraGizmo = true, ShowGrid = false, ShowGizmo = false, ShowBounds = false;
    inline static ImGuizmo::OPERATION GizmoOp{ImGuizmo::TRANSLATE};

    inline static mat4 ObjectMatrix{1.f}, CameraView, CameraProjection;

    inline static float CameraDistance = 4, fov = 27;
    inline static float Bounds[6] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};
    inline static Type ViewMeshType = MeshType_Triangular;

    inline static int NumExcitableVertices = 10;
    inline static bool ShowExcitableVertices = true; // Only shown when viewing tet mesh.

    inline static bool QualityTetMesh = true;

    fs::path TetMeshPath; // Path to the current loaded tet mesh.
    fs::path FilePath; // Most recently loaded file path.

private:
    static void InitializeStatic(); // Initialize variables shared across all meshes.
    void UpdateExcitableVertices();

    // Generate an axisymmetric 3D mesh by rotating the current 2D profile about the y-axis.
    // _This will have no effect if `Load(path)` was not called first to load a 2D profile._
    void ExtrudeProfile();

    // Non-empty if the mesh was generated from a 2D profile:
    std::unique_ptr<MeshProfile> Profile;
    struct MeshInstance TriangularMesh, TetMesh;
    Type ActiveViewMeshType = MeshType_Triangular;

    ImRect BoundsRect; // Bounds of original loaded mesh, before any transformations.

    void Bind(); // Bind active mesh instance.
    void DrawGl() const; // Draw the active mesh to the OpenGL context.
};
