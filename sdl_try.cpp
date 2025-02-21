#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Window* gwindow = NULL;
SDL_Renderer* grenderer = NULL;

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

    gwindow = SDL_CreateWindow("SDL_TRY", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if( gwindow == NULL )
    {
      printf("Failed to create window. SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
      if( grenderer == NULL )
      {
        printf("Failed to create renderer. SDL_Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        int imgFlags = IMG_INIT_PNG;
        if( !IMG_Init(imgFlags) & imgFlags )
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
  return success;
}

void close()
{
  SDL_DestroyRenderer(grenderer);
  grenderer = NULL;
  SDL_DestroyWindow(gwindow);
  gwindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char* args[])
{
  if( !init() )
  {
    printf("Failed to initialize.\n");
  }
  else
  {
    if( !loadMedia() )
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
          if( event.type == SDL_QUIT)
          {
            quit = true;
          }
        }
        SDL_Delay(500);
        SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(grenderer);
        SDL_RenderPresent(grenderer);

        SDL_Delay(500);
        SDL_Rect rectangle = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        SDL_SetRenderDrawColor(grenderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(grenderer, &rectangle);
        //SDL_Delay(2000);
        SDL_RenderPresent(grenderer);

        SDL_Delay(500);
        SDL_SetRenderDrawColor(grenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(grenderer);
        SDL_RenderPresent(grenderer);

        SDL_Delay(500);
        SDL_Rect rectangle_outline = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3 , SCREEN_HEIGHT * 2 / 3 };
        SDL_SetRenderDrawColor(grenderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawRect(grenderer, &rectangle_outline);
        SDL_RenderPresent(grenderer);

      }
    }
  }
  close();
  return 0;
}