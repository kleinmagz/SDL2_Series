#include <stdio.h>
#include <SDL.h>
#include <SDL_Image.h>
#include <string>

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 460;

SDL_Window* gwindow = NULL;
SDL_Surface* gscreenSurface = NULL;
SDL_Surface* PNGSurface = NULL;

SDL_Surface* loadSurface( std::string path );

bool init();
bool loadMedia();
void close();

bool init()
{
  bool success = true;
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf("Failed to initialize SDL. SDL_Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    int imgFlag = IMG_INIT_PNG;
    if( !(IMG_Init(imgFlag) & IMG_INIT_PNG) )
    {
      printf("Failed to initialize SDL_Image. IMG_Error: %s", IMG_GetError());
      success = false;
    }
    else
    {
      gwindow = SDL_CreateWindow("SDL_TRY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

      if ( gwindow == NULL)
      {
        printf("Failed to create window. SDL_Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        gscreenSurface = SDL_GetWindowSurface(gwindow);
      }
    }
  }
  return success;
}

bool loadMedia()
{
  bool success = true;
  PNGSurface = loadSurface("./download.png");
  if ( PNGSurface == NULL)
  {
    printf("Failed to load image. SDL_Error: %s\n", SDL_GetError());
    success = false;
  }
  return success;
}

void close()
{
  SDL_FreeSurface(PNGSurface);
  PNGSurface = NULL;

  SDL_DestroyWindow(gwindow);
  gwindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
  SDL_Surface* optimizedSurface = NULL;
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if ( loadedSurface == NULL )
  {
    printf("Failed to load image from %s. IMG_Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, gscreenSurface->format, 0);
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
  }
  return optimizedSurface;
}

int main(int argc, char* args[])
{
  if (!init())
  {
    printf("Failed to initialize.\n");
  }
  else
  {
    if (!loadMedia())
    {
      printf("Failed to load media.\n");
    }
    else
    {
      bool quit = false;
      SDL_Event event;

      while( !quit )
      {
        while( SDL_PollEvent(&event) != 0 )
        {
          if (event.type == SDL_QUIT )
          {
            quit = true;
          }
        }

        SDL_BlitSurface(PNGSurface, NULL, gscreenSurface, NULL);
        SDL_UpdateWindowSurface(gwindow);
      }
    }
  }
  close();
  return 0;
}

