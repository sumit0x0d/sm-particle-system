#include <stdlib.h>

#include <glad.h>

#include <sm-dynamic-array.h>
#include <particle-system.h>

struct SM_ParticleSystem {
     SM_DynamicArray *particle;
     GLuint vao;
     GLuint vbo;
}

static void _traverse_particle(void *data, void *uData);

SM_ParticleSystem *SM_ParticleSystem_Create(void)
{
     SM_ParticleSystem *this;
     size_t i;

     this = (SM_ParticleSystem *)malloc(sizeof(SM_ParticleSystem));
     if (!this) {
          return NULL;
     }

     this->particle = SM_DynamicArray_Create(sizeof(SM_ParticleSystemParticle), particle_count, 2.0);
     if (!this->particle) {
          free(this);
          return NULL;
     }

     for (i = 0; i < particle_count; i++) {
          SM_DynamicArray_PushBack(pSystem->particle_dynamic_array, &cInfo->particle);
     };

     glGenVertexArrays(1, &pSystem->vao);
     glGenBuffers(1, &pSystem->vbo);
     glBindVertexArray(pSystem->vao);
     glBindBuffer(GL_ARRAY_BUFFER, pSystem->vbo);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(1);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glBindVertexArray(0);

     return pSystem;
}

void SM_ParticleSystem_Destroy(SM_ParticleSystem *this)
{
     SM_DynamicArray_Destroy(this->particle);
     free(this);
}

size_t SM_ParticleSystem_GetParticleCount(const SM_ParticleSystem *this)
{
     return SM_DynamicArray_GetSize(this->particle_dynamic_array);
}

void SM_ParticleSystem_Emit(SM_ParticleSystem *this, SM_ParticleSystemParticle *particle)
{
     SM_DynamicArray_PushBack(this->particle_dynamic_array, particle);
}

void SM_ParticleSystem_Simulate(SM_ParticleSystem *this, SimulateInfo *sInfo)
{
     // SM_DynamicArray_Traverse(this->particle_dynamic_array, _traverse_particle, sInfo);
}

void SM_ParticleSystem_Render(SM_ParticleSystem *this)
{
     
}

static void _traverse_particle(void *data, void *uData)
{
     SM_ParticleSystemParticle *particle = (Particle *)data;
     float dt = ((SimulateInfo *)uData)->dt;
     particle->position.x += particle->velocity.x * dt;
     particle->position.y += particle->velocity.y * dt;
     particle->position.z += particle->velocity.z * dt;
}
