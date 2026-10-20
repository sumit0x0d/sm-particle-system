#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <sm-particle-system.h>

#include "sm-particle-system-opengl.h"
#include "sm-particle-system-particle-array.h"

typedef union  {
   SM_Particle_System_OpenGL *opengl;
} SM_Particle_System_Renderer;

struct SM_Particle_System {
   SM_Particle_System_Renderer_Type renderer_type;
   SM_Particle_System_Particle_Array particle_array;
   SM_Particle_System_Renderer renderer;
};

SM_Particle_System *sm_particle_system_create(SM_Particle_System_Renderer_Type renderer_type, size_t maximum_particle_count)
{
   SM_Particle_System *this = (SM_Particle_System *)calloc(1, sizeof(*this));

   if (!this) {
      return NULL;
   }

   sm_particle_system_particle_array_create(&this->particle_array, maximum_particle_count);

   if (!this->particle_array.particle) {
      free(this);
      return NULL;
   }

   switch (renderer_type) {
      case SM_PARTICLE_SYSTEM_RENDERER_OPENGL:
         this->renderer.opengl = sm_particle_system_opengl_create();
         if (!this->renderer.opengl) {
            sm_particle_system_particle_array_destroy(&this->particle_array);
            free(this);
            return NULL;
         }
         break;
      case SM_PARTICLE_SYSTEM_RENDERER_VULKAN:
         fprintf(stderr, "Particle system: Vulkan renderer is not implemented\n");
         sm_particle_system_particle_array_destroy(&this->particle_array);
         free(this);
         return NULL;
      default:
         fprintf(stderr, "Particle system: unknown renderer type\n");
         sm_particle_system_particle_array_destroy(&this->particle_array);
         free(this);
         return NULL;
   }

   this->renderer_type = renderer_type;

   return this;
}

void sm_particle_system_destroy(SM_Particle_System *this)
{
   if (!this) {
      return;
   }

   switch (this->renderer_type) {
      case SM_PARTICLE_SYSTEM_RENDERER_OPENGL:
         sm_particle_system_opengl_destroy(this->renderer.opengl);
         break;
      case SM_PARTICLE_SYSTEM_RENDERER_VULKAN:
         break;
   }

   sm_particle_system_particle_array_destroy(&this->particle_array);
   free(this);
}

size_t sm_particle_system_get_particle_count(const SM_Particle_System *this)
{
   if (!this) {
      return 0;
   }

   return sm_particle_system_particle_array_get_size(&this->particle_array);
}

void sm_particle_system_emit(SM_Particle_System *this, SM_Particle_System_Particle *particle)
{
   if (!this || !particle) {
      return;
   }

   sm_particle_system_particle_array_push_back(&this->particle_array, particle);
}

void sm_particle_system_simulate(SM_Particle_System *this, SM_Float dt)
{
   size_t particle_count;
   size_t i;
   SM_Particle_System_Particle *particle;

   if (!this) {
      return;
   }

   particle_count = sm_particle_system_particle_array_get_size(&this->particle_array);

   for (i = 0; i < particle_count; i++) {
      particle = sm_particle_system_particle_array_get_data(&this->particle_array, i);
      if (!particle->alive) {
         continue;
      }
      particle->position.x += particle->velocity.x * dt;
      particle->position.y += particle->velocity.y * dt;
      particle->position.z += particle->velocity.z * dt;
   }
}

void sm_particle_system_render(SM_Particle_System *this)
{
   size_t i;
   size_t particle_count;
   size_t visible_count = 0;
   float *vertex_data;
   SM_Particle_System_Particle *particle;

   particle_count = sm_particle_system_particle_array_get_size(&this->particle_array);

   if (particle_count > ((size_t) - 1) / (6 * sizeof(*vertex_data))) {
      fprintf(stderr, "Particle system: too many particles to render\n");
      return;
   }

   vertex_data = (float *)malloc(particle_count * 6 * sizeof(*vertex_data));

   if (!vertex_data) {
      fprintf(stderr, "Particle system: failed to allocate render data\n");
      return;
   }

   for (i = 0; i < particle_count; i++) {
      particle = sm_particle_system_particle_array_get_data(&this->particle_array, i);
      if (!particle->alive) {
         continue;
      }
      vertex_data[visible_count * 6] = particle->position.x;
      vertex_data[visible_count * 6 + 1] = particle->position.y;
      vertex_data[visible_count * 6 + 2] = particle->position.z;
      vertex_data[visible_count * 6 + 3] = particle->color.r;
      vertex_data[visible_count * 6 + 4] = particle->color.g;
      vertex_data[visible_count * 6 + 5] = particle->color.b;
      visible_count++;
   }

   if (visible_count == 0) {
      free(vertex_data);
      return;
   }

   if (visible_count > INT_MAX ||
       visible_count > (((size_t)-1) / 2) / (6 * sizeof(*vertex_data))) {
      fprintf(stderr, "Particle system: too many particles to render\n");
      free(vertex_data);
      return;
   }

   switch (this->renderer_type) {
      case SM_PARTICLE_SYSTEM_RENDERER_OPENGL:
         sm_particle_system_opengl_render(this->renderer.opengl, vertex_data, visible_count);
         break;
      case SM_PARTICLE_SYSTEM_RENDERER_VULKAN:
         break;
   }
   free(vertex_data);
}
