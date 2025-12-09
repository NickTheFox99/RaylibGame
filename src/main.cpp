#include "../data/cube.png.h"
#include "raylib.h"
#include <cmath>
#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)
#define WIN_WIDTH (1280)
#define WIN_HEIGHT (720)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void MainLoop();

raylib::Window window(WIN_WIDTH, WIN_HEIGHT, "game",
                      FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

raylib::RenderTexture2D target(SCREEN_WIDTH, SCREEN_HEIGHT);

raylib::Image texImg =
    raylib::LoadImageFromMemory(".png", cube_png, cube_png_len);

raylib::Texture2D texture = texImg.LoadTexture();

raylib::Camera3D cam(raylib::Vector3(0.0f, 0.0f, std::sqrt(3.0f)),
                     raylib::Vector3::Zero(), raylib::Vector3(0.0f, 1.0f, 0.0f),
                     60.0f, CAMERA_PERSPECTIVE);

raylib::Model cube(GenMeshCube(1.0f, 1.0f, 1.0f));

int main(void) {
  window.SetMinSize({320, 240});
  window.SetTargetFPS(60);
  window.SetExitKey(KEY_BACKSPACE);

  target.GetTexture().SetFilter(TEXTURE_FILTER_POINT);

  cube.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(MainLoop, 0, 1);
#else
  while (!window.ShouldClose() && !raylib::Keyboard::IsKeyDown(KEY_ESCAPE))
    MainLoop();
#endif

  window.Close();

  return 0;
}

void MainLoop() {
  {
    int winWidth = GetScreenWidth(), winHeight = GetScreenHeight();
    float scale =
        MIN((float)winWidth / SCREEN_WIDTH, (float)winHeight / SCREEN_HEIGHT);

    cube.SetTransform(
        raylib::Matrix::RotateXYZ(
            raylib::Vector3(1.0f, 1.5f, 2.5f).Scale(window.GetFrameTime()))
            .Multiply(cube.GetTransform()));

    target.BeginMode();
    {
      ClearBackground(BLACK);
      raylib::Vector2(160.0f, 120.0f)
          .DrawCircle(120.0f, raylib::Color::White());
      cam.BeginMode();
      {
        cube.Draw(raylib::Vector3::Zero());
      }
      cam.EndMode();
    }
    target.EndMode();

    window.BeginDrawing();
    {
      ClearBackground(BLACK);
      target.GetTexture().Draw(
          raylib::Rectangle(0.0f, 0.0f, (float)SCREEN_WIDTH,
                            (float)-SCREEN_HEIGHT),
          raylib::Rectangle((winWidth - ((float)SCREEN_WIDTH * scale)) * 0.5f,
                            (winHeight - ((float)SCREEN_HEIGHT * scale)) * 0.5f,
                            (float)SCREEN_WIDTH * scale,
                            (float)SCREEN_HEIGHT * scale),
          raylib::Vector2(0, 0), 0.0f, WHITE);
    }
    window.EndDrawing();
  }
}
