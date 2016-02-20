/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, standard IO, and, strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <string>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// frame data
unsigned int currentFrame = 0;
unsigned int frameWidth = 64;
unsigned int frameHeight = 64;
unsigned int numFrames = 24;
unsigned int textureWidth = 0;
float frameIntervalMs = 1000.0f/(30.0f);

// Globals
GLuint texture=0; //this is a handle to our texture object
GLenum texture_format=0;
GLint nofcolors;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !initGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}

	return success;
}

/*
bool initGL()
{

	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	// Enable texture mapping
	glEnable( GL_TEXTURE_2D ); 
	
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	return success;
}*/

/*
bool initGL()
{
	glViewport(0, 0, (GLsizei) 640, (GLsizei) 480);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) 640, (GLfloat) 480, 
			0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth (1.5);

	// enable texture mapping 
	glEnable( GL_TEXTURE_2D ); 

	// set clear color and drawing color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}*/

bool initGL()
{

	bool success = true;
	GLenum error = GL_NO_ERROR;

	glViewport(0, 0, (GLsizei) 640, (GLsizei) 480);

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) 640, (GLfloat) 480, 
			0.0f, -1.0f, 1.0f);
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// set line smoothness
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth (1.5);
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	// Enable texture mapping
	glEnable( GL_TEXTURE_2D ); 
	
	//Initialize clear color
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	return success;
}

int loadImage()
{
	SDL_Surface *surface; // this surface will tell us the details of the image

	if ( (surface = SDL_LoadBMP("animation.bmp")) ) 
	{
		// Check that the image's width is a power of 2
		if ( (surface->w & (surface->w - 1)) != 0 ) 
		{
			printf("warning: image.bmp's width is not a power of 2\n");
		}

		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 ) 
		{
			printf("warning: image.bmp's height is not a power of 2\n");
		}

		//get number of channels in the SDL surface
		nofcolors=surface->format->BytesPerPixel;

		//contains an alpha channel
		if(nofcolors==4)
		{
			if(surface->format->Rmask==0x000000ff)
				texture_format=GL_RGBA;
			else
				texture_format=GL_BGRA;
		}
		else if(nofcolors==3) //no alpha channel
		{
			if(surface->format->Rmask==0x000000ff)
				texture_format=GL_RGB;
			else
				texture_format=GL_BGR;
		}
		else
		{
			printf("warning: the image is not truecolor...this will break ");
		}
		
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture );

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture );

		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D( GL_TEXTURE_2D, 0, nofcolors, surface->w, surface->h, 0,
		texture_format, GL_UNSIGNED_BYTE, surface->pixels );

		// save texture width
		textureWidth = surface->w;

		// Free the SDL_Surface only if it was successfully created
		if ( surface ) {
			SDL_FreeSurface( surface );
		}

		return 0;
	}
	else {
		printf("SDL could not load image.bmp: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )
	{
		gRenderQuad = !gRenderQuad;
	}
}

void update()
{
	static unsigned long lastTicks = SDL_GetTicks();
	static float elapsedMs = 0.0f;
	unsigned long currentTicks = SDL_GetTicks();
	elapsedMs += float(currentTicks - lastTicks);
	if(elapsedMs > frameIntervalMs) {
		elapsedMs -= frameIntervalMs;
		currentFrame = (currentFrame+1) % numFrames;
	}
	lastTicks = SDL_GetTicks();
}

void render()
{
	//Clear color buffer
	glClear( GL_COLOR_BUFFER_BIT );
	
	//Render quad
	if( gRenderQuad )
	{
		static const float animationWidthUV = float(numFrames*frameWidth)/float(textureWidth);
		static const float frameWidthUV = animationWidthUV/float(numFrames);
		//float quadScale = 0.1f;
		float quadScale = 40.0f;

		glBegin( GL_QUADS );
			glTexCoord2f(currentFrame*frameWidthUV, 0.0f);
			glVertex2f( -quadScale, -quadScale );
			glTexCoord2f((currentFrame+1)*frameWidthUV, 0.0f);
			glVertex2f( quadScale, -quadScale );
			glTexCoord2f((currentFrame+1)*frameWidthUV, 1.0f);
			glVertex2f( quadScale, quadScale );
			glTexCoord2f(currentFrame*frameWidthUV, 1.0f);
			glVertex2f( -quadScale, quadScale );
		glEnd();
		
	}	
}

void close()
{
	//Destroy window	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	
	// dispose of texture
	glDeleteTextures( 1, &texture );

	//Quit SDL subsystems
	SDL_Quit();
}

#undef main
int main( int argc, char** args )
{	
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		loadImage();
	
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;
		
		//Enable text input
		SDL_StartTextInput();

		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				//Handle keypress with current mouse position
				else if( e.type == SDL_TEXTINPUT )
				{
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );
					handleKeys( e.text.text[ 0 ], x, y );
				}
			}

			//Update world
			update();

			//Render quad
			render();
			
			//Update screen
			SDL_GL_SwapWindow( gWindow );
		}
		
		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}
