#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "editor.h"


void render_cursor(SDL_Renderer *renderer, TTF_Font *font, int total_line_width, int y) {
    int cursor_width, cursor_height;
    TTF_SizeUTF8(font, " ", &cursor_width, &cursor_height); 
    SDL_Rect cursor_rect = { .x = total_line_width + 5, .y = y, .w = cursor_width, .h = cursor_height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderFillRect(renderer, &cursor_rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
}


void render_line(SDL_Renderer *renderer, TTF_Font *font, char *line, SDL_Color color, float scaling_factor, int *y) {
    SDL_Surface *line_surface = TTF_RenderText_Blended(font, line, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, line_surface);

    int scaled_width = (int) (line_surface->w * scaling_factor);
    int scaled_height = (int) (line_surface->h * scaling_factor);

    SDL_Rect destinationRect = {
        .x = 5,
        .y = *y,
        .w = scaled_width,
        .h = scaled_height
    };

    if (SDL_RenderCopy(renderer, texture, NULL, &destinationRect) < 0) error("SDL_RenderCopy");

    SDL_FreeSurface(line_surface);
    SDL_DestroyTexture(texture);

    *y += scaled_height; 
}


void render_text(char **buffer, TTF_Font *font, SDL_Color color, SDL_Window *window, SDL_Renderer *renderer, float scaling_factor) {
    size_t buffer_length = strlen(*buffer);
    int total_line_width = 0;

    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    char *current_line = malloc((buffer_length + 1) * sizeof(char));
    if (current_line == NULL) {
        error("current_line malloc failed");
        return;
    }
    current_line[0] = '\0';

    int y = 5;

    for (size_t i = 0; i < buffer_length; i++) {
        char temp[2];
        temp[0] = (*buffer)[i];
        temp[1] = '\0';

        int next_char_width, next_char_height;
        TTF_SizeUTF8(font, temp, &next_char_width, &next_char_height);

        if (total_line_width + next_char_width > window_width) {
            render_line(renderer, font, current_line, color, scaling_factor, &y);
            current_line[0] = '\0';
            total_line_width = 0;
        }

        strcat(current_line, temp);
        total_line_width += next_char_width;
    }

    render_cursor(renderer, font, total_line_width, y);

    if (current_line[0] != '\0') {
        render_line(renderer, font, current_line, color, scaling_factor, &y);
    }

    free(current_line);
}


void error(const char *context) {
    fprintf(stderr, "%s: %s\n", context, SDL_GetError());
    SDL_Quit();
    exit(1);
}


int main(void) {
    // Init services
    if (SDL_Init(SDL_INIT_VIDEO) < 0) error("SDL_Init");

    if (TTF_Init() < 0) error("TTF_Init");

    // Loading the font
    TTF_Font *font = TTF_OpenFont("static/RobotoMono-Regular.ttf", TARGET_FONT_SIZE);

    if (font == NULL) error("TTF_OpenFont");


    // Creating a window
    SDL_Window *window = SDL_CreateWindow("Texdit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (window == NULL) error("SDL_CreateWindow");


    // Create an accelerated renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) error("SDL_CreateRenderer");

    // Color for text rendering
    SDL_Color color = {255, 255, 255, 255};

    bool quit = false;

    // Buffer of the text input
    char *buffer = malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        error("Buffer allocation failed");
        return 1;
    }

    buffer[0] = '\0';

    // Main event loop
    while (!quit) {

        // Get current window size
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: // Handling quit
                    quit = true;
                    break;
                case SDL_TEXTINPUT: // Handling text input
                    if (strlen(buffer) + strlen(e.text.text) < BUFFER_SIZE - 1) {
                        strcat(buffer, e.text.text);
                    } else {
                        fprintf(stderr, "Buffer overflow\n");
                    }
                    break;
            }
            break;
        }
        

        // Clear the current rendering target with the drawing color
        if (SDL_RenderClear(renderer) < 0) error("SDL_RenderClear"); 

        // Set the color of the rendering
        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) < 0) error("SDL_SetRenderDrawColor"); 

        // Buffer rendering 
        render_text(&buffer, font, color, window, renderer, 1);

        SDL_RenderPresent(renderer); // Show renderer on the window
    }

    free(buffer);

    // Free all objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit services
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();


    return 0;
}