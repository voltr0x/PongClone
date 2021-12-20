#include "Object.hpp"
#include "Error.hpp"

Object::Object(){
}

Object::~Object(){
    
}

// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void Object::MakeTexturedQuad(std::string fileName){

        // Setup geometry
        // Be careful not to forget comma's after each line
        // (except the last line of course)!
        
        // TODO:
        m_vertices = {
            0.0f, 0.75f, 0.0f,   // Position
            0.0f, 0.0f,         // Texture
            
            0.0f, 0.0f, 0.0f,   // Position
            1.0f, 0.0f,         // Texture

            0.25f, 0.75f, 0.0f,   // Position
            1.0f, 1.0f,         //Texture
            
            0.25f, 0.0f, 0.0f,   // Position
            0.0f, 1.0f          // Texture
        };

        // Add data to the indices as you see fit
        m_indices = {0,1,2, 2,1,3};

        // Create a buffer and set the stride of information
        // NOTE: We are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateTextureBufferLayout(m_vertices.size(),m_indices.size(),m_vertices.data(),m_indices.data());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        m_texture.LoadTexture(fileName.c_str());
        
        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        m_shader.CreateShader(vertexShader,fragmentShader);
       
}


void Object::Update(unsigned int screenWidth, unsigned int screenHeight){
        m_texture.Unbind();
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        m_texture.Bind();  
        m_shader.Bind();
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        m_shader.SetUniformMatrix1i("u_Texture",0);
 
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
//        modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
        m_projectionMatrix = glm::perspective(45.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);
        // Set the uniforms in our current shader
        m_shader.SetUniformMatrix4fv("modelTransformMatrix",m_transform.GetTransformMatrix());
        m_shader.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);
}

void Object::Render(){
        // Select this objects buffer to render
        m_vertexBufferLayout.Bind();
        // Select this objects texture to render
        m_texture.Bind();
        // Select this objects shader to render
        m_shader.Bind();
	    //Render data
    	glDrawElements(GL_TRIANGLES,
					m_indices.size(),  // The number of indicies, not triangles.
					GL_UNSIGNED_INT,   // Make sure the data type matches
					nullptr);          // Offset pointer to the data. nullptr
                                       // because we are currently bound:
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& Object::GetTransform(){
    return m_transform; 
}
