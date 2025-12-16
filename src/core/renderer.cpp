#include <SDL2/SDL.h>
#include "../svte.hpp"
#include "../util.hpp"
renderer_container::renderer_container(std::string fontpath, int fontsize) : r(nullptr), fc(font_container(fontpath, fontsize)){}
bool renderer_container::init_renderer(int flags, SDL_Window *w){
  SDL_Renderer *tmp = SDL_CreateRenderer(w, -1, flags);
  if(!tmp){
    logger::log_var("Failed to set renderer: ", SDL_GetError);
    return false;
  }

  r = tmp;
  return true;
}

