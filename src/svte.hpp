#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct TTF_Font TTF_Font;
typedef struct SDL_Texture SDL_Texture;

class window_container {
  public:
    window_container(std::string title, int bwidth, int bheight);
    bool init_window(int flags);
    void update_window_dim(void);
  private:
    std::string title;
    SDL_Window *w;
    int width, height;
};


//  Gonna essentially decode input from SDL text input into 32 bit unsigned ints and use that as the lookup.
//  Obviously dont want memory usage to explode so i'll just default to ASCII code to begin with, and then if the user inputs 
//  a char that isnt recognized after decoding, then add it to the map for lookup. Cant really think of a better way right now, 
//  but I think this is good.
//  Atleast SDL handles the hard part for me and I can just decode the information from textinputs utf8 str.
class font_map {
  public:
    font_map(void) = default;
    void map_insert_defaults(TTF_Font *const font, SDL_Renderer *r);
  private:
    std::unordered_map<uint32_t, SDL_Texture*> glyphs;
};

class font_container {
  public:
    font_container(std::string fontpath, int fontsize);
    bool font_open(void);
  private:
    TTF_Font *font;
    std::string name;
    int size;
    font_map map;
};

class renderer_container {
  public:
    renderer_container(std::string fontpath, int fontsize);
    bool init_renderer(int flags, SDL_Window *w);
  private:
    SDL_Renderer *r;
    font_container fc;
};

