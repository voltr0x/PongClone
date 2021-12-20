#include "BallObject.hpp"
#include "Error.hpp"

BallObject::BallObject(){
}

BallObject::~BallObject(){
    
}

// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void BallObject::MakeTexturedSquare(std::string fileName){

        // Setup geometry
        // Be careful not to forget comma's after each line
        // (except the last line of course)!
        
        // TODO:
        b_vertices = {
            0.0f, 0.25f, 0.0f,   // Position
            0.0f, 0.0f,         // Texture
            
            0.0f, 0.0f, 0.0f,   // Position
            1.0f, 0.0f,         // Texture

            0.25f, 0.25f, 0.0f,   // Position
            1.0f, 1.0f,         //Texture
            
            0.25f, 0.0f, 0.0f,   // Position
            0.0f, 1.0f          // Texture
        };

        // Add data to the indices as you see fit
        b_indices = {0,1,2, 2,1,3};

        // Create a buffer and set the stride of information
        // NOTE: We are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        b_vertexBufferLayout.CreateTextureBufferLayout(b_vertices.size(),b_indices.size(),b_vertices.data(),b_indices.data());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        b_texture.LoadTexture(fileName.c_str());
        
        // Setup shaders
        std::string vertexShader = b_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = b_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        b_shader.CreateShader(vertexShader,fragmentShader);
       
}


void BallObject::UpdateBall(unsigned int screenWidth, unsigned int screenHeight){
        b_texture.Unbind();
        // Make sure we are updating the correct 'buffers'
        b_vertexBufferLayout.Bind();
        b_texture.Bind();  
        b_shader.Bind();
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        b_shader.SetUniformMatrix1i("u_Texture",0);
 
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
        //modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
        b_projectionMatrix = glm::perspective(45.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);
        // Set the uniforms in our current shader
        b_shader.SetUniformMatrix4fv("modelTransformMatrix",b_transform.GetTransformMatrix());
        b_shader.SetUniformMatrix4fv("projectionMatrix", &b_projectionMatrix[0][0]);
}

void BallObject::RenderBall(){
        // Select this objects buffer to render
        b_vertexBufferLayout.Bind();
        // Select this objects texture to render
        b_texture.Bind();
        // Select this objects shader to render
        b_shader.Bind();
            //Render data
        glDrawElements(GL_TRIANGLES,
                                        b_indices.size(),  // The number of indicies, not triangles.
                                        GL_UNSIGNED_INT,   // Make sure the data type matches
                                        nullptr);          // Offset pointer to the data. nullptr
                                       // because we are currently bound:
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& BallObject::GetTransformBall(){
    return b_transform; 
}
