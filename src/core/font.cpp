#include "../svte.hpp"
#include "../util.hpp"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>


void font_map::map_insert_defaults(TTF_Font *const font, SDL_Renderer *r){
  const uint32_t ASCII_START = 32;
  const uint32_t ASCII_END = 127;

  for(uint32_t c = ASCII_START; c < ASCII_END; c++){
    //Only enabling white text until I get to parsing later on
    //Get this stuff working first
    
    SDL_Color col = {255, 255, 255, 255};
    SDL_Surface *surf = nullptr;
    std::vector<char> str = utf_handler::encode_utf8(c);
    const char *encoded = str.data();

    if(!(surf= TTF_RenderUTF8_Blended(font, encoded, col))){
      logger::log_var("Failed to render char: ", SDL_GetError());
      continue;
    }

    SDL_Texture *text = nullptr;
    if(!(text == SDL_CreateTextureFromSurface(r, surf))){
      logger::log_var("Failed to create texture: ", SDL_GetError());
      SDL_FreeSurface(surf);
      continue;
    }

    SDL_FreeSurface(surf);
    glyphs[c] = text;
  }
}

font_container::font_container(std::string fontpath, int fontsize) 
  : font(nullptr), name(fontpath), size(fontsize), map(font_map()) {
  logger::log_var("Font path: ", name);
  logger::log_var("Font size: ", size);
}

bool font_container::font_open(void){
  TTF_Font *tmp = TTF_OpenFont(name.c_str(), size);
  if(!tmp){
    logger::log_var("Failed to open font: ", SDL_GetError);
    return false;
  }

  font = tmp;
  return true;
}
