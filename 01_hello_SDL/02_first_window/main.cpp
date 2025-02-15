//using SDL and standard IO
#include <SDL.h>
#include <stdio.h> //included to print errors to the console

//printf is more thread safe.

//screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
  //The window we'll be rendering to
  SDL_Window* window = NULL;

  //THe surface contained by the window
  SDL_Surface* screenSurface = NULL;

  //Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
  }
  else
  {
    //Create window                                                                                                    SDL_WINDOW_SHOWN makes sure the window is shown when it is created.
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    //The next two arguments define the x and y position the window is created in. Since we don't care where it is created, we just put SDL_WINDOWPOS_UNDEFINED for the x and y position.
    if(window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s \n", SDL_GetError());
    }
    else
    {
      //Get window surface
      screenSurface = SDL_GetWindowSurface(window);

      //Fill the surface white
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

      //Update the surface
      SDL_UpdateWindowSurface(window);

      //Hack to get window to stay up
      SDL_Event e; bool quit = false; while (quit == false){while(SDL_PollEvent(&e)){if(e.type == SDL_QUIT)quit = true;}}
    }
  }
  //Destroy window
  SDL_DestroyWindow(window);

  //Quit SDL subsystems
  SDL_Quit();
  
  return 0;
}