#include "../svte.hpp"
#include "../util.hpp"
#include <SDL2/SDL.h>

renderer_container::renderer_container(std::string fontpath, int fontsize)
    : r(nullptr), fc(font_container(fontpath, fontsize)) {}
bool renderer_container::init_renderer(int flags, SDL_Window *w) {
  SDL_Renderer *tmp = SDL_CreateRenderer(w, -1, flags);
  if (!tmp) {
    logger::log_var("Failed to set renderer: ", SDL_GetError);
    return false;
  }

  r = tmp;
  return true;
}

void renderer_container::draw_text(const vec_2d_ptr textbuffer) { 
  if(textbuffer){
    for(size_t y = 0; y < textbuffer->size(); y++){
      const std::vector<uint32_t> &line =  (*textbuffer)[y];
      for(size_t x = 0; x < line.size(); x++){
        glyph *glyph_data = fc.get_font_map()->map_find(line[x]);
        if(glyph_data && glyph_data->texture){
          const int posx = x * glyph_data->w, posy = y * glyph_data->h;
          SDL_Rect box = { posx, posy, glyph_data->w, glyph_data->h };
          SDL_RenderCopy(r, glyph_data->texture, nullptr, &box);
        }
      }
    }
  }
}

void renderer_container::clear(void) { SDL_RenderClear(r); }

void renderer_container::set_col(void) { SDL_SetRenderDrawColor(r, 10, 0, 21, 255); }

void renderer_container::present(void) { SDL_RenderPresent(r); }
