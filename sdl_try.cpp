#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;

class LTexture
{
  public:
    LTexture();
    ~LTexture();
    void free();
    bool loadFromFile( std::string path );
    void render( int x, int y, SDL_Rect* clip = NULL);
    int getHeight();
    int getWidth();
  private:
    SDL_Texture* mTexture;
    int mHeight;
    int mWidth;
};

bool init();
bool loadMedia();
void close();
SDL_Window* gwindow = NULL;
SDL_Renderer* grenderer = NULL;
SDL_Rect spriteClips[4];
LTexture spriteSheet;

LTexture::LTexture()
{
  mTexture = NULL;
  mHeight = 0;
  mWidth = 0;
}

LTexture::~LTexture()
{
  free();
}

void LTexture::free()
{
  if ( mTexture != NULL )
  { 
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mHeight = 0;
    mWidth = 0;
  }
}

bool LTexture::loadFromFile( std::string path )
{ 
  free();
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if ( loadedSurface == NULL )
  {
    printf("Unable to load surface from %s. IMG_Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(grenderer, loadedSurface);
    if ( newTexture == NULL )
    {
      printf("Unable to convert surface to texture. SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  mTexture = newTexture;
  return mTexture != NULL;
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
  SDL_Rect renderQuad = { x, y, mWidth, mHeight};
  if ( clip != NULL )
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopy(grenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
  return mWidth;
}

int LTexture::getHeight()
{
  return mHeight;
}

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
    if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"));
    {
      printf("Warning! Unable to set to linear texture filtering.\n");
    }

    gwindow = SDL_CreateWindow("SDL_TRY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if ( gwindow == NULL )
    {
      printf("Failed to create window. SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
      if ( grenderer == NULL )
      {
        printf("Failed to create renderer. SDL_Error: %s", SDL_GetError());
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags) & imgFlags))
        {
          printf("Failed to initialize SDL_image. IMG_Error: %s", IMG_GetError());
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
  if ( !spriteSheet.loadFromFile("./dots.png") )
  {
    printf("Failed to load spriteSheet.\n");
    success = false;
  }
  else
  {
    spriteClips[ 0 ] = { 0, 0, 100, 100};
    spriteClips[ 1 ] = { 100, 0, 100, 100};
    spriteClips[ 2 ] = { 0, 100, 100, 100};
    spriteClips[ 3 ] = { 100, 100, 100, 100};
  }
  return success;
}

void close()
{
  SDL_DestroyRenderer(grenderer);
  SDL_DestroyWindow(gwindow);
  grenderer = NULL;
  gwindow = NULL;
  IMG_Quit();
  SDL_Quit();
}

int main( int argc, char* args[])
{
  if ( !init() )
  {
    printf("Failed to initialize.\n");
  }
  else
  {
    if ( !loadMedia() )
    {
      printf("Failed to load media.\n");
    }
    else
    {
      bool quit = false;
      SDL_Event event;

      while ( !quit )
      {
        while( SDL_PollEvent(&event) != 0 )
        {
          if ( event.type == SDL_QUIT )
          {
            quit = true;
          }

          SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
          SDL_RenderClear(grenderer);

          spriteSheet.render(0, 0, &spriteClips[ 0 ]);
          spriteSheet.render(SCREEN_WIDTH - spriteClips[ 1 ].w, 0, &spriteClips[ 1 ]);
          spriteSheet.render(0, SCREEN_HEIGHT - spriteClips[ 2 ].h, &spriteClips[ 2 ]);
          spriteSheet.render(SCREEN_WIDTH - spriteClips[ 3 ].w, SCREEN_HEIGHT - spriteClips[ 3 ].h, &spriteClips[ 3 ]);

          SDL_RenderPresent(grenderer);
        }
      }
    }
  }
  close();
  return 0;
}