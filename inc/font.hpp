#ifndef FONT_HPP
#define FONT_HPP

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;
struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;
struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

#define ASCII_TABLE_SIZE 128
#define STRBUF_SIZE 2

typedef enum { DEF = 0, TITLE = 1 } Table_Selectors;

struct Ascii_Char {
  SDL_Texture *t;
  int width, height;
};
typedef struct Ascii_Char Ascii_Char;

struct Fonts {
  TTF_Font *title;
  TTF_Font *def;
};
typedef struct Fonts Fonts;

struct Char_Tables {
  unsigned int c;
  char str[STRBUF_SIZE];
  Ascii_Char title;
  Ascii_Char def;
};
typedef struct Char_Tables Char_Tables;

class Font {
public:
  Font(void);
  TTF_Font *open_font(const char *fn, const int size);
  Fonts *get_font(void);
  Char_Tables *get_tbls(void);
  int table_create_textures(SDL_Renderer *rend);
  Char_Tables *char_lookup(unsigned char c, const int selector);

private:
  void set_table(void);
  Fonts fonts;
  Char_Tables chtbls[ASCII_TABLE_SIZE];
};

Font *get_font_inst(void);

#endif
