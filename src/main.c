#include "cons.h"
#include "controls/keys.c"
#include "controls/display.c"
#include "physics/step.c"


int main() {
    log_info("Starting program");
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cloth Sim");
    log_info("Window created");
    
    SetTargetFPS(60);
    log_info("Raylib Version: %s", RAYLIB_VERSION);
    

    // -- Inits
    Camera3D* camera = init_camera();
    PhysicData* data = init_physics();

    float time = 0.0;
    int step;
    // -- Main game loop
    log_info("Entering main loop");
    while (!WindowShouldClose()) {
        
        // -- Key press
        handle_key();

        // -- Physics
        float dt = GetFrameTime();
        if (dt > 0.016f) dt = 0.016f;
        time += dt;
        update_physic(data, dt, (Vector3){0.0, 0.0, 0.0});

        // -- Step
        step = check_step(data, time);


        // -- Drawing
        BeginDrawing();
            ClearBackground((Color){ 50, 50, 50, 255 });



            // -- Draw
            BeginMode3D(*camera);
                DrawPlane((Vector3){0, 0, 0}, (Vector2){2*BORDER, 2*BORDER}, GRAY);
                render(data);
            EndMode3D();

            // -- Display
            handle_display(camera, step);
        EndDrawing();
    }

    // -- Cleanup
    log_info("Cleaning up...");
    free_physic(data);
    CloseWindow();    
    return 0;
}    
