#include "raylib.h"
#include <cmath>
// Want to draw some waves. 
// The waves are drawn by using the sin function.


int main(void) {
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Luovaklubbi!");

    float curtime = 0.0f;

    // float freq = 0.5f; // Looks like a cylinder spirilning!
    float freq = 0.1f; 
    float ampl = 50.0f;

    Image img = LoadImageFromScreen(); 

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose()) {

        curtime += GetFrameTime();
        BeginDrawing();
            ClearBackground(BLACK);
            
            // Uncomment to make it move! 
            /**/
            // ampl = 50.0f*sin(curtime);
            // freq = 0.5f*sin(curtime);
            
            for (int i = 0; i < screenWidth; i++) {
                // DrawPixel(i, screenHeight/2 + ampl*sin(i*freq), BEIGE);
                DrawPixel(i, screenHeight/2 + ampl*sin((i*freq - curtime)), RED);
                DrawPixel(i, screenHeight/4 + ampl*sin(i*freq + curtime), BEIGE);
                DrawPixel(i, (screenHeight/2 + screenHeight/4) + ampl*sin(i*freq + curtime), BEIGE);
                
            }
            // Draw the currentime 
            DrawText(TextFormat("Time: %02.02f", curtime), 20, screenHeight - 50, 10, RAYWHITE);
            // DrawText("Tervetuloa Luovaklubbi!", 190, 200, 20, LIGHTGRAY);
            // 
        EndDrawing();
        // TakeScreenshot("waves.png");

        img = LoadImageFromScreen(); 

        ExportImage(img, "waves.png");

    }
    CloseWindow(); 
    
    return 0;
}