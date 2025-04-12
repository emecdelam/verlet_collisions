# 3D verlet collisions

Simple collisions in C using raylib

[![Watch the video](https://img.youtube.com/vi/Vb6UVNXkIog/0.jpg
)](https://youtu.be/Vb6UVNXkIog)


# Run it yourself :)

Simply download it and run
```sh
make
```
then

```sh
make run
```

If you have raylib installed, it should work

## Verlet

The solution is found using verlet collisions in O(n^2), there are some room left for improvement.

## Rendering

Most of the hardwork is done by raylib, we simply have to draw the sphere with a color matching an interpolation of their speed, blue being slower and red faster

```c
Color point_color(Point* point){
    Vector3 velocity = Vector3Subtract(point->position, point->old_position);
    return ColorLerp(BLUE, RED, 10* Clamp(Vector3Length(velocity), MIN_SPEED, MAX_SPEED));
}
```

## Physic
The datastructure for points is here, link aren't added in the example but could be


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


