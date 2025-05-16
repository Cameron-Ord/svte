#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"
#include "../../include/SDL2/sdl2_renderer.hpp"

#include <iostream>

VectorFont::VectorFont(SDL_Renderer *rend)
    : error(SDL2_NIL), font(nullptr), row_block(0), col_block(0), ch(nullptr)
{
    vec_set_err(vec_alloc_texture_array());
    vec_set_err(vec_open_font("dogicapixel.ttf", 14));
    vec_set_char();
    vec_set_err(vec_create_textures(rend));
}

const int &VectorFont::vec_col_block(void) const
{
    return col_block;
}

const int &VectorFont::vec_row_block(void) const
{
    return row_block;
}

int VectorFont::vec_get_err(void)
{
    return error;
}

void VectorFont::vec_set_err(const int errval)
{
    error = errval;
}

void VectorFont::vec_set_font(TTF_Font *f)
{
    font = f;
}

TTF_Font *VectorFont::vec_get_font(void)
{
    return font;
}

int VectorFont::vec_alloc_texture_array(void)
{
    ch = new CSprite[ASCII_MAX];
    if (!ch) {
        return SDL2_ERR;
    }
    return SDL2_NIL;
}

int VectorFont::vec_open_font(const std::string path, const int ptsize)
{
    vec_set_font(TTF_OpenFont(path.c_str(), ptsize));
    if (!vec_get_font()) {
        std::cerr << "Could not open vector font -> " << SDL_GetError() << std::endl;
        return SDL2_ERR;
    }
    return SDL2_NIL;
}

void VectorFont::vec_set_char(void)
{
    for (unsigned int i = ASCII_MIN; i < ASCII_END; i++) {
        ch[i].w = 0;
        ch[i].h = 0;
        ch[i].texture = nullptr;
        ch[i].bad = SDL2_ERR;
    }
}

int VectorFont::vec_create_textures(SDL_Renderer *rend)
{
    if (!rend) {
        std::cerr << "Passed a NULL pointer using SDL_Renderer" << std::endl;
        return SDL2_ERR;
    }

    for (unsigned int i = ASCII_START; i < ASCII_END; i++) {
        char c = (char)i;
        const char str[2] = {c, '\0'};
        vec_create_char_texture(rend, ch[i], vec_create_char_surface(str));
        if (ch[i].bad != SDL2_NIL) {
            return ch[i].bad;
        }
    }
    return SDL2_NIL;
}

SDL_Surface *VectorFont::vec_create_char_surface(const char *str)
{
    SDL_Color col = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Solid(font, str, col);
    if (!surf) {
        std::cerr << "Failed to create surface -> " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return surf;
}

void VectorFont::vec_create_char_texture(SDL_Renderer *rend, CSprite &sprite, SDL_Surface *surface)
{
    if (!surface) {
        std::cerr << "No workable surface" << std::endl;
        return;
    }

    sprite.w = surface->w;
    sprite.h = surface->h;

    if (surface->w > col_block) {
        col_block = surface->w;
    }

    if (surface->h > row_block) {
        row_block = surface->h;
    }

    sprite.texture = SDL_CreateTextureFromSurface(rend, surface);
    if (!sprite.texture) {
        std::cerr << "Failed to create texture -> " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_FreeSurface(surface);
    sprite.bad = SDL2_NIL;
}

const CSprite &VectorFont::vec_index_texture(const unsigned char c) const
{
    if (c >= ASCII_START && c < ASCII_MAX) {
        return ch[c];
    } else {
        return ch['?'];
    }
}