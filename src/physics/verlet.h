#pragma once


#include "../cons.h"

typedef struct Point {
    Vector3 position;
    Vector3 acceleration;
    Vector3 old_position;
    float radius;
    bool constrained;
    bool skip;
} Point;

typedef struct Link {
    int x1;
    int x2;
    float target;
    bool skip;
} Link;

typedef struct PhysicData {
    int n_point;
    Point* points;
    int n_link;
    Link* links;
} PhysicData;

void verlet_point(Point* point, float dt);
void verlet_link(PhysicData* cloth, Link* link);
void apply_acceleration(Point* point, Vector3 acc);
void apply_constraint(Point* point);
void update_physic(PhysicData* data, float dt, Vector3 acc);
int find_point(PhysicData* data, Vector3 pos, float epsilon);
int add_point(PhysicData* data, Vector3 pos);
void add_link(PhysicData* data, Vector3 x1, Vector3 x2);