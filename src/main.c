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
      wheel_draw();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
