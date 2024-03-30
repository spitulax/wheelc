#include "wheel.h"
#include "color.h"
#include "raylib.h"
#include <math.h>

#define WHEEL_RADIUS (GetScreenHeight()/3.0f)
#define WHEEL_SPEED 5000
#define LINES 10

Vector2 pos_on_circum(float degree, Vector2 center, float radius) {
  degree -= 90;
  return (Vector2) {
    center.x + radius * cosf(degree*DEG2RAD),
    center.y + radius * sinf(degree*DEG2RAD)
  };
}

void wheel_draw() {
  Vector2 center = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
  DrawCircleV(center, WHEEL_RADIUS, COLOR_WHEEL);
  for (int i = 0; i < LINES; i++) {
    float gap = 360.0f/LINES*i;
    DrawLineEx(center, pos_on_circum(GetTime()*WHEEL_SPEED*GetFrameTime() + gap, center, WHEEL_RADIUS), 5, ColorBrightness(COLOR_WHEEL, -0.5));
  }
}
