#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <string>

class Window;

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

struct SDL_Window;
typedef struct SDL_Window SDL_Window;

class Chars;
class Buffer;

#include <SDL2/SDL_rect.h>

typedef enum { RNDR_STATE_BAD = 0, RNDR_STATE_OK = 1, RNDR_NO_VP} RENDER_STATE;


struct Buffer_Viewport {
  //ID of buffer, each buffer gets assigned its own viewport.
  //This can be done later as I have yet to actually implement multiple buffers.. but the groundwork is laid out.
  SDL_Rect viewport;
  SDL_Rect sviewport;
  //gets set once a condition is reached. Ex: reached end of row that is being drawn. either increments by one or gets set to a fixed value. Start y loop from this value.
  int curs_row_start_position;
  //For navigating lines that are larger than the window width. Follows the cursor. Start x loop from this value. 
  int curs_col_start_position; 
};

struct TTF_Font;
typedef struct TTF_Font TTF_Font;

#define ASCII_TABLE_SIZE 128
#define STRBUF_SIZE 2

struct Ascii_Char
{
    SDL_Texture *t;
    int width, height;
};
typedef struct Ascii_Char Ascii_Char;

struct Fonts
{
    TTF_Font *base;
};
typedef struct Fonts Fonts;

struct Char_Table
{
    unsigned int c;
    char str[STRBUF_SIZE];
    Ascii_Char base;
};
typedef struct Char_Tables Char_Tables;

class Chars
{
  public:
    Chars();
    TTF_Font *ch_open_font(const char *fn, const int size);
    const Fonts *ch_get_fonts(void) { return &f; }
    const void *ch_set_font(TTF_Font *f);
    Char_Table *ch_get_tbls(void) { return chtbls; }
    Char_Table *ch_lookup(unsigned char c);
    int ch_create_textures(SDL_Renderer *rend);
    const int *ch_max_height(void) { return &max_height; }
    const int *ch_max_width(void) { return &max_width; }

  private:
    Fonts f;
    Char_Table chtbls[ASCII_TABLE_SIZE];
    void set_table(void);
    int max_height, max_width;
};

class Renderer
{
  public:
    Renderer(SDL_Window *w, const int *width, const int *height);
    ~Renderer(void);
    const int get_state(void) { return STATE; }
    SDL_Renderer *get_renderer(void);
    const void *create_renderer(SDL_Window *w);
    void renderer_create_buffer_viewport(const int32_t id);
    void renderer_fill_bg(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void renderer_clear(void);
    void renderer_present(void);
    int renderer_draw_char(const int x, const int y, const int w, const int h, SDL_Texture *tex);
    int renderer_draw_cursor(const int row, const int col);
    int renderer_draw_file(class Buffer *buf);
    void renderer_draw_row(const std::string *str, const int start, const int y);
    const Buffer_Viewport* renderer_get_viewport(const int id);
    const int renderer_set_viewport_row_start(const int id, const int row);
    const int renderer_set_viewport_col_start(const int id, int col);
    void renderer_update_viewports(const std::vector<int32_t> *open);
    Chars* renderer_get_chars(void) { return &chars; }
    int renderer_draw_status(const int32_t id, const std::string *cmd);

  private:
    SDL_Renderer *rend;
    //Const pointers to the window dimension values.
    //Useful in that it points to the memory address of the values so if the values do change outside of this class, it will reflect.
    const int *win_width;
    const int *win_height;
    class Chars chars;
    
    int STATE;
    std::unordered_map<int32_t, Buffer_Viewport> vps;
  };

#endif
