#include "sim.h"


PhysicData* init_physics(){
    PhysicData* data = (PhysicData*)malloc(sizeof(PhysicData));
    
    data->n_point = 0;
    data->n_link = 0;
    data->points = NULL;
    data->links = NULL;
    return data;
}
Color point_color(Point* point){
    Vector3 velocity = Vector3Subtract(point->position, point->old_position);
    return ColorLerp(BLUE, RED, 10* Clamp(Vector3Length(velocity), MIN_SPEED, MAX_SPEED));
}


void render(PhysicData* data){
    for (int i = 0; i < data->n_point; i++) {
        Point point = data->points[i];
        DrawSphere(point.position, point.radius, point_color(&point));
    }
    for (int i = 0; i < data->n_link; i++){
        if (!data->links[i].skip){
            Link link = data->links[i];
            DrawLine3D(data->points[link.x1].position, data->points[link.x2].position, RAYWHITE);
        }
    }
}

void free_physic(PhysicData* data) {
    if (!data) return;
    
    if (data->points) {
        free(data->points);
        data->points = NULL;
    }
    
    if (data->links) {
        free(data->links);
        data->links = NULL;
    }
    
    free(data);
}

