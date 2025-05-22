#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"
#include "../../include/SDL2/sdl2_renderer.hpp"

#include <iostream>

const SDL_Color punct = {237, 135, 150, 255};
const SDL_Color ops = {139, 213, 202, 255};
const SDL_Color letter = {202, 211, 245, 255};
const SDL_Color digit = {245, 169, 127, 255};
const SDL_Color type = {198, 160, 246, 255};
const SDL_Color keyword = {138, 173, 244, 255};

void ColourRef::set_map(void){
    map.push_back(std::make_pair(PUNCTUATION, punctuation));
    map.push_back(std::make_pair(OPERATORS, operators));
    map.push_back(std::make_pair(TYPE_DEFINITION, type));
    map.push_back(std::make_pair(GENERIC_TEXT, generic_letters));
    map.push_back(std::make_pair(KEYWORD, key_words));
    map.push_back(std::make_pair(DIGITS, digits));
}

VectorFont::VectorFont(SDL_Renderer *rend)
    : error(SDL2_NIL), font(nullptr), row_block(0), col_block(0), ch(nullptr), colours(punct, ops, letter, digit, type, keyword)
{
    vec_set_err(rend != nullptr ? SDL2_NIL : SDL2_ERR);
    colours.set_map();
    vec_set_err(vec_alloc_texture_array());
    vec_set_err(vec_open_font("LiberationMono-Regular.ttf", 16));
    vec_set_char();
    vec_set_err(vec_create_textures(rend));
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

        for(size_t j = 0; j < colours.map.size(); j++){
            vec_create_char_texture(colours.map[j].first, rend, ch[i], vec_create_char_surface(str, &colours.map[j].second));
        }

        if (ch[i].bad != SDL2_NIL) {
            return ch[i].bad;
        }
    }
    return SDL2_NIL;
}

SDL_Surface *VectorFont::vec_create_char_surface(const char *str, const SDL_Color* col)
{
    SDL_Surface *surf = TTF_RenderText_Blended(font, str, *col);
    if (!surf) {
        std::cerr << "Failed to create surface -> " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return surf;
}

void VectorFont::vec_create_char_texture(const uint8_t key, SDL_Renderer *rend, CSprite &sprite, SDL_Surface *surface)
{
    if (!surface) {
        std::cerr << "No workable surface" << std::endl;
        return;
    }

    SDL_Texture *t = SDL_CreateTextureFromSurface(rend, surface);
    sprite.tmap.insert({key, t});
    if (!t) {
        std::cerr << "Failed to create texture -> " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    sprite.tmap.insert({key, t});

    sprite.w = surface->w;
    sprite.h = surface->h;

    if (surface->w > col_block) {
        col_block = surface->w;
    }

    if (surface->h > row_block) {
        row_block = surface->h;
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