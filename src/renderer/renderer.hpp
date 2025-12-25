#include "../util/alias.hpp"
#include <string>
#include <unordered_map>

typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct TTF_Font TTF_Font;
typedef struct SDL_FRect SDL_FRect;

class buffer;

struct glyph {
  i32 w, h;
  SDL_Texture *texture;
};

class font_map {
public:
  font_map(void) : max_w(0), max_h(0) {}
  void map_insert_defaults(TTF_Font *const font, SDL_Renderer *r);
  glyph *map_find(u32 ch);

  i32 get_max_glyph_h(void) { return max_h; }
  i32 get_max_glyph_w(void) { return max_w; }

  void free_glyph_textures(void);

private:
  i32 max_w, max_h;
  std::unordered_map<u32, glyph> glyphs;
};

class font_container {
public:
  font_container(std::string fontpath, f32 fontsize);
  bool font_open(void);
  font_map &get_font_map(void) { return map; }
  TTF_Font *get_font(void) { return font; }
  void free_font(void);

private:
  TTF_Font *font;
  std::string name;
  f32 size;
  font_map map;
};

class renderer_container {
public:
  renderer_container(std::string fontpath, f32 fontsize);
  void draw_text(const_char_mat_ptr textbuffer);
  void draw_cursor(std::shared_ptr<const buffer> buffer);
  void clear(void);
  void render_copy(SDL_Texture *texture, SDL_FRect *dstrect);
  void fill_rect(SDL_FRect *dstrect);
  void set_col(u8 r8, u8 g8, u8 b8, u8 a8);
  void present(void);
  bool init_renderer(SDL_Window *w);
  void free_renderer(void);
  font_container &get_font_container(void) { return fc; }
  SDL_Renderer *get_renderer(void) { return r; }

private:
  i32 start;
  SDL_Renderer *r;
  font_container fc;
};
