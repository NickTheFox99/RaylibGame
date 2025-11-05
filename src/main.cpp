#include "../data/cube.png.h"
#include <raylib-cpp.hpp>

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(void) {
  raylib::Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "game",
                        FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE |
                            FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
  window.SetMinSize({320, 240});
  window.SetTargetFPS(60);

  raylib::RenderTexture2D target(320, 240);

  target.GetTexture().SetFilter(TEXTURE_FILTER_POINT);

  raylib::Image texImg(".png", assets_cube_png, assets_cube_png_len);
  raylib::Texture2D texture(texImg);

  raylib::Camera3D cam(raylib::Vector3::Zero(), raylib::Vector3::Zero(),
                       {0.0f, 1.0f, 0.0f}, 60.0f, CAMERA_PERSPECTIVE);

  Model cube = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
  cube.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

  while (!WindowShouldClose()) {

    float scale = MIN((float)GetScreenWidth() / SCREEN_WIDTH,
                      (float)GetScreenHeight() / SCREEN_HEIGHT);

    cube.transform =
        MatrixMultiply(MatrixRotateXYZ(Vector3Scale((Vector3){1.0f, 1.5f, 2.5f},
                                                    GetFrameTime())),
                       cube.transform);

    BeginTextureMode(target);
    {
      ClearBackground(BLACK);
      BeginMode3D(camera);
      {
        DrawModel(cube, Vector3Zero(), 1.0f, WHITE);
      }
      EndMode3D();
    }
    EndTextureMode();

    BeginDrawing();
    {
      ClearBackground(BLACK);
      DrawTexturePro(
          target.texture,
          (Rectangle){0.0f, 0.0f, (float)target.texture.width,
                      (float)-target.texture.height},
          (Rectangle){
              (GetScreenWidth() - ((float)SCREEN_WIDTH * scale)) * 0.5f,
              (GetScreenHeight() - ((float)SCREEN_HEIGHT * scale)) * 0.5f,
              (float)SCREEN_WIDTH * scale, (float)SCREEN_HEIGHT * scale},
          (Vector2){0, 0}, 0.0f, WHITE);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
