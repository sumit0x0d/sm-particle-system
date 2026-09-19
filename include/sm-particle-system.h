#ifndef SM_PARTICLE_H
#define SM_PARTICLE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct SM_ParticleSystem SM_ParticleSystem;
typedef struct SM_ParticleSystemParticle SM_ParticleSystemParticle;
typedef struct SM_ParticleSystemParticlePosition SM_ParticleSystemParticlePosition;
typedef struct SM_ParticleSystemParticleVelocity SM_ParticleSystemParticleVelocity;
typedef struct SM_ParticleSystemParticleColor SM_ParticleSystemParticleColor;

struct SM_ParticleSystemParticlePosition {
     float x;
     float y;
     float z;
};

struct SM_ParticleSystemParticleVelocity {
     float x;
     float y;
     float z;
};

struct SM_ParticleSystemParticleColor {
     float a;
     float r;
     float g;
     float b;
};

struct SM_Particle {
     SM_ParticleSystemParticlePosition position;
     SM_ParticleSystemParticlePosition velocity;
     SM_ParticleSystemParticlePosition color;
     bool alive;
};

SM_ParticleSystem *SM_ParticleSystem_Create(void);
void SM_ParticleSystem_Destroy(SM_ParticleSystem *this);

size_t SM_ParticleSystem_GetParticleCount();

void SM_ParticleSystem_Emit(SM_ParticleSystem *this, SM_Particle *particle);
void SM_ParticleSystem_Simulate(SM_ParticleSystem *this, float dt);
void SM_ParticleSystem_Render(SM_ParticleSystem *this);

#endif
