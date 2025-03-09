#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"

FontRenderer::FontRenderer(SDL_Renderer *r, const Vec2i *dimensions)
    : rend(NULL), dim(dimensions) {
  fprintf(stdout, "Font renderer instance created\n");
}

void FontRenderer::frender_set_renderer(SDL_Renderer *r) { rend = r; }

void FontRenderer::render_buffer(std::vector<std::string> strbuf, Chars *chs,
                                 const Vec2i *chardims) {
  const int char_height = chardims->y;
  const int padding = 2;
  int rowy = 0;

  std::vector<std::string>::iterator it;
  for (it = strbuf.begin(); it != strbuf.end(); ++it) {
    std::string line = *it;
    int colx = 0;

    for (size_t i = 0; i < line.size(); i++) {
      if (line[i] == '\n') {
        continue;
      }
      Char_Tables *ct = chs->char_lookup(line[i]);
      SDL_Rect char_rect = {colx, rowy, ct->def.width, ct->def.height};
      SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);
      colx += (ct->def.width + padding);
    }

    rowy += (char_height + padding);
  }
}

std::vector<std::string> FontRenderer::split_by_nl(const Buf *buf) {
  std::vector<std::string> str_lines;
  unsigned char nl = '\n';
  std::string line = "";
  for (size_t i = 0; i < buf->size; i++) {
    line += buf->buf[i];

    if (buf->buf[i] == nl) {
      str_lines.push_back(line);
      str_lines.push_back("\n");
      line.clear();
    }
  }

  return str_lines;
}

FontRenderer::~FontRenderer(void) {}
