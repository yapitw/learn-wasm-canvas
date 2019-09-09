#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>
// Number of circles
#define NUM_CIRCLES 20

// Circle Struct

struct Circle
{
  int x;  // x coordinate
  int y;  // y coordinate
  int r;  // circle radius
  int cr; // color red
  int cg; // color green
  int cb; // color blue
};

// Circles variable
struct Circle circles[NUM_CIRCLES];

// Random number generator
int getRand(max)
{
  return (rand() % max);
}

// Init circle data and start render - JS
int main()
{
  // Seed random number generator
  srand(time(NULL));

  // Create circles
  for (int i = 0; i < NUM_CIRCLES; i++)
  {
    // Radius
    int radius = getRand(50);
    int x = getRand(1000) + radius;
    int y = getRand(1000) + radius;

    // Fill circle struct
    circles[i].x = x;
    circles[i].y = y;
    circles[i].r = radius;
    circles[i].cr = getRand(255);
    circles[i].cg = getRand(255);
    circles[i].cb = getRand(255);
  }

  printf("Init circles\n");
  // emscripten_run_script("render()");
  EM_ASM({render($0, $1)}, NUM_CIRCLES * 6, 6);

  return 0;
}

// Return circles to JS
struct Circle *getCircles()
{
  return circles;
}
