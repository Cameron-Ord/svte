
#include "../core/buffer.hpp"
#include "../util/util.hpp"
#include "renderer.hpp"
#include <SDL3/SDL.h>

const int CURSOR_WIDTH = 2;

renderer_container::renderer_container(std::string fontpath, int fontsize)
    : r(nullptr), fc(font_container(fontpath, fontsize)) {}
bool renderer_container::init_renderer(SDL_Window *w) {
  SDL_Renderer *tmp = SDL_CreateRenderer(w, nullptr);
  if (!tmp) {
    logger::log_str("Failed to set renderer: ", SDL_GetError());
    return false;
  }

  r = tmp;
  return true;
}

void renderer_container::draw_text(const_char_mat_ptr textbuffer) {
  int y = 0, x = 0;
  for(auto it = textbuffer->begin(); it != textbuffer->end(); it++){
    if(*it == NEW_LINE){
      x = 0, y++;
      continue;
    }
    if(*it != NEW_LINE && *it != SPACE_CHAR){
        glyph *g = fc.get_font_map().map_find(*it);
        if(g){
          const float pos_x = g->w * x;
          const float pos_y = g->h * y;
          SDL_FRect box = { pos_x, pos_y, (float)g->w, (float)g->h };
          SDL_RenderTexture(r, g->texture, NULL, &box);
        }
    }
    x++;
  }
}

void renderer_container::draw_cursor(std::shared_ptr<const buffer> buffer) {
  int y = 0, x = 0;
  const_char_mat_ptr text_buffer = buffer->const_buf();
  const int cursor = buffer->const_mutator().const_cursor().get_cursor();
  for(auto it = text_buffer->begin(); it != text_buffer->begin() + cursor; it++){
      if(*it == NEW_LINE){
          x = 0, y++;
          continue;
      }
      x++;
  }

  u32 character = *(text_buffer->begin() + cursor);
  int c_width = fc.get_font_map().get_max_glyph_w();
  int c_height = fc.get_font_map().get_max_glyph_h();
  
  if(character != NEW_LINE && character != SPACE_CHAR){
      glyph *g = fc.get_font_map().map_find(character);
      if(g){
        c_width = g->w;
        c_height = g->h;
      }
  }
  
  const float pos_x = c_width * x;
  const float pos_y = c_height * y;  
  SDL_FRect box = { pos_x, pos_y, (float)CURSOR_WIDTH, (float)c_height };
  set_col(255, 255, 255, 255);

  SDL_RenderFillRect(r, &box);
}

void renderer_container::clear(void) { SDL_RenderClear(r); }

void renderer_container::set_col(uint8_t r8, uint8_t g8, uint8_t b8, uint8_t a8) {
  SDL_SetRenderDrawColor(r, r8, g8, b8, a8);
}

void renderer_container::present(void) { SDL_RenderPresent(r); }
