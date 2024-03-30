#include "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include "wheel.h"
#include "color.h"

int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Random ahh");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(COLOR_BACKGROUND);
      wheel_draw((Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f }, 300, 50, 12, COLOR_RED);
      wheel_draw((Vector2){ 100, 400 }, 100, 100, 3, COLOR_BLUE);
      DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
