#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// === Class: Ball ===
// Manages physics and rendering of the ball object
class Ball {
public:
    float x, y;
    float dx, dy; // Velocity
    int radius;

    Ball(float startX, float startY, int r) 
        : x(startX), y(startY), radius(r) {
        // Set initial speed
        dx = 5.0f; 
        dy = 4.0f;
    }

    // Update position based on velocity and handle wall collisions
    void update() {
        x += dx;
        y += dy;

        // Collision with Left or Right walls
        if (x - radius < 0) {
            x = radius; // Correct position
            dx = -dx;   // Reverse horizontal direction
        } else if (x + radius > SCREEN_WIDTH) {
            x = SCREEN_WIDTH - radius;
            dx = -dx;
        }

        // Collision with Top or Bottom walls
        if (y - radius < 0) {
            y = radius;
            dy = -dy;   // Reverse vertical direction
        } else if (y + radius > SCREEN_HEIGHT) {
            y = SCREEN_HEIGHT - radius;
            dy = -dy;
        }
    }

    // Render the ball (Drawing a filled circle manually since SDL2 doesn't have a direct function)
    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255); // Red color

        // Midpoint circle algorithm for filling
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // Horizontal distance from center
                int dy = radius - h; // Vertical distance from center
                if ((dx*dx + dy*dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
};

// === Class: Game ===
// Manages the SDL window, renderer, and game loop
class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    Ball* ball;

public:
    Game() : window(nullptr), renderer(nullptr), isRunning(false), ball(nullptr) {}

    ~Game() {
        clean();
    }

    bool init(const char* title) {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create Window
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create Renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Initialize Game Objects
        ball = new Ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20);
        isRunning = true;
        
        std::cout << "Game Initialized successfully." << std::endl;
        return true;
    }

    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }
    }

    void update() {
        if (ball) ball->update();
    }

    void render() {
        // Clear screen to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw game objects
        if (ball) ball->render(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    void clean() {
        if (ball) delete ball;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::cout << "Game Cleaned up." << std::endl;
    }

    bool running() const { return isRunning; }
};

// === Main Function ===
int main(int argc, char* argv[]) {
    // Limit frame rate
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Game* game = new Game();

    if (game->init("SDL2 Bouncing Ball Game")) {
        while (game->running()) {
            frameStart = SDL_GetTicks();

            game->handleEvents();
            game->update();
            game->render();

            // Frame capping logic
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }

    delete game;
    return 0;
}
