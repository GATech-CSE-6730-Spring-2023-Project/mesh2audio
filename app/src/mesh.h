#include <GL/glew.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace gl {
struct Mesh {
    void generate_buffers();
    void destroy_buffers();
    void parse_and_bind();
    inline void bind() { glBindVertexArray(vertex_array); }

    fs::path object_path;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

private:
    GLuint vertex_array, vertex_buffer, normal_buffer, index_buffer;
};
} // namespace gl