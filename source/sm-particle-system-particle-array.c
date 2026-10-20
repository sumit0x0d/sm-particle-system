#include <stdlib.h>

#include "sm-particle-system-particle-array.h"

void sm_particle_system_particle_array_create(SM_Particle_System_Particle_Array *this, size_t capacity)
{
   if (capacity > ((size_t) - 1) / sizeof(SM_Particle_System_Particle)) {
      return;
   }
   
   this->particle = (SM_Particle_System_Particle *)malloc(capacity * sizeof(SM_Particle_System_Particle));

   if (!this->particle) {
      free(this);
      return;
   }

   this->size = 0;
   this->capacity = capacity;
}

void sm_particle_system_particle_array_destroy(SM_Particle_System_Particle_Array *this)
{
   if (!this) {
      return;
   }

   free(this->particle);
}

size_t sm_particle_system_particle_array_get_size(const SM_Particle_System_Particle_Array *this)
{
   return this->size;
}

SM_Particle_System_Particle *sm_particle_system_particle_array_get_data(SM_Particle_System_Particle_Array *this, size_t index)
{
   return &this->particle[index];
}

void sm_particle_system_particle_array_push_back(SM_Particle_System_Particle_Array *this, const SM_Particle_System_Particle *particle)
{
   this->particle[this->size] = *particle;
   this->size++;
}