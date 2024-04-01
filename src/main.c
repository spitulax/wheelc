#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FONT_SIZE 24
#define MAX_WHEELS 100

// Yes, this is catppuccin mocha
#define COLOR_BASE 0x101020ff
#define COLOR_TEXT 0xcdd6f4ff
#define COLOR_COUNT 14
unsigned int colors[COLOR_COUNT] = {
  0xb4befeff,
  0x89b4faff,
  0x74c7ecff,
  0x89dcebff,
  0x94e2d5ff,
  0xa6e3a1ff,
  0xf9e2afff,
  0xfab387ff,
  0xeba0acff,
  0xf38ba8ff,
  0xcba6f7ff,
  0xf5c2e7ff,
  0xf2cdcdff,
  0xf5e0dcff,
};

#define KEY_DELAY 0.1
double key_timeout = 0;

#define WHEEL_ACCEL 0.1
#define WHEEL_DECEL 0.015
#define WHEEL_SPIN_FORCE 2000

typedef struct Wheel {
  Vector2 center;
  float radius;
  float max_speed;
  unsigned int slices;
  Color base_color;
  float rot_deg;
  float velocity;
} Wheel;

Wheel wheel_new(Vector2 center, float radius, float max_speed, unsigned int slices, Color base_color) {
  return (Wheel) {
    center,
    radius,
    max_speed,
    slices,
    base_color,
    .rot_deg = 0,
    .velocity = 0,
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
    wheels[(*wheel_count)--] = (Wheel){};
  else
    fprintf(stderr, "[ERROR] Trying to pop the only remaining wheel. Cancelling\n");
}

void wheels_draw(Wheel *wheels, int wheel_count) {
  for (int i = 0; i < wheel_count; i++) {
    Wheel *wheel = &wheels[i];
    DrawCircleV(wheel->center, wheel->radius, wheel->base_color);
    if (wheel->velocity < wheel->max_speed && wheel->velocity > 0)
      wheel->velocity = Lerp(wheel->velocity, wheel->max_speed, WHEEL_ACCEL);
    else
      wheel->velocity = Lerp(wheel->velocity, wheel->max_speed, WHEEL_DECEL);
    wheel->rot_deg += wheel->velocity * GetFrameTime();

    for (unsigned int i = 0; i < wheel->slices && wheel->slices > 1; i++) {
      const Color color = (i % 10 < 5)
        ? ColorBrightness(wheel->base_color, 0 + 0.1 * (i % 5))
        : ColorBrightness(wheel->base_color, -0.5 + 0.1 * (i % 5));
      const float slice_angle = 360.0f/wheel->slices;
      const float angle = wheel->rot_deg + slice_angle * i;
      DrawCircleSector(wheel->center, wheel->radius, angle, angle + slice_angle, 100, color);
    }
  }
}

// Wheels can stack
// This makes sure only the topmost wheel gets polled
void wheels_poll(Wheel *wheels, unsigned int *wheel_count) {
  for (int i = *wheel_count - 1; i >= 0; i--) {
    Wheel *wheel = &wheels[i];
    if (CheckCollisionPointCircle(GetMousePosition(), wheel->center, wheel->radius)) {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        wheel->radius += 10;
      if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        wheel->radius -= 10;

      if (GetMouseWheelMoveV().y > 0)
        wheel->max_speed += 10;
      else if (GetMouseWheelMoveV().y < 0)
        wheel->max_speed -= 10;
      else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
        wheel->max_speed = 0;

      if (IsKeyDown(KEY_EQUAL) && !IsKeyDown(KEY_MINUS)) {
        if (GetTime() - key_timeout >= KEY_DELAY) {
          wheel->slices++;
          key_timeout = GetTime();
        }
      }
      else if (IsKeyDown(KEY_MINUS) && !IsKeyDown(KEY_EQUAL)) {
        if (GetTime() - key_timeout >= KEY_DELAY) {
          if (wheel->slices > 1) wheel->slices--;
          else                   fprintf(stderr, "[ERROR] Trying to remove more slice. Cancelling\n");
          key_timeout = GetTime();
        }
      }

      if (IsKeyReleased(KEY_SPACE))
        wheel->velocity = Lerp(wheel->max_speed + WHEEL_SPIN_FORCE, wheel->max_speed, WHEEL_ACCEL);

      break;
    } else if (i <= 0) {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        Color color = GetColor(colors[GetRandomValue(0, COLOR_COUNT-1)]);
        wheels_add(wheels, wheel_count, wheel_new(GetMousePosition(), 100, 100, 10, color));
      }
    }
  }
}

void wheels_hud(Wheel *wheels, int wheel_count) {
  for (int i = wheel_count - 1; i >= 0; i--) {
    Wheel *wheel = &wheels[i];
    if (CheckCollisionPointCircle(GetMousePosition(), wheel->center, wheel->radius)) {
      char buf[100] = {0}; // TODO: unbound. autosegfault when reaches more than allocated chars
      sprintf(buf, "ID: %d\nStatus: %s\nSpeed: %.2f\nRadius: %.2f\nSlices: %d",
              i,
              wheel->max_speed == 0 ? "Stopped" : "Running",
              wheel->velocity,
              wheel->radius,
              wheel->slices);
      DrawText(buf, 0, GetScreenHeight() - FONT_SIZE*4, FONT_SIZE, GetColor(COLOR_TEXT));
      break;
    }
  }
}

int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Wheel C");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetRandomSeed(time(NULL));

  SetTargetFPS(60);

  Wheel wheels[MAX_WHEELS];
  unsigned int wheel_count = 0;
  wheels_add(wheels, &wheel_count, wheel_new(
    (Vector2) { WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f },
    200, 50, 100,
    GetColor(colors[GetRandomValue(0, COLOR_COUNT-1)])));

  while (!WindowShouldClose()) {
    /* Event */
    wheels_poll(wheels, &wheel_count);
    if (IsKeyReleased(KEY_P)) wheels_pop(wheels, &wheel_count);

    BeginDrawing();

    /* Game */
    ClearBackground(GetColor(COLOR_BASE));
    wheels_draw(wheels, wheel_count);

    /* HUD */
    DrawFPS(0, 0);
    wheels_hud(wheels, wheel_count);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
