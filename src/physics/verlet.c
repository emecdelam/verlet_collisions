#include "verlet.h"



/**
 * Updates physics by using the point solver
 * @param PhysicData* the simulation data
 * @param Vector3 any externet acceleration than gravity
 */
void update_physic(PhysicData* data, float dt, Vector3 acc) {
    for (int i = 0; i < data->n_point; i++) {
        Point* point = &data->points[i];
        if (point->constrained || point->skip) {
            continue;
        }
        apply_acceleration(point, Vector3Add(GRAVITY, acc));
        apply_constraint(point);
        verlet_point(point, dt);
    }
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < data->n_link; i++) {
            if (!data->links[i].skip){
                verlet_link(data, &data->links[i]);
            }
        }
    }
}



/**
 * Apply acceleration to a point
 * @param Point* the point
 * @param Vector3 the acceleration
 */
void apply_acceleration(Point* point, Vector3 acc){
    log_debug("acceleration is %f, %f, %f", acc.x, acc.y, acc.z);
    point->acceleration = acc;
}



/**
 * Apply verlet position for a point
 * @param Point* the point
 * @param float the time interval
 */
void verlet_point(Point* point, float dt) {
    log_debug("Point pos and old_pos %f,%f,%f, %f, %f, %f",point->position.x, point->position.y, point->position.z,point->old_position.x, point->old_position.y, point->old_position.z);
    Vector3 velocity = Vector3Subtract(point->position, point->old_position);
    log_debug("Point moving at %f,%f,%f",velocity.x, velocity.y, velocity.z);
    point->old_position = point->position;
    point->position = Vector3Add(Vector3Add(point->position, velocity), Vector3Scale(point->acceleration, dt * dt));
    log_debug("Drawing point at %f, %f, %f", point->position.x, point->position.y, point->position.z);
    DrawSphere(point->position, point->radius, RAYWHITE);
}



/**
 * Apply constraint to the point
 */
void apply_constraint(Point* point){
    if (point->position.y < 0){
        point->position.y = 0.0;
    }
    if (point->position.x > 2.0){
        point->position.x = 2.0;
    }
    if (point->position.x < -2.0){
        point->position.x = -2.0;
    }
    if (point->position.z > 2.0){
        point->position.z = 2.0;
    }
    if (point->position.z < -2.0){
        point->position.z = -2.0;
    }
    return;
}



/**
 * Apply verlet link
 * @param PhysicData* the physic object witht the points
 * @param Link* the link to apply verlet on
 */
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




/**
 * Find a single point in the physic simulation
 * @param PhysicData* the simulation data
 * @param Vector3 the position of the point
 * @param float the confidence interval
 */
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



/**
 * Adds a point in the simulation
 * @param PhysicData* the simulation data
 * @param Vector3 the position of the point
 */
int add_point(PhysicData* data, Vector3 pos) {
    log_warn("Adding point at %f,%f,%f", pos.x, pos.y, pos.z);
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
    log_warn("Point added at %f,%f,%f", p.position.x, p.position.y, p.position.z);
    log_warn("Point added at %f,%f,%f", p.old_position.x, p.old_position.y, p.old_position.z);
    data->points[data->n_point] = p;
    return data->n_point++;
}



/**
 * Add a single link in the simulation
 * @param PhysicData* the simulation data
 * @param Vector3 the position of the first point
 * @param Vector3 the positino of the second point
 */
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
