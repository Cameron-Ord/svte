#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"
#include "../../include/SDL2/sdl2_renderer.hpp"

#include <iostream>

const SDL_Color punct      = {220, 110, 130, 255};  // Slightly deeper pink
const SDL_Color ops        = {120, 200, 190, 255};  // Teal softened
const SDL_Color letter     = {190, 200, 230, 255};  // Soft lavender
const SDL_Color digit      = {235, 140, 90, 255};   // Warm orange
const SDL_Color type       = {175, 140, 220, 255};  // Muted purple
const SDL_Color keyword    = {120, 150, 220, 255};  // Desaturated blue
const SDL_Color string     = {170, 200, 120, 255};  // Gentle green
const SDL_Color comment    = {100, 100, 120, 255}; 

void ColourRef::set_map(void){
    map.push_back(std::make_pair(PUNCTUATION, punct));
    map.push_back(std::make_pair(OPERATORS, ops));
    map.push_back(std::make_pair(TYPE_DEFINITION, type));
    map.push_back(std::make_pair(GENERIC_TEXT, letter));
    map.push_back(std::make_pair(KEYWORD, keyword));
    map.push_back(std::make_pair(DIGITS, digit));
    map.push_back(std::make_pair(STRING, string));
    map.push_back(std::make_pair(COMMENT, comment));
}

VectorFont::VectorFont(SDL_Renderer *rend)
    : error(SDL2_NIL), font(nullptr), row_block(0), col_block(0), ch(nullptr)
{
    vec_set_err(rend != nullptr ? SDL2_NIL : SDL2_ERR);
    colours.set_map();
    vec_set_err(vec_alloc_texture_array());
    vec_set_err(vec_open_font("LiberationMono-Regular.ttf", 12));
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