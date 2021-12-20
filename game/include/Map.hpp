#ifndef MAP_HPP
#define MAP_HPP

#include <glad/glad.h>

#include <vector>
#include <string>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Map{
public:
    // Object Constructor
    Map();
    // Object destructor
    ~Map();
    // Create a textured quad
    void MakeTexturedRect(std::string fileName);
    // Updates and transformatinos applied to object
    void UpdateMap(unsigned int screenWidth, unsigned int screenHeight);
    // How to draw the object
    void RenderMap();
    // Returns an objects transform
    Transform& GetTransformMap();
private:
    // Object vertices
    std::vector<GLfloat> map_vertices;
    // Object indicies
    std::vector<GLuint> map_indices;

    // For now we have one shader per object.
    Shader map_shader;
    // For now we have one buffer per object.
    VertexBufferLayout map_vertexBufferLayout;
    // For now we have one texture per object
    Texture map_texture;
    // Store the objects transformations
    Transform map_transform; 
    // Store the 'camera' projection
    glm::mat4 map_projectionMatrix;
};


#endif