#include "sim.h"


PhysicData* init_physics(){
    PhysicData* data = (PhysicData*)malloc(sizeof(PhysicData));
    
    data->n_point = 0;
    data->n_link = 0;
    data->points = NULL;
    data->links = NULL;
    return data;
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

