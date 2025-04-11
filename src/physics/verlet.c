#include "verlet.h"



void update_physic(PhysicData* data, float dt, Vector3 wind) {
    verlet_point_solver(data, dt, wind);
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < data->n_link; i++) {
            if (!data->links[i].skip){
                verlet_link(data, &data->links[i]);
            }
        }
    }
}

void verlet_point(Point* point, float dt) {
    if (point->constrained || point->skip) {
        return;
    }
    Vector3 velocity = Vector3Subtract(point->position, point->old_position);
    point->old_position = point->position;
    point->position = Vector3Add(Vector3Add(point->position, velocity), Vector3Scale(point->acceleration, dt * dt));
    DrawSphere(point->position, point->radius, RAYWHITE);
}

void verlet_point_solver(PhysicData* data, float dt, Vector3 acc) {
    for (int i = 0; i < data->n_point; i++) {
        data->points[i].acceleration = Vector3Add(GRAVITY, acc);
        verlet_point(&data->points[i], dt);
    }
}

void verlet_link(PhysicData* data, Link* link) {
    Point* x1 = &data->points[link->x1];
    Point* x2 = &data->points[link->x2];
    
    Vector3 axis = Vector3Subtract(x1->position, x2->position);
    float length = Vector3Length(axis);
    if (length < 0.0001f) return;
    
    Vector3 n = Vector3Scale(axis, 1.0f/length);
    float delta = link->target - length;
    
    if (x1->constrained && x2->constrained) {
        return;
    } 
    else if (x1->constrained) {
        x2->position = Vector3Subtract(x2->position, Vector3Scale(n, delta));
    }
    else if (x2->constrained) {
        x1->position = Vector3Add(x1->position, Vector3Scale(n, delta));
    }
    else {
        x1->position = Vector3Add(x1->position, Vector3Scale(n, delta * 0.5f));
        x2->position = Vector3Subtract(x2->position, Vector3Scale(n, delta * 0.5f));
    }
    DrawLine3D(x1->position, x2->position, RAYWHITE);
}





int find_point(PhysicData* data, Vector3 pos, float epsilon) {
    for (int i = 0; i < data->n_point; i++) {
        Vector3 p = data->points[i].position;
        if (fabsf(p.x - pos.x) < epsilon && 
            fabsf(p.y - pos.y) < epsilon && 
            fabsf(p.z - pos.z) < epsilon) {
            return i;
        }
    }
    return -1;
}

int add_point(PhysicData* data, Vector3 pos) {
    int idx = find_point(data, pos, 0.001f);
    if (idx >= 0) {
        return idx;
    }
    
    data->points = (Point*) realloc(data->points, sizeof(Point) * (data->n_point + 1));
    
    Point p = {
        .position = pos,
        .old_position = pos,
        .acceleration = (Vector3){0.0f, 0.0f, 0.0f},
        .radius = RADIUS,
        .constrained = false,
        .skip = false
    };
    
    data->points[data->n_point] = p;
    return data->n_point++;
}


void add_link(PhysicData* data, Vector3 x1, Vector3 x2) {
    int idx1 = add_point(data, x1);
    int idx2 = add_point(data, x2);
    
    if (idx1 == idx2) {
        return;
    }
    
    for (int i = 0; i < data->n_link; i++) {
        if ((data->links[i].x1 == idx1 && data->links[i].x2 == idx2) ||
            (data->links[i].x1 == idx2 && data->links[i].x2 == idx1)) {
            return;
        }
    }
    
    data->links = (Link*) realloc(data->links, sizeof(Link) * (data->n_link + 1));
    
    Link l = {.x1 = idx1, .x2 = idx2, .target = Vector3Distance(x1, x2), .skip = false};
    data->links[data->n_link] = l;
    data->n_link++;
}

void render(PhysicData* data){
    for (int i = 0; i < data->n_link; i++){
        Link link = data->links[i];
        if (!link.skip){
           
        }
    }
    for (int i = 0; i < data->n_point; i++){
        Point point = data->points[i];
        if (!point.skip){
            
        }
    }
}