
#include "renderer.hpp"
#include "../core/buffer.hpp"
#include "../util/util.hpp"
#include <SDL3/SDL.h>

const i32 CURSOR_WIDTH = 2;

void renderer_container::free_renderer(void) {
  if (r) {
    SDL_DestroyRenderer(r);
  }
}

renderer_container::renderer_container(std::string fontpath, f32 fontsize)
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
  i32 y = 0, x = 0;
  for (auto it = textbuffer->begin(); it != textbuffer->end(); it++) {
    if (*it == NEW_LINE) {
      x = 0, y++;
      continue;
    }
    if (*it != NEW_LINE && *it != SPACE_CHAR) {
      glyph *g = fc.get_font_map().map_find(*it);
      if (g) {
        const f32 rect_x = static_cast<f32>(g->w * x);
        const f32 rect_y = static_cast<f32>(g->h * y);
        const f32 rect_w = static_cast<f32>(g->w);
        const f32 rect_h = static_cast<f32>(g->h);
        SDL_FRect box = {rect_x, rect_y, rect_w, rect_h};
        render_copy(g->texture, &box);
      }
    }
    x++;
  }
}

void renderer_container::draw_cursor(std::shared_ptr<const buffer> buffer) {
  i32 y = 0, x = 0;
  const_char_mat_ptr text_buffer = buffer->const_buf();
  const i32 text_length = static_cast<i32>(text_buffer->size());
  const buf_cursor &cursor = buffer->const_mutator().const_cursor();
  for (auto it = text_buffer->begin(); it != text_buffer->begin() + cursor.get_cursor(); it++) {
    if (*it == NEW_LINE) {
      x = 0, y++;
      continue;
    }
    x++;
  }

  i32 c_width = fc.get_font_map().get_max_glyph_w();
  i32 c_height = fc.get_font_map().get_max_glyph_h();
  if (cursor.within_bounds(cursor.get_cursor(), text_length)) {
    u32 character = *(text_buffer->begin() + cursor.get_cursor());
    if (character != NEW_LINE && character != SPACE_CHAR) {
      glyph *g = fc.get_font_map().map_find(character);
      if (g) {
        c_width = g->w;
        c_height = g->h;
      }
    }
  }

  const f32 rect_x = static_cast<f32>(c_width * x);
  const f32 rect_y = static_cast<f32>(c_height * y);
  const f32 rect_w = static_cast<f32>(CURSOR_WIDTH);
  const f32 rect_h = static_cast<f32>(c_height);

  SDL_FRect box = {rect_x, rect_y, rect_w, rect_h};
  set_col(255, 255, 255, 255);
  fill_rect(&box);
}

void renderer_container::fill_rect(SDL_FRect *rect) { SDL_RenderFillRect(r, rect); }

void renderer_container::render_copy(SDL_Texture *texture, SDL_FRect *dstrect) {
  SDL_RenderTexture(r, texture, NULL, dstrect);
}

void renderer_container::clear(void) { SDL_RenderClear(r); }

void renderer_container::set_col(u8 r8, u8 g8, u8 b8, u8 a8) { SDL_SetRenderDrawColor(r, r8, g8, b8, a8); }

void renderer_container::present(void) { SDL_RenderPresent(r); }
