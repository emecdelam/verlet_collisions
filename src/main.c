#include "cons.h"
#include "controls/keys.c"
#include "controls/display.c"


int main() {
    log_info("Starting program");
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cloth Sim");
    log_info("Window created");
    
    SetTargetFPS(60);
    log_info("Raylib Version: %s", RAYLIB_VERSION);
    

    // -- Inits
    Camera3D* camera = init_camera();


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

            // -- Display
            handle_display(camera);

            // -- Draw
            BeginMode3D(*camera);
                DrawCube((Vector3){ 0, 0, 0 }, 2.0f, 2.0f, 2.0f, RED);
    
            EndMode3D();


        EndDrawing();
    }

    // -- Cleanup
    log_info("Cleaning up...");
    CloseWindow();    
    return 0;
}    
