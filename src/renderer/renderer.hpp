#include <string>
#include <unordered_map>
#include <cstdint>
#include <memory>
#include "../util/alias.hpp"

typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct TTF_Font TTF_Font;

class buffer;

struct glyph {
  int w, h;
  SDL_Texture *texture;
};

class font_map {
public:
  font_map(void) : max_w(0), max_h(0) {}
  void map_insert_defaults(TTF_Font *const font, SDL_Renderer *r);
  glyph *map_find(uint32_t ch);

  int get_max_glyph_h(void) { return max_h; }
  int get_max_glyph_w(void) { return max_w; }

private:
  int max_w, max_h;
  std::unordered_map<uint32_t, glyph> glyphs;
};

class font_container {
public:
  font_container(std::string fontpath, int fontsize);
  bool font_open(void);
  font_map &get_font_map(void) { return map; }
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
  void draw_text(const_char_mat_ptr textbuffer);
  void draw_cursor(const std::shared_ptr<buffer> buffer);
  void clear(void);
  void set_col(uint8_t r8, uint8_t g8, uint8_t b8, uint8_t a8);
  void present(void);
  bool init_renderer(SDL_Window *w);
  font_container get_font_container(void) { return fc; }
  SDL_Renderer *get_renderer(void) { return r; }

private:
  int start;
  SDL_Renderer *r;
  font_container fc;
};
