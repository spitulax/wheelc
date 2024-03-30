#include "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Yes, this is catppuccin mocha
#define COLOR_BACKGROUND GetColor(0X101020ff)
#define COLOR_TEXT GetColor(0xcdd6f4ff)
#define COLOR_BLUE GetColor(0x89b4faff)
#define COLOR_RED GetColor(0xf38ba8ff)

void wheel_draw(Vector2 center, float radius, float speed, int lines, Color color) {
  DrawCircleV(center, radius, color);
  for (int i = 0; i < lines; i++) {
    float gap = 360.0f/lines*i;
    float angle = GetTime()*speed + gap;
    DrawCircleSector(center, radius, angle, angle + 360.0f/lines, 100, ColorBrightness(color, -0.7 + 0.1 * (i % 10)));
  }
}

int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Random ahh");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(COLOR_BACKGROUND);
      wheel_draw((Vector2){ WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f }, 300, 50, 100, COLOR_RED);
      wheel_draw((Vector2){ 100, 400 }, 100, 100, 30, COLOR_BLUE);
      DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
