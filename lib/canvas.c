#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>
#include <stdint.h>
// Number of circles
#define NUM_CIRCLES 20

// Circle Struct

struct Circle
{
  int16_t x;  // x coordinate
  int16_t y;  // y coordinate
  int16_t r;  // circle radius
  int16_t cr; // color red
  int16_t cg; // color green
  int16_t cb; // color blue
};

struct CircleAnimationData
{
  int16_t x;  // x coordinate
  int16_t y;  // y coordinate
  int16_t r;  // circle radius
  int16_t xv; // x - axis velocity
  int16_t yv; // y - axis velocity
  int16_t xd; // x - axis direction (1 = forward)
  int16_t yd; // y - axis direction (1 = forward)
};

// Circles variable
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

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

    // Fill animation data struct - i
    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].r = radius;
    animationData[i].xv = getRand(10);
    animationData[i].yv = getRand(10);
    animationData[i].xd = 1;
    animationData[i].yd = 1;

    // Fill circle struct - i
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
struct Circle *getCircles(int canvasWidth, int canvasHeight)
{
  // Update circle data
  for (int i = 0; i < NUM_CIRCLES; i++)
  {

    // Collision RIGHT - set x direction backwards 0
    if ((animationData[i].x + animationData[i].r) >= canvasWidth)
      animationData[i].xd = 0;

    // Collision LEFT - set x direction forwards 1
    if ((animationData[i].x - animationData[i].r) <= 0)
      animationData[i].xd = 1;

    // Collision BOTTOM - set y direction backwards 0
    if ((animationData[i].y + animationData[i].r) >= canvasHeight)
      animationData[i].yd = 0;

    // Collision TOP - set y direction forwards 1
    if ((animationData[i].y - animationData[i].r) <= 0)
      animationData[i].yd = 1;

    // Move circle in specified direction
    if (animationData[i].xd == 1)
    {
      animationData[i].x += animationData[i].xv;
    }
    else
    {
      animationData[i].x -= animationData[i].xv;
    }

    if (animationData[i].yd == 1)
    {
      animationData[i].y += animationData[i].yv;
    }
    else
    {
      animationData[i].y -= animationData[i].yv;
    }

    // Update matching circle
    circles[i].x = animationData[i].x;
    circles[i].y = animationData[i].y;
  }

  // Return updated circles
  return circles;
}
