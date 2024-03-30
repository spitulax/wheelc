#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FONT_SIZE 24
// we can't afford too much
#define MAX_WHEELS 100

// Yes, this is catppuccin mocha
#define COLOR_BACKGROUND GetColor(0x101020ff)
#define COLOR_TEXT GetColor(0xcdd6f4ff)
#define COLOR_BLUE GetColor(0x89b4faff)
#define COLOR_GREEN GetColor(0xa6e3a1ff)
#define COLOR_RED GetColor(0xf38ba8ff)

#define WHEEL_ACCEL 0.1
#define WHEEL_DECEL 0.01

typedef struct Wheel {
  Vector2 center;
  float radius;
  float max_speed;
  unsigned int slices;
  Color base_color;
  float rot_deg;
  float speed;
} Wheel;

Wheel wheel_new(Vector2 center, float radius, float max_speed, int slices, Color base_color) {
  return (Wheel) {
    .center = center,
    .radius = radius,
    .max_speed = max_speed,
    .slices = slices,
    .base_color = base_color,
    .rot_deg = 0,
    .speed = 0,
  };
}

void wheels_add(Wheel *wheels, unsigned int *wheel_count, Wheel wheel) {
  if (*wheel_count + 1 < MAX_WHEELS)
    wheels[(*wheel_count)++] = wheel;
  else
    fprintf(stderr, "[ERROR] Trying to add more wheel past MAX_WHEELS. Cancelling\n");
}

void wheels_pop(Wheel *wheels, unsigned int *wheel_count) {
  if (*wheel_count > 1)
    (*wheel_count)--;
  else
    fprintf(stderr, "[ERROR] Trying to pop the only remaining wheel. Cancelling\n");
}

void wheels_draw(Wheel *wheels, int wheel_count) {
  for (int i = 0; i < wheel_count; i++) {
    Wheel *wheel = &wheels[i];
    DrawCircleV(wheel->center, wheel->radius, wheel->base_color);
    if (wheel->speed < wheel->max_speed && wheel->speed > 0)
      wheel->speed = Lerp(wheel->speed, wheel->max_speed, WHEEL_ACCEL);
    else
      wheel->speed = Lerp(wheel->speed, wheel->max_speed, WHEEL_DECEL);
    wheel->rot_deg += wheel->speed * GetFrameTime();
    for (int i = 0; i < wheel->slices; i++) {
      float gap = 360.0f/wheel->slices*i;
      float angle = wheel->rot_deg + gap;
      DrawCircleSector(wheel->center, wheel->radius, angle, angle + 360.0f/wheel->slices, 100, ColorBrightness(wheel->base_color, -0.7 + 0.1 * (i % 10)));
    }
  }
}

// Wheels can stack
// This makes sure only the topmost wheel gets polled
void wheels_poll(Wheel *wheels, unsigned int *wheel_count) {
  for (int i = *wheel_count - 1; i >= 0; i--) {
    if (CheckCollisionPointCircle(GetMousePosition(), wheels[i].center, wheels[i].radius)) {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        wheels[i].radius += 10;
      if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        wheels[i].radius -= 10;

      if (GetMouseWheelMoveV().y > 0)
        wheels[i].max_speed += 10;
      else if (GetMouseWheelMoveV().y < 0)
        wheels[i].max_speed -= 10;
      else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
        wheels[i].max_speed = 0;

      break;
    } else if (i <= 0) {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        wheels_add(wheels, wheel_count, wheel_new(GetMousePosition(), 100, 100, 10, COLOR_GREEN)); // TODO: randomize colors
    }
  }
}

void wheels_hud(Wheel *wheels, int wheel_count) {
  for (int i = wheel_count - 1; i >= 0; i--) {
    if (CheckCollisionPointCircle(GetMousePosition(), wheels[i].center, wheels[i].radius)) {
      char buf[100] = {0}; // TODO: unbound. autosegfault when reaches more than allocated chars
      sprintf(buf, "ID: %d\nStatus: %s\nSpeed: %.2f\nRadius: %.2f\nSlices: %d",
              i,
              wheels[i].max_speed == 0 ? "Stopped" : "Running",
              wheels[i].speed,
              wheels[i].radius,
              wheels[i].slices); // TODO: add/remove slices on the fly
      DrawText(buf, 0, GetScreenHeight() - FONT_SIZE*4, FONT_SIZE, COLOR_TEXT);
      break;
    }
  }
}

int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Wheel C");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(60);

  Wheel *wheels = calloc(MAX_WHEELS, sizeof(Wheel));
  unsigned int wheel_count = 0;
  wheels_add(wheels, &wheel_count, wheel_new((Vector2) { WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f }, 200, 50, 100, COLOR_BLUE));

  while (!WindowShouldClose()) {
    /* Event */
    wheels_poll(wheels, &wheel_count);
    if (IsKeyReleased(KEY_P)) wheels_pop(wheels, &wheel_count);

    BeginDrawing();

    /* Game */
    ClearBackground(COLOR_BACKGROUND);
    wheels_draw(wheels, wheel_count);

    /* HUD */
    DrawFPS(0, 0);
    wheels_hud(wheels, wheel_count);

    EndDrawing();
  }

  free(wheels);

  CloseWindow();

  return 0;
}
