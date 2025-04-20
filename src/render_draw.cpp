#include "../inc/editor.hpp"
#include "../inc/globaldef.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/vecdef.hpp"


 //Returns the height in which the cursor reaches.
 int Renderer::render_curs(const Buf *buf, const Vec2i *window_dims)
 {
     const int pad = 2;
     int rowy = 0, colx = 0;
 
     for (size_t i = 0; i <= buf->bchar.size && (int)i <= buf->bcurs.pos; i++) {
         const int x = colx * buf->bcurs.w + pad;
         const int y = rowy * buf->bcurs.h + pad;
         
         //If going out of bounds of viewport area, just return and dont render
         //the cursor. This wont be implemented like this once viewport moving
         //is implemented and it is just in place to make sure things are
         //responding in the way of which I am expecting.
         if(y >= window_dims->y - buf->bcurs.h){
             return (rowy * buf->bcurs.h) + (rowy * pad); 
         }
 
         if (i == (size_t)buf->bcurs.pos) {
             SDL_Rect curs_rect = {x, y, buf->bcurs.w, buf->bcurs.h};
             SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
             SDL_RenderFillRect(rend, &curs_rect);
             return (rowy *  buf->bcurs.h) + (pad * rowy);
         }
 
         const int is_nl = buf->bchar.buf[i] == NEWLINE;
         switch (is_nl) {
         default:
             break;
         case 0:
         {
             colx += 1;
         } break;
         case 1:
         {
             colx = 0;
             rowy += 1;
         } break;
         }
     }
     return 0;
 }
 // Returns the total height of the buffer when rendered. TODO: pass window size
 // and height and cull the loop to avoid rendering past visible area
 int Renderer::render_buffer(const Buf *buf, Chars *ch, const Vec2i *window_dims)
 {
     const int padding = 2;
     const int m_ch_width = ch->get_char_dims()->x, m_ch_height = ch->get_char_dims()->y;
     int rowy = 0, colx = 0;
 
 
     for (size_t i = 0; i < buf->bchar.size && buf->bchar.buf[i] != NULLCHAR; i++) {
         const int x = colx * m_ch_width + padding;
         const int y = rowy * m_ch_height + padding;
 
         if(y >= window_dims->y - m_ch_height){
             return (rowy * m_ch_height) + (rowy * padding); 
         }
 
         const int is_nl = buf->bchar.buf[i] == NEWLINE;
         switch (is_nl) {
         default:
             break;
         case 0:
         {
             Char_Tables *ct = ch->char_lookup(buf->bchar.buf[i]);
             SDL_Rect char_rect = {x, y, ct->def.width, ct->def.height};
             SDL_RenderCopy(rend, ct->def.t, NULL, &char_rect);
             colx += 1;
         } break;
         case 1:
         {
             colx = 0;
             rowy += 1;
         } break;
         }
     }
     return (rowy * m_ch_height) + (rowy * padding); 
 }
 
