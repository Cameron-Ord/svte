#ifndef FONT_HPP
#define FONT_HPP

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

#define ASCII_TABLE_SIZE 128
#define STRBUF_SIZE 2

struct ascii_char {
  unsigned int c;
  char str[STRBUF_SIZE];
};

class Font {
public:
  Font(void);
  const void *open_font(const char *fn);
  TTF_Font *get_font(void);

private:
  int size;
  void set_table(void);
  TTF_Font *f;
  ascii_char tbl[ASCII_TABLE_SIZE];
};

Font *get_font_inst(void);

#endif
