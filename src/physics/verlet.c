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
    apply_collisions(data);
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
    point->acceleration = acc;
}


/**
 * Apply constraint to the point
 * @param Point* the point
 */
void apply_constraint(Point* point) {
    Vector3 velocity = Vector3Subtract(point->position, point->old_position);

    if (point->position.y - point->radius < 0.0f) {
        point->position.y = point->radius;
        // Invert the velocity in Y
        velocity.y = -velocity.y * BOUNCE;
        // Recompute old_position so it reflects the new velocity
        point->old_position = Vector3Subtract(point->position, velocity);
    }

    if (point->position.x > (BORDER - point->radius)) {
        point->position.x = BORDER - point->radius;
        velocity.x = -velocity.x * BOUNCE;
        point->old_position = Vector3Subtract(point->position, velocity);
    } 
    else if (point->position.x < (-BORDER + point->radius)) {
        point->position.x = -BORDER + point->radius;
        velocity.x = -velocity.x * BOUNCE;
        point->old_position = Vector3Subtract(point->position, velocity);
    }

    if (point->position.z > (BORDER - point->radius)) {
        point->position.z = BORDER - point->radius;
        velocity.z = -velocity.z * BOUNCE;
        point->old_position = Vector3Subtract(point->position, velocity);
    } 
    else if (point->position.z < (-BORDER + point->radius)) {
        point->position.z = -BORDER + point->radius;
        velocity.z = -velocity.z * BOUNCE;
        point->old_position = Vector3Subtract(point->position, velocity);
    }
}


/**
 * Apply verlet position for a point
 * @param Point* the point
 * @param float the time interval
 */
void verlet_point(Point* point, float dt) {
    Vector3 velocity = Vector3Subtract(point->position, point->old_position);
    point->old_position = point->position;
    point->position = Vector3Add(Vector3Add(point->position, velocity), Vector3Scale(point->acceleration, dt * dt));
}

/**
 * Applies collisions to each point
 * @param PhysicData* the simulation
 */
void apply_collisions(PhysicData* data){
    for (int i = 0; i < data->n_point; i++) {
        for (int j = 0; j < data->n_point; j++) {
            if (i == j) { 
                continue; 
            }
            Point *p1 = &data->points[i];
            Point *p2 = &data->points[j];

            Vector3 axis = Vector3Subtract(p1->position, p2->position);
            float dist = Vector3Length(axis);
            float combinedRadius = p1->radius + p2->radius;

            if (dist < combinedRadius) {
                float delta = combinedRadius - dist;
                Vector3 normal = Vector3Scale(axis, 1.0f/dist);
                p1->position = Vector3Add(p1->position, Vector3Scale(normal,  0.5f * delta));
                p2->position = Vector3Subtract(p2->position, Vector3Scale(normal, 0.5f * delta));
            }
        }
    }
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
