#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "editor.h"

// TODO: TEXT WRAPPER FUCKED UP, is works really strange, check the source code for that shit


void render_text(char buffer[], TTF_Font *font, SDL_Color color, SDL_Window *window, SDL_Renderer *renderer, float scaling_factor) {
    // Wrap length
    uint32_t windowWidth = (uint32_t) SDL_GetWindowSurface(window)->w;

    // Create a surface to render TT fonts
    SDL_Surface *text_surface = TTF_RenderText_Blended_Wrapped(font, buffer, color, windowWidth);

    if (text_surface == NULL) error("TTF_RenderText_Blended_Wrapped");

    // Creating texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    if (texture == NULL) error("SDL_CreateTextureFromSurface");

    // Calculate scaled dimensions
    int scaled_width = (int)(text_surface->w * scaling_factor);
    int scaled_height = (int)(text_surface->h * scaling_factor);

    // Destination rectangle
    SDL_Rect destinationRect = {
        .x = 10,
        .y = 10,
        .w = scaled_width,
        .h = scaled_height
    };

    // Render the rectangle with the text
    if (SDL_RenderCopy(renderer, texture, NULL, &destinationRect) < 0) error("SDL_RenderCopy");

    SDL_FreeSurface(text_surface); // Free the surface
    SDL_DestroyTexture(texture); // Free the texture
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
    TTF_Font *font = TTF_OpenFont("static/OpenSans-Regular.ttf", TARGET_FONT_SIZE);

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
    char buffer[BUFFER_SIZE];

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
        render_text(buffer, font, color, window, renderer, 1);

        SDL_RenderPresent(renderer); // Show renderer on the window
    }


    // Free all objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit services
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();


    return 0;
}