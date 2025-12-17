#include "../svte.hpp"
#include "../util.hpp"
#include <SDL3/SDL.h>

renderer_container::renderer_container(std::string fontpath, int fontsize)
    : r(nullptr), fc(font_container(fontpath, fontsize)) {}
bool renderer_container::init_renderer(SDL_Window *w) {
  SDL_Renderer *tmp = SDL_CreateRenderer(w, nullptr);
  if (!tmp) {
    logger::log_var("Failed to set renderer: ", SDL_GetError());
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
          const float posx = x * glyph_data->w;
          const float posy = y * glyph_data->h;
          const SDL_FRect box = { posx, posy, (float)glyph_data->w, (float)glyph_data->h };
          SDL_RenderTexture(r, glyph_data->texture, nullptr, &box);
        }
      }
    }
  }
}

void renderer_container::clear(void) { SDL_RenderClear(r); }

void renderer_container::set_col(void) { SDL_SetRenderDrawColor(r, 10, 0, 21, 255); }

void renderer_container::present(void) { SDL_RenderPresent(r); }
