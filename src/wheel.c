#include "wheel.h"
#include "raylib.h"
#include <math.h>

Vector2 pos_on_circum(float degree, Vector2 center, float radius) {
  degree -= 90;
  return (Vector2) {
    center.x + radius * cosf(degree*DEG2RAD),
    center.y + radius * sinf(degree*DEG2RAD)
  };
}

void wheel_draw(Vector2 center, float radius, float speed, int lines, Color color) {
  DrawCircleV(center, radius, color);
  for (int i = 0; i < lines; i++) {
    float gap = 360.0f/lines*i;
    DrawLineEx(center, pos_on_circum(GetTime()*speed + gap, center, radius), 2, ColorBrightness(color, -0.3));
  }
}
