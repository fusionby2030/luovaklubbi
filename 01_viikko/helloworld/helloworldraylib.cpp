#include "raylib.h"

int main(void)  {
    int screenWidth = 800;
    int screenHeight = 450;
    char text[] = "Tervetuloa Luovaklubbi!";
    InitWindow(screenWidth, screenHeight, "Luovaklubbi!");
    Font font = LoadFontEx("/usr/share/fonts/NationalPark-VariableVF.ttf", 96, 0, 0);

    GenTextureMipmaps(&font.texture);
    float fontSize = (float)font.baseSize - 2.0f;
    Vector2 fontPosition = { 20.0f, screenHeight/2.0f - 80.0f };
    Vector2 textSize = { 0.0f, 0.0f };
    
    textSize = MeasureTextEx(font, text, fontSize, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    int currentFontFilter = 0;      // TEXTURE_FILTER_POINT

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Rectangle textrec = {fontPosition.x, fontPosition.y, textSize.x + 0.5f, textSize.y+ 0.5f};

    while (!WindowShouldClose()) {
        
        BeginDrawing();
            ClearBackground(GRAY);
            if (IsKeyDown(KEY_SPACE)) {
                DrawTextEx(font, text, fontPosition, fontSize, 0, BEIGE);
                DrawRectangleLinesEx(textrec, 3.0f, Fade(BEIGE, 0.5f));
                }
            else {
                DrawTextEx(font, text, fontPosition, fontSize, 0, BLACK);
                }

            DrawText(TextFormat("Font size: %02.02f", fontSize), 20, screenHeight - 50, 10, DARKGRAY);
            DrawText(TextFormat("Text size: [%02.02f, %02.02f]", textSize.x, textSize.y), 20, screenHeight - 30, 10, DARKGRAY);

        EndDrawing();
    }
    CloseWindow(); 

    return 0;
}