#include "cons.h"
#include "controls/keys.c"
#include "controls/display.c"
#include "physics/sim.c"


int main() {
    log_info("Starting program");
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cloth Sim");
    log_info("Window created");
    
    SetTargetFPS(60);
    log_info("Raylib Version: %s", RAYLIB_VERSION);
    

    // -- Inits
    Camera3D* camera = init_camera();
    PhysicData* data = init_physics();
    add_point(data, (Vector3){0.0, 1.0, 0.0});

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



            // -- Draw
            BeginMode3D(*camera);
                DrawPlane((Vector3){0, 0, 0}, (Vector2){ 2.0, 2.0}, BLUE);
                //DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
                update_physic(data, dt, (Vector3){0.0, 0.0, 0.0});
            EndMode3D();

            // -- Display
            handle_display(camera);
        EndDrawing();
    }

    // -- Cleanup
    log_info("Cleaning up...");
    free_physic(data);
    CloseWindow();    
    return 0;
}    
