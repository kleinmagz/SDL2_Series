//TRYING TO MEMORIZE SDL_RENDERER
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 460;

bool init();
bool loadMedia();
void close();

SDL_Window* global_window = NULL;
SDL_Renderer* global_renderer = NULL;
SDL_Texture* PNG_Texture = NULL;

SDL_Texture* loadTexture( std::string path );

bool init()
{
  bool success = true;

  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf("Failed to initialize SDL. SDL_Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
      printf("Warning! Unable to set to linear texture filtering.\n");
    }

    global_window = SDL_CreateWindow("SDL_TRY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if( global_window == NULL )
    {
      printf("Failed to create a window. SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED);
      if( global_renderer == NULL )
      {
        printf("Failed to create renderer. SDL_Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(global_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        int imgFlags = IMG_INIT_PNG;
        if (!IMG_Init(imgFlags) & imgFlags )
        {
          printf("Failed to initialize SDL_Image. IMG_Error: %s\n", IMG_GetError());
          success = false;
        }
      }
    }
  }
  return success;
}

bool loadMedia()
{
  bool success = true;

  PNG_Texture = loadTexture("./download.png");
  if ( PNG_Texture == NULL )
  {
    printf("Failed to load media.\n");
    success = false;
  }

  return success;
}

void close()
{
  SDL_DestroyTexture(PNG_Texture);
  PNG_Texture = NULL;

  SDL_DestroyRenderer(global_renderer);
  global_renderer = NULL;

  SDL_DestroyWindow(global_window);
  global_window = NULL;

  IMG_Quit();
  SDL_Quit();
}

SDL_Texture* loadTexture ( std::string path )
{
  SDL_Texture* loadedTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf("Failed to load surface from %s. IMG_Error: %s", path.c_str(), IMG_GetError());
  }
  else
  {
    loadedTexture = SDL_CreateTextureFromSurface( global_renderer, loadedSurface );
    if ( loadedTexture == NULL )
    {
      printf("Failed to convert surface to texture. SDL_Error: %s", SDL_GetError());
    }

    SDL_FreeSurface(loadedSurface);
  }
  return loadedTexture;
}

int main(int argc, char* args[])
{
  if(!init())
  {
    printf("Failed to initialize.\n");
  }
  else
  {
    if(!loadMedia())
    {
      printf("Failed to load media.\n");
    }
    else
    {
      bool quit = false;
      SDL_Event event;

      while ( !quit )
      {
        while ( SDL_PollEvent(&event) != 0 )
        {
          if ( event.type == SDL_QUIT )
          {
            quit = true;
          }
        }

        SDL_RenderClear(global_renderer);
        SDL_RenderCopy(global_renderer, PNG_Texture, NULL, NULL);
        SDL_RenderPresent(global_renderer);
      }
    }
  }
  close();
  return 0;
}
