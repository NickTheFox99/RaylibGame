#include "raylib.h"
#include "raymath.h"

#include "../data/cube.png.h"

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE |
                 FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
  SetWindowMinSize(320, 240);
  SetTargetFPS(60);

  RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

  Image image =
      LoadImageFromMemory(".png", assets_cube_png, assets_cube_png_len);

  Texture2D texture = LoadTextureFromImage(image);

  Camera camera = {0};
  camera.position = (Vector3){0.0f, 0.0f, 5.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

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
