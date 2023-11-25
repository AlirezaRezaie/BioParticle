
#include <SDL2/SDL.h>


void draw_circle(SDL_Renderer *renderer, float x, float y, float radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (float w = 0; w < radius * 2; w++)
    {
        for (float h = 0; h < radius * 2; h++)
        {
            float dx = radius - w; // horizontal offset
            float dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPointF(renderer, x + dx, y + dy);
            }
        }
    }
}