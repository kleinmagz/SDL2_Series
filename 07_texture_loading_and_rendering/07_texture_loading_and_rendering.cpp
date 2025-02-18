/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

/*
Surfaces are image data loaded into system memory. They're accessible from the CPU, and can be manipulated without even initializing any rendering context. They're just plain old data, as if you loaded the image data into an array.

Textures are image data loaded onto the GPU. Generally speaking, once data is on the GPU is quite hard to get it back to system memory, but it's blindingly fast to render to the screen (or even onto other textures). This is the same mechanism games use for textures on 3D models and stuff.
*/

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

/*
	a renderer is an object that handles the rendering of 2d graphics to a window.
	Think of a renderer as a "drawing engine" that takes care of the low-level detailed ot rendering
	graphics. 

	If you still feel dumb. 
	"render" means to generate an image or a sequence of images from a set of data.

	a "texture" is an object that represents a 3d image or surface that can e rendered to a window or a screen. a container of sorts that holds a 2d image.
*/

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{	
		/*
		a function that sets a hinting value for SDL.
		Hints are used to control various aspects of SDL's behaviour such as: 
			>Rendering
			>Audio
			>Input
			>Video

		kind of like preferences or setting that help a program work the way you want.
		For example: when playing a game, you may want to make the graphics look better.
		You might go to the game's settings and turn up the grapahics option. 
		SDL_SetHint works the same way. kind of like assiging a value to a variable. 
		*/
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		/*
			linear texture filtering is a technique used in computer graphics to render textures
			(images) on 3d objects or 2d surfaces. Its a way to interpolate (smooth out) the texture
			pixels when they're stretched or shrunk. 

			When a texture is applied to a surface, the graphics hardware needs to determine which 
			texture pixels to use for each screen pixel. Linear texture filtering does this by: 

				1. Taking teh four nearest texture pixels (2x2 neighbourhood)
				2. Calculating a weighted average of these pixels based on their distance to teh screen
						pixel. 
				3. Using the avregae value as the final color for the screen pixel.

			SDL_HINT_RENDER_SCALE_QUALITY - controles the quality of texture scaling when rendering.
			"0" (fastest) nearest-neighbout interpolation (no smoothing).
			"1" (default) linear interpolation (some smoothing). 
			"2" (hights) Anisotropic filtering (higherst quality, most smoothing).
		*/

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			//Thik of this renderer as a "pointer" or a "handle" to the renderer engine.
			//THink of this as well like this is where the office of the renderer is. hence the context.
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture( "./texture.png" );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

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
				}
				/*
				In the main loop after the event loop, we call SDL_RenderClear. This function fills the 
				screen with the color that was last set with SDL_SetRenderDrawColor.
				*/
				//Clear screen
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
