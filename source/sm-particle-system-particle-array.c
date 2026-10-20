#include <stdlib.h>

#include "sm-particle-system-particle-array.h"

SM_Particle_System_Particle_Array *sm_particle_system_particle_array_create(size_t capacity)
{
   SM_Particle_System_Particle_Array *this;

   if (capacity > ((size_t)-1) / sizeof(SM_Particle_System_Particle)) {
      return NULL;
   }

   this = (SM_Particle_System_Particle_Array *)malloc(sizeof(*this));

   if (!this) {
      return NULL;
   }

   this->data = NULL;
   this->size = 0;
   this->capacity = capacity;
   
   if (capacity != 0) {
      this->data = (SM_Particle_System_Particle *)malloc(capacity * sizeof(SM_Particle_System_Particle));
      if (!this->data) {
         free(this);
         return NULL;
      }
   }

   return this;
}

void sm_particle_system_particle_array_destroy(SM_Particle_System_Particle_Array *this)
{
   if (!this) {
      return;
   }

   free(this->data);
   free(this);
}

size_t sm_particle_system_particle_array_get_size(const SM_Particle_System_Particle_Array *this)
{
   if (!this) {
      return 0;
   }

   return this->size;
}

SM_Particle_System_Particle *sm_particle_system_particle_array_get_data(SM_Particle_System_Particle_Array *this, size_t index)
{
   if (!this || index >= this->size) {
      return NULL;
   }

   return &this->data[index];
}

int sm_particle_system_particle_array_push_back(SM_Particle_System_Particle_Array *this, const SM_Particle_System_Particle *particle)
{
   if (!this || !particle) {
      return 0;
   }

   if (this->size >= this->capacity) {
      return 0;
   }

   this->data[this->size] = *particle;
   this->size++;
   return 1;
}