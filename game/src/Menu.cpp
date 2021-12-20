#include "Menu.hpp"
#include "Error.hpp"

Menu::Menu(){
}

Menu::~Menu(){
    
}

// Initialization of Menu as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our Menus at the correct time
void Menu::MakeTexturedMessage(std::string fileName){

        // Setup geometry
        // Be careful not to forget comma's after each line
        // (except the last line of course)!
        
        // TODO:
        menu_vertices = {
            -1.5f,-0.75f,0.0f,   // Position
            0.0f, 0.0f,         // Texture
            
            1.5f,-0.75f,0.0f,    // Position
            1.0f, 0.0f,         // Texture

            1.5f,0.75f,0.0f,     // Position
            1.0f, 1.0f,         //Texture
            
            -1.5f,0.75f,0.0f,    // Position
            0.0f, 1.0f          // Texture
        };

        // Add data to the indices as you see fit
        menu_indices = {0,1,2, 2,3,0};

        // Create a buffer and set the stride of information
        // NOTE: We are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        menu_vertexBufferLayout.CreateTextureBufferLayout(menu_vertices.size(),menu_indices.size(),menu_vertices.data(),menu_indices.data());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        menu_texture.LoadTexture(fileName.c_str());
        
        // Setup shaders
        std::string vertexShader = menu_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = menu_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        menu_shader.CreateShader(vertexShader,fragmentShader);
       
}


void Menu::UpdateMessage(unsigned int screenWidth, unsigned int screenHeight){
        menu_texture.Unbind();
        // Make sure we are updating the correct 'buffers'
        menu_vertexBufferLayout.Bind();
        menu_texture.Bind();  
        menu_shader.Bind();
        // For our Menu, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        menu_shader.SetUniformMatrix1i("u_Texture",0);
 
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
//        modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
        menu_projectionMatrix = glm::perspective(45.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);
        // Set the uniforms in our current shader
        menu_shader.SetUniformMatrix4fv("modelTransformMatrix",menu_transform.GetTransformMatrix());
        menu_shader.SetUniformMatrix4fv("projectionMatrix", &menu_projectionMatrix[0][0]);
}

void Menu::RenderMessage(){
        // Select this Menus buffer to render
        menu_vertexBufferLayout.Bind();
        // Select this Menus texture to render
        menu_texture.Bind();
        // Select this Menus shader to render
        menu_shader.Bind();
	    //Render data
    	glDrawElements(GL_TRIANGLES,
					menu_indices.size(),  // The number of indicies, not triangles.
					GL_UNSIGNED_INT,   // Make sure the data type matches
					nullptr);          // Offset pointer to the data. nullptr
                                       // because we are currently bound:
}

// Returns the actual transform stored in our Menu
// which can then be modified
Transform& Menu::GetTransformMessage(){
    return menu_transform; 
}