#include "raylib.h"
#include <cmath>
#include <iostream>
#include <fftw3.h>

static float interesting_freq_amplitudes[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static float interesting_freq_amplitudes_avg[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const float interesting_freqsranges[6][2] = {{0.0f, 10.0f}, {10.0f, 100.0f}, {100.0f, 1000.0f}, {1000.0f, 10000.0f}, {10000.0f, 100000.0f}, {100000.0f, 1000000.0f}};

void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;   // Samples internally stored as <float>s
    float average = 0.0f;               // Temporary average volume
    int sampleRate = 44100;             // Sample rate (samples per second)
    float peak = 0.0f;                  // Peak amplitude
    // get length of the intersting_freq_ranges
    int interesting_freq_ranges_length = sizeof(interesting_freqsranges) / sizeof(interesting_freqsranges[0]) ;

    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (frames / 2 + 1));
    fftw_plan plan = fftw_plan_dft_r2c_1d(frames, (double *)samples, out, FFTW_ESTIMATE);
    fftw_execute(plan);
    float frequency = 2000.0f;

    for (unsigned int j=0; j < interesting_freq_ranges_length; j++) {
        interesting_freq_amplitudes[j] = 0.0f;
        interesting_freq_amplitudes_avg[j] = 0.0f;
    }

    for (unsigned int i = 0; i < frames / 2; i++) {
        // (float)i * sampleRate / frames
        // minimum w.r.t to set minimum 
        frequency = (float)i * sampleRate / frames; 
        float magnitude = 1000.0f*sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        // loop over interesting_freqsranges
        for (int j = 0; j < interesting_freq_ranges_length; j++) {
            
            if (frequency >= interesting_freqsranges[j][0] && frequency <= interesting_freqsranges[j][1]) {
                if (magnitude > interesting_freq_amplitudes[j]) {
                    interesting_freq_amplitudes[j] = magnitude;
                }
                interesting_freq_amplitudes_avg[j] += magnitude;
            }
        }
        
    }
    printf("Frequency: %f\n", frequency);


    for (int i = 0; i < interesting_freq_ranges_length; i++) {
        interesting_freq_amplitudes_avg[i] /= (interesting_freqsranges[i][1] - interesting_freqsranges[i][0]);
        printf("Interesting Frequency Amplitude: %f->%f | Peak %f Avg. %f \n", interesting_freqsranges[i][0],interesting_freqsranges[i][1],  interesting_freq_amplitudes[i], interesting_freq_amplitudes_avg[i]);
    }

    fftw_destroy_plan(plan);
    fftw_free(out);
}


int main(void) {
    int screenWidth = 1600;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Luovaklubbi!");
    float curtime = 0.0f;
    Font font = LoadFontEx("/usr/share/fonts/NationalPark-VariableVF.ttf", 96, 0, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    InitAudioDevice(); 
    AttachAudioMixedProcessor(ProcessAudio);
    Music music = LoadMusicStream("./resources/drums_fun.wav");
    PlayMusicStream(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        curtime += GetFrameTime();
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextEx(font, TextFormat("Time: %02.02f", curtime), (Vector2){20.0f, screenHeight - 30.0f}, 30.0f, 0, BEIGE);
        EndDrawing();
        // Print the current buffer of music to console 
    
    }

    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    CloseWindow();              // Close window and OpenGL context
}