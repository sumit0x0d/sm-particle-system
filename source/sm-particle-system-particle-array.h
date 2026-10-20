#ifndef SM_PARTICLE_SYSTEM_PARTICLE_ARRAY_H
#define SM_PARTICLE_SYSTEM_PARTICLE_ARRAY_H

#include <stddef.h>

#include <sm-particle-system.h>

typedef struct SM_Particle_System_Particle_Array SM_Particle_System_Particle_Array;

struct SM_Particle_System_Particle_Array {
   SM_Particle_System_Particle *data;
   size_t size;
   size_t capacity;
};

SM_Particle_System_Particle_Array *sm_particle_system_particle_array_create(size_t capacity);

void sm_particle_system_particle_array_destroy(SM_Particle_System_Particle_Array *this);

size_t sm_particle_system_particle_array_get_size(const SM_Particle_System_Particle_Array *this);

SM_Particle_System_Particle *sm_particle_system_particle_array_get_data(SM_Particle_System_Particle_Array *this, size_t index);

int sm_particle_system_particle_array_push_back(SM_Particle_System_Particle_Array *this, const SM_Particle_System_Particle *particle);

#endif