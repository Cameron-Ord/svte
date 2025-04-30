#include "../inc/font.hpp"
#include "../inc/globaldef.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

static int force_bounds(const int i)
{
    if (i >= ASCII_TABLE_SIZE) {
        return 0;
    } else if (i < 0) {
        return 0;
    }
    return 1;
}

static SDL_Surface *font_surface(TTF_Font *f, const char *buf, SDL_Color col)
{
    return TTF_RenderText_Solid(f, buf, col);
}

static SDL_Texture *font_texture(SDL_Renderer *rend, SDL_Surface *s)
{
    return SDL_CreateTextureFromSurface(rend, s);
}

Chars::Chars(void) { 
    max_height = 0, max_width = 0;
    set_table(); 
}

const void* Chars::ch_set_font(TTF_Font *fp){
    f.base = fp;
    return f.base;
}

TTF_Font *Chars::ch_open_font(const char *fn, const int size)
{
    TTF_Font *f = TTF_OpenFont(fn, size);
    if (!f) {
        fprintf(stderr, "Failed to open font! -> %s\n", TTF_GetError());
    }
    return f;
}



int Chars::ch_create_textures(SDL_Renderer *rend, const Fonts *f)
{
    SDL_Color col = {255, 255, 255, 255};
    for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
        Char_Table *ct = &chtbls[i];

        SDL_Surface *s = font_surface(f->base, ct->str, col);
        if (!s) {
            std::cerr << "Failed to create surface!"
                      << " " << SDL_GetError()
                      << std::endl;
            return -1;
        }
        if (!(ct->base.t = font_texture(rend, s))) {
            std::cerr << "Failed to create texture!"
                      << " " << SDL_GetError()
                      << std::endl;
            return -1;
        }

        ct->base.width = s->w, ct->base.height = s->h;

        if (s->w > max_width) {
            max_width = s->w;
        }

        if (s->h > max_height) {
            max_height = s->h;
        }

        SDL_FreeSurface(s);
    }
    return 1;
}


void Chars::set_table(void)
{
    for (int i = 32; i < ASCII_TABLE_SIZE; i++) {
        chtbls[i].c = i;
        const char str[] = {(char)i, NULLCHAR};
        memcpy(chtbls[i].str, str, 2);
    }
}

Char_Table *Chars::ch_lookup(unsigned char c)
{
    if (!force_bounds((int)c)) {
        return &chtbls['?'];
    }

    return &chtbls[c];
}
