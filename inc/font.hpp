#ifndef FONT_HPP
#define FONT_HPP

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;
struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

// Older versions use TTF_Font struct name, newer use _TTF_Font struct name
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

#endif
