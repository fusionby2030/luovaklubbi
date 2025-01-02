#include "raylib.h"
#include <cmath>


static const int avgvolbuffer = 800;                 // Audio volume history buffer length 
static float exponent = 1.0f;                 // Audio exponentiation value
static float averageVolume[avgvolbuffer] = { 0.0f };   // Average volume history

// Now we want to add some audio! 

static void AudioProcessEffectLPF(void *buffer, unsigned int frames);   // Audio effect: lowpass filter

void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;   // Samples internally stored as <float>s
    float average = 0.0f;               // Temporary average volume

    for (unsigned int frame = 0; frame < frames; frame++)
    {
        float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];

        // *left = powf(fabsf(*left), exponent) * ( (*left < 0.0f)? -1.0f : 1.0f );
        // *right = powf(fabsf(*right), exponent) * ( (*right < 0.0f)? -1.0f : 1.0f );

        average += fabsf(*left) / frames;   // accumulating average volume
        average += fabsf(*right) / frames;
    }

    // Moving history to the left
    for (int i = 0; i < 399; i++) averageVolume[i] = averageVolume[i + 1];

    averageVolume[400] = average;         // Adding last average value
}


int main(void) {
    int screenWidth = 1600;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Luovaklubbi!");

    float curtime = 0.0f;
    float tlastspace = 0.0f;
    float circle_radius = 200.0f;
    float theta = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float modulated_val = 0.0f; 
    float modulated_ampl = 50.0f;
    float modulated_decay_rate = 0.01f;
    float modulated_spatial_freq = 0.05f;

    InitAudioDevice(); 
    AttachAudioMixedProcessor(ProcessAudio);
    Music music = LoadMusicStream("./resources/drums_fun.wav");
    // AttachAudioStreamProcessor(music.stream, AudioProcessEffectLPF);
    PlayMusicStream(music);

    Font font = LoadFontEx("/usr/share/fonts/NationalPark-VariableVF.ttf", 96, 0, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    float curamp = 0.0f; 
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

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

            // Draw the current music volume! 

            // DrawRectangle(screenWidth/2, screenHeight - 40, 402, 34, LIGHTGRAY);
            for (int i = 0; i < avgvolbuffer; i++)
            {
                if ((screenHeight - 40 + 34 )-  (int)(averageVolume[i] * 100) < screenHeight - 40 + 17) {
                    DrawLine(screenWidth/2 + i, (screenHeight - 40 + 34 )-  (int)(averageVolume[i] * 100), screenWidth/2 + i, screenHeight - 40 + 34, GREEN);
                    tlastspace = 0.0f;
                }
                else {
                    DrawLine(screenWidth/2 + i, (screenHeight - 40 + 34 )-  (int)(averageVolume[i] * 100), screenWidth/2 + i, screenHeight - 40 + 34, RED);
                }
            }
            
            // DrawRectangleLines(screenWidth/2, screenHeight - 40, 402, 34, GRAY);
            // Draw a line horizontally halfway through the rectangle 
            // DrawLine(screenWidth/2, screenHeight - 40 + 17, screenWidth/2 + avgvolbuffer, screenHeight - 40 + 17, GRAY);
            
        EndDrawing();
    }
    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    DetachAudioMixedProcessor(ProcessAudio);  // Disconnect audio processor

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context

}  

static void AudioProcessEffectLPF(void *buffer, unsigned int frames)
{
    static float low[2] = { 0.0f, 0.0f };
    static const float cutoff = 300.0f / 44100.0f; // 200 Hz lowpass filter
    const float k = cutoff / (cutoff + 0.1591549431f); // RC filter formula

    // Converts the buffer data before using it
    float *bufferData = (float *)buffer;
    for (unsigned int i = 0; i < frames*2; i += 2)
    {
        const float l = bufferData[i];
        const float r = bufferData[i + 1];

        low[0] += k * (l - low[0]);
        low[1] += k * (r - low[1]);
        bufferData[i] = low[0];
        bufferData[i + 1] = low[1];
    }
}