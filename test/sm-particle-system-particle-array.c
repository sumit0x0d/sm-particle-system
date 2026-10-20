#include <assert.h>

#include <sm-particle-system-particle-array.h>

int main(void)
{
   SM_Particle_System_Particle_Array *particle_array;
   SM_Particle_System *particle_system;
   SM_Particle_System_Particle particle;
   SM_Particle_System_Particle *stored_particle;

   particle.position.x = 1.0f;
   particle.position.y = 2.0f;
   particle.position.z = 3.0f;
   particle.velocity.x = 4.0f;
   particle.velocity.y = 5.0f;
   particle.velocity.z = 6.0f;
   particle.color.a = 255.0f;
   particle.color.r = 10.0f;
   particle.color.g = 20.0f;
   particle.color.b = 30.0f;
   particle.alive = SM_BOOLEAN_TRUE;

   particle_array = sm_particle_system_particle_array_create(1);
   assert(particle_array != NULL);
   assert(sm_particle_system_particle_array_get_size(particle_array) == 0);
   assert(sm_particle_system_particle_array_push_back(particle_array, &particle));
   assert(sm_particle_system_particle_array_get_size(particle_array) == 1);

   particle.position.x = 99.0f;
   stored_particle = sm_particle_system_particle_array_get_data(particle_array, 0);
   assert(stored_particle != NULL);
   assert(stored_particle->position.x == 1.0f);
   assert(!sm_particle_system_particle_array_push_back(particle_array, &particle));
   assert(sm_particle_system_particle_array_get_size(particle_array) == 1);
   assert(sm_particle_system_particle_array_get_data(particle_array, 1) == NULL);
   sm_particle_system_particle_array_destroy(particle_array);

   particle_array = sm_particle_system_particle_array_create(0);
   assert(particle_array != NULL);
   assert(!sm_particle_system_particle_array_push_back(particle_array, &particle));
   sm_particle_system_particle_array_destroy(particle_array);

   particle_system = sm_particle_system_create(SM_PARTICLE_SYSTEM_RENDERER_OPENGL, 1);
   assert(particle_system != NULL);
   sm_particle_system_emit(particle_system, &particle);
   sm_particle_system_emit(particle_system, &particle);
   assert(sm_particle_system_get_particle_count(particle_system) == 1);
   sm_particle_system_destroy(particle_system);

   return 0;
}
