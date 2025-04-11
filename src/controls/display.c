#include "../cons.h"

void handle_display(Camera3D* camera){

    // -- Fps count
    char fps[4];
    sprintf(fps, "%i", GetFPS());
    DrawText(fps, 10, 10, 16, RAYWHITE);

    if (DEBUG){
        // -- Position
        char cam_pos[64];
        sprintf(cam_pos, "Position: (%.2f, %.2f, %.2f)", camera->position.x, camera->position.y, camera->position.z);
        DrawText(cam_pos, 10, 30, 16, RAYWHITE);
    }

}
            
