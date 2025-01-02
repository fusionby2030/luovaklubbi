#include "raylib.h"
#include <cmath>

// Now we want to add some audio! 

int main(void) {
    int screenWidth = 1600;
    int screenHeight = 800;

    float curtime = 0.0f;
    float tlastspace = 0.0f;
    float circle_radius = 200.0f;
    float theta = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float modulated_val = 0.0f; 
    float modulated_ampl = 50.0f;
    float modulated_decay_rate = 0.1f;
    float modulated_spatial_freq = 0.05f;
    InitWindow(screenWidth, screenHeight, "Luovaklubbi!");
    Font font = LoadFontEx("/usr/share/fonts/NationalPark-VariableVF.ttf", 96, 0, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose()) {
        curtime += GetFrameTime();
        tlastspace += GetFrameTime();
        if (IsKeyDown(KEY_SPACE)) {
            tlastspace = 0.0f;
        }
        modulated_ampl = 50.0f*pow(modulated_decay_rate, tlastspace);

        BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < 1000; i++) {
                // DrawPixel(i, screenHeight/2 + 50*sin(i*0.1f + curtime), BEIGE);
                theta = i / 1000.0f * 2.0f * PI;
                modulated_val = modulated_ampl*sin(i*modulated_spatial_freq + curtime);
                x = screenWidth/2 + circle_radius * cos(theta);
                y = screenHeight/2 + circle_radius * sin(theta);
                // DrawPixel(x, y, BEIGE);
                DrawPixel(x + modulated_val, y + modulated_val, BEIGE);
            }
            DrawTextEx(font, TextFormat("Amplitude: %02.02f", modulated_ampl), (Vector2){20.0f, screenHeight - 90.0f}, 30.0f, 0, BEIGE);
            DrawTextEx(font, TextFormat("Time: %02.02f", curtime), (Vector2){20.0f, screenHeight - 30.0f}, 30.0f, 0, BEIGE);
            DrawTextEx(font, TextFormat("Last space: %02.02f", tlastspace), (Vector2){20.0f, screenHeight - 60.0f}, 30.0f, 0, BEIGE);
        EndDrawing();
    }

}  