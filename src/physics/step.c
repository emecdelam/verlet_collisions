#include "../cons.h"
#include "sim.c"





int __step;

float random_range(float minVal, float maxVal) {
    float randNorm = (float)rand() / (float)RAND_MAX;  // range [0..1]
    return minVal + randNorm * (maxVal - minVal);      // scale and shift
}

int check_step(PhysicData* data, float time){
    if (time / STEP_TIME < __step){
        return __step;
    }
    __step++;
    add_point(data, (Vector3){random_range(-0.1, 0.1), 3.0, random_range(-0.1, 0.1)});
    return __step;
}