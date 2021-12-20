#ifndef MENU_HPP
#define MENU_HPP

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
class Menu{
public:
    // Object Constructor
    Menu();
    // Object destructor
    ~Menu();
    // Create a textured quad
    void MakeTexturedMessage(std::string fileName);
    // Updates and transformatinos applied to object
    void UpdateMessage(unsigned int screenWidth, unsigned int screenHeight);
    // How to draw the object
    void RenderMessage();
    // Returns an objects transform
    Transform& GetTransformMessage();
private:
    // Object vertices
    std::vector<GLfloat> menu_vertices;
    // Object indicies
    std::vector<GLuint> menu_indices;

    // For now we have one shader per object.
    Shader menu_shader;
    // For now we have one buffer per object.
    VertexBufferLayout menu_vertexBufferLayout;
    // For now we have one texture per object
    Texture menu_texture;
    // Store the objects transformations
    Transform menu_transform; 
    // Store the 'camera' projection
    glm::mat4 menu_projectionMatrix;
};


#endif