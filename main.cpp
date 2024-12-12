#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void SDL_RenderDrawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
    int x = radius, y = 0;
    int err = 0;

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        y++;
        err += 2 * y + 1;
        if (2 * err >= 2 * x + 1)
        {
            x--;
            err -= 2 * x + 1;
        }
    }
}

void SDL_AppIterate(SDL_Renderer *renderer, float &scaleFactor, int &direction, int xPos, int yPos)
{
    scaleFactor += direction * 0.01f;

    if (scaleFactor >= 1.5f || scaleFactor <= 0.5f)
    {
        direction = -direction;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255);

    int centerX = xPos, centerY = yPos;
    int radius = static_cast<int>(100 * scaleFactor);

    SDL_RenderDrawCircle(renderer, centerX, centerY, radius);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an SDL window
    SDL_Window *window = SDL_CreateWindow("Box Transformation Example",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    float scaleFactor = 1.0f;
    int direction = 1;

    bool running = true;

    SDL_Event event;

    int xPos = WINDOW_WIDTH / 2, yPos = WINDOW_HEIGHT / 2;

    while (running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    xPos = event.button.x;
                    yPos = event.button.y;
                }
            }
        }

        SDL_AppIterate(renderer, scaleFactor, direction, xPos, yPos);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}