#ifndef EDITOR_H
#define EDITOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800
#define TARGET_FONT_SIZE 30
#define BUFFER_SIZE 1024

void render_text(char buffer[], TTF_Font *font, SDL_Color color, SDL_Window *window, SDL_Renderer *renderer, float scaling_factor);
void error(const char *context);

#endif // EDITOR_H