
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
}

void renderer_container::draw_cursor(const std::shared_ptr<buffer> buffer) {
}

void renderer_container::clear(void) { SDL_RenderClear(r); }

void renderer_container::set_col(uint8_t r8, uint8_t g8, uint8_t b8, uint8_t a8) {
  SDL_SetRenderDrawColor(r, r8, g8, b8, a8);
}

void renderer_container::present(void) { SDL_RenderPresent(r); }
