#include "Map.hpp"
#include "Error.hpp"

Map::Map(){
}

Map::~Map(){
    
}

// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void Map::MakeTexturedRect(std::string fileName){

        // Setup geometry
        // Be careful not to forget comma's after each line
        // (except the last line of course)!
        
        // TODO:
        map_vertices = {
            0.0f, 9.0f, 0.0f,   // Position
            0.0f, 0.0f,         // Texture
            
            0.0f,-9.0f, 0.0f,   // Position
            1.0f, 0.0f,         // Texture

            0.1f,9.0f, 0.0f,   // Position
            1.0f, 1.0f,         //Texture
            
            0.1f,-9.0f, 0.0f,   // Position
            0.0f, 1.0f          // Texture
        };

        // Add data to the indices as you see fit
        map_indices = {0,1,2, 2,1,3};

        // Create a buffer and set the stride of information
        // NOTE: We are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        map_vertexBufferLayout.CreateTextureBufferLayout(map_vertices.size(),map_indices.size(),map_vertices.data(),map_indices.data());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        map_texture.LoadTexture(fileName.c_str());
        
        // Setup shaders
        std::string vertexShader = map_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = map_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        map_shader.CreateShader(vertexShader,fragmentShader);
       
}


void Map::UpdateMap(unsigned int screenWidth, unsigned int screenHeight){
        map_texture.Unbind();
        // Make sure we are updating the correct 'buffers'
        map_vertexBufferLayout.Bind();
        map_texture.Bind();  
        map_shader.Bind();
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        map_shader.SetUniformMatrix1i("u_Texture",0);
 
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
        //modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
        map_projectionMatrix = glm::perspective(45.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);
        // Set the uniforms in our current shader
        map_shader.SetUniformMatrix4fv("modelTransformMatrix",map_transform.GetTransformMatrix());
        map_shader.SetUniformMatrix4fv("projectionMatrix", &map_projectionMatrix[0][0]);
}

void Map::RenderMap(){
        // Select this objects buffer to render
        map_vertexBufferLayout.Bind();
        // Select this objects texture to render
        map_texture.Bind();
        // Select this objects shader to render
        map_shader.Bind();
            //Render data
        glDrawElements(GL_TRIANGLES,
                                        map_indices.size(),  // The number of indicies, not triangles.
                                        GL_UNSIGNED_INT,   // Make sure the data type matches
                                        nullptr);          // Offset pointer to the data. nullptr
                                       // because we are currently bound:
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& Map::GetTransformMap(){
    return map_transform; 
}