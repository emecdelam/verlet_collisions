#include "cons.h"
#include "controls/keys.c"



int main() {
    log_info("Starting program");
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cloth Sim");
    log_info("Window created");
    
    SetTargetFPS(60);
    log_info("Raylib Version: %s", RAYLIB_VERSION);
    

    // -- Camera
    Camera3D* camera = init_camera();

    

    // -- Inits




    // -- Main game loop
    log_info("Entering main loop");
    while (!WindowShouldClose()) {
        
        // -- Key press
        handle_key();

        // -- Physics
        float dt = GetFrameTime();
        if (dt > 0.016f) dt = 0.016f;


        // -- Drawing
        BeginDrawing();
            ClearBackground((Color){ 50, 50, 50, 255 });

            // -- Fps count
            char fps[4];
            sprintf(fps, "%i", GetFPS());
            DrawText(fps, 10, 20, 16, RAYWHITE);

            // -- Draw
            BeginMode3D(*camera);
                DrawCube((Vector3){ 0, 0, 0 }, 2.0f, 2.0f, 2.0f, RED);
    
            EndMode3D();

        
            char camPosition[64];
            sprintf(camPosition, "Camera: (%.2f, %.2f, %.2f)", camera->position.x, camera->position.y, camera->position.z);
            DrawText(camPosition, 10, 10, 10, RAYWHITE);
        EndDrawing();
    }

    // -- Cleanup
    log_info("Cleaning up...");
    CloseWindow();    
    return 0;
}    
