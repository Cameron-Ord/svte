#include "../svte.hpp"
#include "../util.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void font_map::map_insert_defaults(TTF_Font *const font, SDL_Renderer *r) {
  const uint32_t ASCII_START = 32;
  const uint32_t ASCII_END = 127;

  for (uint32_t c = ASCII_START; c < ASCII_END; c++) {
    // Only enabling white text until I get to parsing later on
    // Get this stuff working first

    SDL_Color col = {255, 255, 255, 255};
    std::vector<char> str = utf_handler::encode_utf8(c);
    const char *encoded = str.data();
    glyph g = { 0, 0, nullptr };

    SDL_Surface *surf = TTF_RenderText_Blended(font, encoded, 0, col);
    if (!surf) {
      logger::log_var("Failed to render char: ", SDL_GetError());
      continue;
    }
    g.w = surf->w;
    g.h = surf->h;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surf);
    if (!texture) {
      logger::log_var("Failed to create texture: ", SDL_GetError());
      SDL_DestroySurface(surf);
      continue;
    }
    g.texture = texture;
    SDL_DestroySurface(surf);

    glyphs[c] = g;
  }
}

glyph *font_map::map_find(uint32_t character){
  auto it = glyphs.find(character);
  if(it != glyphs.end()){
    return &it->second;
  } else {
    return nullptr;
  }
}

font_container::font_container(std::string fontpath, int fontsize)
    : font(nullptr), name(fontpath), size(fontsize), map(font_map()) {
  logger::log_var("Font path: ", name);
  logger::log_var("Font size: ", size);
}

bool font_container::font_open(void) {
  TTF_Font *tmp = TTF_OpenFont(name.c_str(), size);
  if (!tmp) {
    logger::log_var("Failed to open font: ", SDL_GetError());
    return false;
  }

  font = tmp;
  return true;
}
