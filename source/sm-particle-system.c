#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <sm-particle-system.h>

#include "sm-particle-system-opengl.h"
#include "sm-particle-system-particle-array.h"

union SM_Particle_System_Renderer {
   SM_Particle_System_OpenGL *opengl;
};

struct SM_Particle_System {
   SM_Particle_System_Renderer_Type renderer_type;
   SM_Particle_System_Particle_Array *particle;
   union SM_Particle_System_Renderer renderer;
};

SM_Particle_System *sm_particle_system_create(SM_Particle_System_Renderer_Type renderer_type, size_t particle_capacity)
{
   SM_Particle_System *this = (SM_Particle_System *)calloc(1, sizeof(*this));

   if (!this) {
      return NULL;
   }

   this->particle = sm_particle_system_particle_array_create(particle_capacity);

   if (!this->particle) {
      free(this);
      return NULL;
   }

   switch (renderer_type) {
      case SM_PARTICLE_SYSTEM_RENDERER_OPENGL:
         this->renderer.opengl = sm_particle_system_opengl_create();
         if (!this->renderer.opengl) {
            sm_particle_system_particle_array_destroy(this->particle);
            free(this);
            return NULL;
         }
         break;
      case SM_PARTICLE_SYSTEM_RENDERER_VULKAN:
         fprintf(stderr, "Particle system: Vulkan renderer is not implemented\n");
         sm_particle_system_particle_array_destroy(this->particle);
         free(this);
         return NULL;
      default:
         fprintf(stderr, "Particle system: unknown renderer type\n");
         sm_particle_system_particle_array_destroy(this->particle);
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
   sm_particle_system_particle_array_destroy(this->particle);
   free(this);
}

size_t sm_particle_system_get_particle_count(const SM_Particle_System *this)
{
   if (!this) {
      return 0;
   }

   return sm_particle_system_particle_array_get_size(this->particle);
}

void sm_particle_system_emit(SM_Particle_System *this, SM_Particle_System_Particle *particle)
{
   if (!this || !particle) {
      return;
   }

   if (!sm_particle_system_particle_array_push_back(this->particle, particle)) {
      fprintf(stderr, "Particle system: failed to store emitted particle\n");
   }
}

void sm_particle_system_simulate(SM_Particle_System *this, SM_Float dt)
{
   size_t i;
   size_t particle_count;

   if (!this) {
      return;
   }

   particle_count = sm_particle_system_particle_array_get_size(this->particle);
   for (i = 0; i < particle_count; i++) {
      SM_Particle_System_Particle *particle = sm_particle_system_particle_array_get_data(this->particle, i);
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

   if (!this) {
      return;
   }

   particle_count = sm_particle_system_particle_array_get_size(this->particle);

   if (particle_count == 0) {
      return;
   }

   if (particle_count > ((size_t)-1) / (6 * sizeof(*vertex_data))) {
      fprintf(stderr, "Particle system: too many particles to render\n");
      return;
   }

   vertex_data = (float *)malloc(particle_count * 6 * sizeof(*vertex_data));

   if (!vertex_data) {
      fprintf(stderr, "Particle system: failed to allocate render data\n");
      return;
   }

   for (i = 0; i < particle_count; i++) {
      const SM_Particle_System_Particle *particle =
          sm_particle_system_particle_array_get_data(this->particle, i);
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
