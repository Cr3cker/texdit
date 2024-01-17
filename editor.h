#ifndef EDITOR_H
#define EDITOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800
#define TARGET_FONT_SIZE 30
#define BUFFER_SIZE 1024

void render_line(SDL_Renderer *renderer, TTF_Font *font, char *line, SDL_Color color, float scaling_factor, int *y);
void render_text(char **buffer, TTF_Font *font, SDL_Color color, SDL_Window *window, SDL_Renderer *renderer, float scaling_factor, int cursor_position);
void error(const char *context);
void render_cursor(SDL_Renderer *renderer, TTF_Font *font, int total_line_width, int y);

#endif // EDITOR_H