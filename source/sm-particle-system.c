#include <stdlib.h>

#include <glad.h>

#include <sm-dynamic-array.h>
#include <sm-particle-system.h>

struct SM_Particle_System {
     SM_Dynamic_Array *particle;
     GLuint vao;
     GLuint vbo;
};

// static void _traverse_particle(void *data, void *uData);

SM_Particle_System *sm_particle_system_create(void)
{
     SM_Particle_System *this;
     size_t i;

     this = (SM_Particle_System *)malloc(sizeof(SM_Particle_System));
     if (!this) {
          return NULL;
     }

     this->particle = sm_dynamic_array_create(sizeof(SM_Particle_System_Particle), 100, 2.0);
     if (!this->particle) {
          free(this);
          return NULL;
     }

     // for (i = 0; i < 100; i++) {
     //      SM_DynamicArray_PushBack(this->particle, &cInfo->particle);
     // };

     /* glGenVertexArrays(1, &this->vao); */
     /* glGenBuffers(1, &this->vbo); */
     /* glBindVertexArray(this->vao); */
     /* glBindBuffer(GL_ARRAY_BUFFER, this->vbo); */
     /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); */
     /* glEnableVertexAttribArray(0); */
     /* glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); */
     /* glEnableVertexAttribArray(1); */
     /* glBindBuffer(GL_ARRAY_BUFFER, 0); */
     /* glBindVertexArray(0); */

     return this;
}

void sm_particle_system_destroy(SM_Particle_System *this)
{
     sm_dynamic_array_destroy(this->particle);
     free(this);
}

size_t sm_particle_system_get_particle_count(const SM_Particle_System *this)
{
     return sm_dynamic_array_get_size(this->particle);
}

void sm_particle_system_emit(SM_Particle_System *this, SM_Particle_System_Particle *particle)
{
     sm_dynamic_array_push_back(this->particle, particle);
}

// void SM_Particle_System_Simulate(SM_Particle_System *this, SimulateInfo *sInfo)
// {
     // SM_DynamicArray_Traverse(this->particle_dynamic_array, _traverse_particle, sInfo);
// }

void sm_particle_system_render(SM_Particle_System *this)
{
     
}

static void _traverse_particle(void *data, void *uData)
{
     SM_Particle_System_Particle *particle = (SM_Particle_System_Particle *)data;
     float dt = 10 /*((SimulateInfo *)uData)->dt*/;
     particle->position.x += particle->velocity.x * dt;
     particle->position.y += particle->velocity.y * dt;
     particle->position.z += particle->velocity.z * dt;
}
