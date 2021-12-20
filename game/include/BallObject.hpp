#ifndef BALLOBJECT_HPP
#define BALLOBJECT_HPP

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
class BallObject{
public:
    // Object Constructor
    BallObject();
    // Object destructor
    ~BallObject();
    // Create a textured quad
    void MakeTexturedSquare(std::string fileName);
    // Updates and transformatinos applied to object
    void UpdateBall(unsigned int screenWidth, unsigned int screenHeight);
    // How to draw the object
    void RenderBall();
    // Returns an objects transform
    Transform& GetTransformBall();
private:
    // Object vertices
    std::vector<GLfloat> b_vertices;
    // Object indicies
    std::vector<GLuint> b_indices;

    // For now we have one shader per object.
    Shader b_shader;
    // For now we have one buffer per object.
    VertexBufferLayout b_vertexBufferLayout;
    // For now we have one texture per object
    Texture b_texture;
    // Store the objects transformations
    Transform b_transform; 
    // Store the 'camera' projection
    glm::mat4 b_projectionMatrix;
};


#endif