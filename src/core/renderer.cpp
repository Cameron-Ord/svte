#include "../buffer.hpp"
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
  if (textbuffer) {
    for (size_t y = 0; y < textbuffer->size(); y++) {
      const std::vector<uint32_t> &line = (*textbuffer)[y];
      for (size_t x = 0; x < line.size(); x++) {
        glyph *glyph_data = fc.get_font_map()->map_find(line[x]);
        if (glyph_data && glyph_data->texture) {
          const float posx = x * glyph_data->w;
          const float posy = y * glyph_data->h;
          const SDL_FRect box = {posx, posy, (float)glyph_data->w, (float)glyph_data->h};
          SDL_RenderTexture(r, glyph_data->texture, nullptr, &box);
        }
      }
    }
  }
}

void renderer_container::draw_cursor(const std::shared_ptr<buffer> buffer) {
  const uint32_t *root_char = buffer->char_at_cursor();
  glyph *glyph_data = nullptr;
  if (root_char && (glyph_data = fc.get_font_map()->map_find(*root_char))) {
    const int posx = buffer->get_curs_x() * glyph_data->w;
    const int posy = buffer->get_curs_y() * glyph_data->h;
    SDL_FRect box = {(float)posx, (float)posy, (float)glyph_data->w, (float)glyph_data->h};
    set_col(255, 255, 255, 255);
    SDL_RenderFillRect(r, &box);
  } else {
    const int h = fc.get_font_map()->get_max_glyph_h();
    const int w = fc.get_font_map()->get_max_glyph_w();

    const int posx = buffer->get_curs_x() * w;
    const int posy = buffer->get_curs_y() * h;
    SDL_FRect box = {(float)posx, (float)posy, (float)w, (float)h};
    set_col(255, 255, 255, 255);
    SDL_RenderFillRect(r, &box);
  }
}

void renderer_container::clear(void) { SDL_RenderClear(r); }

void renderer_container::set_col(uint8_t r8, uint8_t g8, uint8_t b8, uint8_t a8) {
  SDL_SetRenderDrawColor(r, r8, g8, b8, a8);
}

void renderer_container::present(void) { SDL_RenderPresent(r); }
