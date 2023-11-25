#include "math.h"
#include "time.h"
#include <SDL2/SDL.h>

#define WIDTH 1000
#define HEIGHT 1000

void draw_circle(SDL_Renderer *renderer, float x, float y, float radius,
                 SDL_Color color);

typedef struct particle {
  float x;
  float y;
  float vx;
  float vy;
  SDL_Color color;
} Particle;

struct PSystem {
  Particle *particles;
  int numOfParticles;
};

Particle *create_particles(int numParticles, SDL_Color color) {
  Particle *particles = (Particle *)malloc(numParticles * sizeof(Particle));
  if (particles == NULL)
    return NULL;

  for (int i = 0; i < numParticles; i++) {
    particles[i].x =
        ((float)rand() / RAND_MAX) * 900 + 50; // Adjust range as needed
    particles[i].y = ((float)rand() / RAND_MAX) * 900 + 50;
    particles[i].vx = 0;
    particles[i].vy = 0;
    particles[i].color = color;
  }

  return particles;
}

void draw_particles(SDL_Renderer *renderer, Particle *particles, int num) {
  for (size_t i = 0; i < num; i++) {
    draw_circle(renderer, particles[i].x, particles[i].y, 2,
                particles[i].color);
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

int main() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  srand(time(0));
  // Create a window
  SDL_Window *window =
      SDL_CreateWindow("Simple Game", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

  // Create a renderer
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // create yellow particles
  int numOfYellows = 900;
  SDL_Color colorYellow = {255, 255, 0, 2};
  Particle *yellowParticles = create_particles(numOfYellows, colorYellow);
  struct PSystem yellow = {yellowParticles, numOfYellows};

  // create red particles
  int numOfReds = 900;
  SDL_Color colorRed = {255, 0, 0, 2};
  Particle *redParticles = create_particles(numOfReds, colorRed);
  struct PSystem red = {redParticles, numOfReds};

  // create green particles
  int numOfGreens = 900;
  SDL_Color colorGreen = {0, 255, 0, 2};
  Particle *greenParticles = create_particles(numOfGreens, colorGreen);
  struct PSystem green = {greenParticles, numOfGreens};

  // Game loop
  int quit = 0;
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    // this rules emerge cells
    rule(red, red, 0.1);
    rule(yellow, red, 0.15);
    rule(green, green, -0.7);
    rule(green, red, -0.2);
    rule(red, green, -0.1);


    // this rules emerges spaceships
/*     rule(green, green, -0.32);
    rule(green, red, -0.17);
    rule(green, yellow, 0.34);
    rule(red, red, -0.10);
    rule(red, green, -0.34);
    rule(yellow, yellow, 0.15);
    rule(yellow, green, -0.20); */

    draw_particles(renderer, yellowParticles, numOfYellows);
    draw_particles(renderer, greenParticles, numOfGreens);
    draw_particles(renderer, redParticles, numOfReds);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Delay to control frame rate
    // SDL_Delay(1);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
