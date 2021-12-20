#include "SDLGraphicsProgram.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h):m_screenWidth(w),m_screenHeight(h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;
	// Render flag

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Pong Clone",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth,
                                m_screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our objects
    m_object = new Object();
    m_object2 = new Object();
    ball = new BallObject();
    map_object = new Map();
    menu_object = new Menu();
    menu_object2 = new Menu();
    menu_object3 = new Menu();

    // Make a textured quad
    m_object->MakeTexturedQuad("white.ppm");
    m_object2->MakeTexturedQuad("white.ppm");
    map_object->MakeTexturedRect("white.ppm");
    ball->MakeTexturedSquare("white.ppm");

    menu_object->MakeTexturedMessage("title.ppm");
    menu_object2->MakeTexturedMessage("enter.ppm");
    menu_object3->MakeTexturedMessage("instruct.ppm");
    
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    // Reclaim all of our objects
    delete m_object;
    delete m_object2;
    delete map_object;
    delete menu_object;
    delete menu_object2;
    delete ball;

    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

    // Setup our OpenGL State machine
    // TODO: Read this
    // The below command is new!
    // What we are doing, is telling opengl to create a depth(or Z-buffer) 
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
	return success;
}

void SDLGraphicsProgram::MainMenu() {

    menu_object->GetTransformMessage().LoadIdentity();
    menu_object->GetTransformMessage().Translate(0.0f,0.0f,-8.0f);
    menu_object->GetTransformMessage().Rotate(9.4f,0.0f,1.0f,0.0f);
    menu_object->GetTransformMessage().Scale(2.0f,1.0f,2.0f);
    menu_object->UpdateMessage(m_screenWidth,m_screenHeight);

    menu_object2->GetTransformMessage().LoadIdentity();
    menu_object2->GetTransformMessage().Translate(0.0f,-2.0f,-8.0f);
    menu_object2->GetTransformMessage().Rotate(9.4f,0.0f,1.0f,0.0f);
    menu_object2->GetTransformMessage().Scale(0.8f,0.6f,0.8f);
    menu_object2->UpdateMessage(m_screenWidth,m_screenHeight);

}

void SDLGraphicsProgram::Start() {

	//Set the players at starting position
	offset1 = 0;
	offset2 = 0;

	velocity_x = 0;
	velocity_y = 0;

	// Initialize scores
	scoreLeft = 0;
	scoreRight = 0;

    menu_object3->GetTransformMessage().LoadIdentity();
    menu_object3->GetTransformMessage().Translate(0.0f,-0.4f,-8.0f);
    menu_object3->GetTransformMessage().Rotate(9.4f,0.0f,1.0f,0.0f);
    menu_object3->GetTransformMessage().Scale(0.9f,0.4f,0.9f);
    menu_object3->UpdateMessage(m_screenWidth,m_screenHeight);

    map_object->GetTransformMap().LoadIdentity();
    map_object->GetTransformMap().Translate(0.0f,0.0f,-8.0f);
    map_object->GetTransformMap().Scale(1.0f,0.5f,1.0f);
    map_object->UpdateMap(m_screenWidth,m_screenHeight);

	m_object->GetTransform().LoadIdentity();
    m_object->GetTransform().Translate(7.0f,-0.6f,-8.0f);
    m_object->GetTransform().Scale(2.0f,2.0f,2.0f);
    m_object->Update(m_screenWidth,m_screenHeight);

    m_object2->GetTransform().LoadIdentity();
    m_object2->GetTransform().Translate(-7.45f,-0.6f,-8.0f);
    m_object2->GetTransform().Scale(2.0f,2.0f,2.0f);
    m_object2->Update(m_screenWidth,m_screenHeight);

    ball->GetTransformBall().LoadIdentity();
    ball->GetTransformBall().Translate(-0.08f,0.0f,-8.0f);
    ball->GetTransformBall().Scale(1.0f,1.0f,1.0f);
    ball->UpdateBall(m_screenWidth,m_screenHeight);
}

// Update OpenGL
void SDLGraphicsProgram::Move(float speed1, float speed2){

    offset1 += speed1;
    offset2 += speed2;

    // Restrict players from going out of the scene
    if(offset1 > 3.0f) {
    	offset1 = 3.0f;
    }

    if(offset2 > 3.0f) {
    	offset2 = 3.0f;
    }

    if(offset1 < -4.4f) {
    	offset1 = -4.4f;
    }

    if(offset2 < -4.4f) {
    	offset2 = -4.4f;
    }
    
    m_object->GetTransform().LoadIdentity();
    m_object->GetTransform().Translate(7.0f,0.0f + offset1,-8.0f);
    m_object->GetTransform().Scale(2.0f,2.0f,2.0f);
    m_object->Update(m_screenWidth,m_screenHeight);

    m_object2->GetTransform().LoadIdentity();
    m_object2->GetTransform().Translate(-7.45f,0.0f + offset2,-8.0f);
    m_object2->GetTransform().Scale(2.0f,2.0f,2.0f);
    m_object2->Update(m_screenWidth,m_screenHeight);
}

void SDLGraphicsProgram::Update() {

	float playerHeight = 1.42f;

    static float deltaX = 0.2f;
	static float deltaY = 0.2f;

	velocity_x += deltaX;
	velocity_y += deltaY;

	if(velocity_x > 7.6f || velocity_x < -7.9f) {
		scoreLeft += 1;
		velocity_x = 0.0f;
		velocity_y = 0.0f;
        deltaX = 0.2f;
		deltaX = -deltaX;
	}
	if(velocity_x < -7.9f) {
		scoreRight += 1;
		velocity_x = 0.0f;
		velocity_y = 0.0f;
        deltaX = 0.2f;
		deltaX = -deltaX;
	}

	if(velocity_y > 4.2f || velocity_y < -4.4f) {
		deltaY = -deltaY;
	}

	ball->GetTransformBall().LoadIdentity();
    ball->GetTransformBall().Translate(0.0f + velocity_x ,0.0f + velocity_y ,-8.0f);
    ball->GetTransformBall().Scale(1.0f,1.0f,1.0f);
    ball->UpdateBall(m_screenWidth,m_screenHeight);

    // For collisions
    if(velocity_x > 6.75 && velocity_y>=offset1 && velocity_y<=(offset1+playerHeight)) {
    	deltaX = -deltaX * 1.1f;
    }

    if(velocity_x < -6.95 && velocity_y>=offset2 && velocity_y<=(offset2+playerHeight)) {
    	deltaX = -deltaX * 1.1f;
    }
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::Render(){
    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    // TODO: Read this
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Render our object
    m_object->Render();
    m_object2->Render();
    map_object->RenderMap();
    ball->RenderBall();
    menu_object->RenderMessage();
    menu_object2->RenderMessage();
    menu_object3->RenderMessage();
    // Note: This line is equivalent to:
    // (*m_object).Render(); // But the easier style
    // is to use the '->' which dereferences and then
    // selects the field/function with the '.' operator.
   
    SDL_Delay(50); 
}


//Loops forever!
void SDLGraphicsProgram::Loop(){
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    bool gameStart = false;
    bool matchStart = false;
    // Starts the scene
    MainMenu();
    Start();
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
    // While application is running
    while(!quit){

     	//Handle events on queue
		while(SDL_PollEvent( &e ) != 0){
        	// User posts an event to quit
	        // An example is hitting the "x" in the corner of the window.
    	    if(e.type == SDL_QUIT){
        		quit = true;
	        }else if(e.type == SDL_KEYDOWN) {
	        	
	        	if(keyboard_state_array[SDL_SCANCODE_UP] && !(keyboard_state_array[SDL_SCANCODE_DOWN])) {
	        		Move(0.3f, 0.0f);
	        	}else if(!(keyboard_state_array[SDL_SCANCODE_UP]) && keyboard_state_array[SDL_SCANCODE_DOWN]) {
	        		Move(-0.3f, 0.0f);
	        	}

	        	if(keyboard_state_array[SDL_SCANCODE_W] && !(keyboard_state_array[SDL_SCANCODE_S])) {
	        		Move(0.0f, 0.3f);
	        	}else if(!(keyboard_state_array[SDL_SCANCODE_W]) && keyboard_state_array[SDL_SCANCODE_S]) {
	        		Move(0.0f, -0.3f);
	        	}

	        	if(keyboard_state_array[SDL_SCANCODE_SPACE]) {
                    gameStart = true;
                    menu_object3->MakeTexturedMessage("intruct.ppm");
	        	}

                if(keyboard_state_array[SDL_SCANCODE_Q]) {
                    quit = true;
                }

                if(keyboard_state_array[SDL_SCANCODE_RETURN]) {
                    menu_object->MakeTexturedMessage("title.ppm");
                    menu_object2->MakeTexturedMessage("enter.ppm");
                }
	        }


      	} // End SDL_PollEvent loop.

		// Update our scene
		if(gameStart) {
			Update();
		}
		
		// Render using OpenGL
	    Render(); 	// TODO: potentially move this depending on your logic
					// for how you handle drawing a triangle or rectangle.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
    }

    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
