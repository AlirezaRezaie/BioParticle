/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void drawFilledRectangle(SDL_Surface *surface, SDL_Rect rect, int width,
                         int height, Uint32 color) {
  for (int row = rect.y; row < rect.y + height; row++) {
    for (int col = rect.x; col < rect.x + width; col++) {
      Uint32 *pixel = (Uint32 *)surface->pixels + row * surface->w + col;
      *pixel = color;
    }
  }
}

typedef struct particle {
  float x;
  float y;
  float vx;
  float vy;
  Uint32 color;
} Particle;

struct PSystem {
  Particle *particles;
  int numOfParticles;
};

Particle *create_particles(int numParticles, Uint32 color) {
  Particle *particles = (Particle *)malloc(numParticles * sizeof(Particle));
  if (particles == NULL)
    return NULL;

  for (int i = 0; i < numParticles; i++) {
    particles[i].x =
        ((float)rand() / RAND_MAX) * 500 + 50; // Adjust range as needed
    particles[i].y = ((float)rand() / RAND_MAX) * 500 + 200;

    particles[i].vx = 0;
    particles[i].vy = 0;
    particles[i].color = color;
  }

  return particles;
}

void draw_particles(SDL_Surface *surface, Particle *particles, int num) {
  for (size_t i = 0; i < num; i++) {
    // clrear the screen
    SDL_Rect rect = {particles[i].x, particles[i].y, 2, 2};
    SDL_FillRect(surface, &rect, particles[i].color);
  }
}

void rule(struct PSystem particles1, struct PSystem particles2, float g) {
  // applys a gravitational force to each particle and changes their x,y based
  // on the force

  for (size_t i = 0; i < particles1.numOfParticles; i++) {
    float fx = 0;
    float fy = 0;

    // we alias the currenct particles to a for easy access
    Particle *a = &particles1.particles[i];

    for (size_t j = 0; j < particles2.numOfParticles; j++) {
      // calculate the force on the currenct particle from every other
      Particle b = particles2.particles[j];

      float dx = a->x - b.x;
      float dy = a->y - b.y;

      float d = sqrt(dx * dx + dy * dy);

      if (d > 0 && d < 80) {
        float F = (g * 1) / d;
        fx += (F * dx);
        fy += (F * dy);
      }
    }

    a->vx = (a->vx + fx) * 0.5;
    a->vy = (a->vy + fy) * 0.5;

    a->x += a->vx;
    a->y += a->vy;

    if (a->x < 0 || a->x > 500) {
      a->vx *= -1;
    }
    if (a->y < 0 || a->y > 500) {
      a->vy *= -1;
    }
  }
}

// Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;

int WinMain(int argc, char *args[]) {
  srand(0);
  // The window we'll be rendering to
  SDL_Window *window = NULL;

  // The surface contained by the window
  SDL_Surface *screenSurface = NULL;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  } else {
    // Create window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      // Get window surface
      screenSurface = SDL_GetWindowSurface(window);

      // create yellow particles
      int numOfYellows = 500;
      Uint32 colorYellow = SDL_MapRGB(screenSurface->format, 255, 255, 0);
      Particle *yellowParticles = create_particles(numOfYellows, colorYellow);
      struct PSystem yellow = {yellowParticles, numOfYellows};
      srand(time(0));

      // create red particles
      int numOfReds = 500;
      Uint32 colorRed = SDL_MapRGB(screenSurface->format, 255, 0, 0);
      Particle *redParticles = create_particles(numOfReds, colorRed);
      struct PSystem red = {redParticles, numOfReds};

      // create green particles
      int numOfGreens = 500;
      Uint32 colorGreen = SDL_MapRGB(screenSurface->format, 0, 255, 0);
      Particle *greenParticles = create_particles(numOfGreens, colorGreen);
      struct PSystem green = {greenParticles, numOfGreens};

      // Hack to get window to stay up
      SDL_Event e;
      int quit = 0;
      int x = 100;
      SDL_Rect rect = {20, 30, 10, 10};
      while (quit == 0) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            quit = 1;
        }

        // clrear the screen
        SDL_FillRect(screenSurface, NULL,
                     SDL_MapRGB(screenSurface->format, 0, 0, 0));

        rule(green, green, -0.32);
        rule(green, red, -0.17);
        rule(green, yellow, 0.34);
        rule(red, red, -0.10);
        rule(red, green, -0.34);
        rule(yellow, yellow, 0.15);
        rule(yellow, green, -0.20);

        draw_particles(screenSurface, yellowParticles, numOfYellows);
        draw_particles(screenSurface, greenParticles, numOfGreens);
        draw_particles(screenSurface, redParticles, numOfReds);

        // Update the surface
        SDL_UpdateWindowSurface(window);
      }
    }
  }

  // Destroy window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return 0;
}