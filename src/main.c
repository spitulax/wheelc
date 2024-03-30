#include "raylib.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Yes, this is catppuccin mocha
#define COLOR_BACKGROUND GetColor(0X101020ff)
#define COLOR_TEXT GetColor(0xcdd6f4ff)
#define COLOR_BLUE GetColor(0x89b4faff)
#define COLOR_GREEN GetColor(0xa6e3a1ff)
#define COLOR_RED GetColor(0xf38ba8ff)

typedef struct Wheel {
  Vector2 center;
  float radius;
  float speed;
  unsigned int lines;
  Color base_color;
} Wheel;

Wheel wheel_new(Vector2 center, float radius, float speed, int lines, Color base_color) {
  return (Wheel) {
    .center = center,
    .radius = radius,
    .speed = speed,
    .lines = lines,
    .base_color = base_color,
  };
}

void wheel_draw(const Wheel *wheel) {
  DrawCircleV(wheel->center, wheel->radius, wheel->base_color);
  for (int i = 0; i < wheel->lines; i++) {
    float gap = 360.0f/wheel->lines*i;
    float angle = GetTime()*wheel->speed + gap;
    DrawCircleSector(wheel->center, wheel->radius, angle, angle + 360.0f/wheel->lines, 100, ColorBrightness(wheel->base_color, -0.7 + 0.1 * (i % 10)));
  }
}

// Wheels can stack
// This makes sure only the topmost wheel gets polled
void wheels_poll(Wheel *wheels, int wheel_count) {
  for (int i = wheel_count - 1; i >= 0; i--) {
    if (CheckCollisionPointCircle(GetMousePosition(), wheels[i].center, wheels[i].radius)) {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        wheels[i].radius += 10;
      if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        wheels[i].radius -= 10;

      if (GetMouseWheelMoveV().y > 0)
        wheels[i].speed += 10;
      else if (GetMouseWheelMoveV().y < 0)
        wheels[i].speed -= 10;
      else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
        wheels[i].speed = 0;

      break;
    }
  }
}

#define WHEEL_COUNT 3

int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Random ahh");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(60);

  Wheel wheels[WHEEL_COUNT] = {
    wheel_new((Vector2) { WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f }, 300, 50, 100, COLOR_RED),
    wheel_new((Vector2) { 100, 400 }, 100, 100, 30, COLOR_BLUE),
    wheel_new((Vector2) { 600, 100 }, 150, 500, 50, COLOR_GREEN),
  };

  while (!WindowShouldClose()) {
    BeginDrawing();

    /* Event */
    wheels_poll(wheels, WHEEL_COUNT);

    /* Game */
    ClearBackground(COLOR_BACKGROUND);
    for (int i = 0; i < WHEEL_COUNT; i++)
      wheel_draw(&wheels[i]);

    /* HUD */
    DrawFPS(0, 0);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
