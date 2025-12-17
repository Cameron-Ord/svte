#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "alias.hpp"

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct TTF_Font TTF_Font;
typedef struct SDL_Texture SDL_Texture;

class window_container {
public:
  window_container(std::string title, int bwidth, int bheight);
  bool init_window(int flags);
  void update_window_dim(void);
  SDL_Window *get_window(void) { return w; }

private:
  std::string title;
  SDL_Window *w;
  int width, height;
};

//  Gonna essentially decode input from SDL text input into 32 bit unsigned ints
//  and use that as the lookup. Obviously dont want memory usage to explode so
//  i'll just default to ASCII code to begin with, and then if the user inputs
//  a char that isnt recognized after decoding, then add it to the map for
//  lookup. Cant really think of a better way right now, but I think this is
//  good. Atleast SDL handles the hard part for me and I can just decode the
//  information from textinputs utf8 str.

struct glyph {
  int w, h;
  SDL_Texture *texture;
};

class font_map {
public:
  font_map(void) = default;
  void map_insert_defaults(TTF_Font *const font, SDL_Renderer *r);
  glyph *map_find(uint32_t ch);

private:
  std::unordered_map<uint32_t, glyph> glyphs;
};

class font_container {
public:
  font_container(std::string fontpath, int fontsize);
  bool font_open(void);
  font_map *get_font_map(void) { return &map; }
  TTF_Font *get_font(void) { return font; }

private:
  TTF_Font *font;
  std::string name;
  int size;
  font_map map;
};

class renderer_container {
public:
  renderer_container(std::string fontpath, int fontsize);
  void draw_text(const vec_2d_ptr textbuffer);
  void clear(void);
  void set_col(void);
  void present(void);
  bool init_renderer(SDL_Window *w);
  font_container *get_font_container(void) { return &fc; }
  SDL_Renderer *get_renderer(void) { return r; }

private:
  SDL_Renderer *r;
  font_container fc;
};
