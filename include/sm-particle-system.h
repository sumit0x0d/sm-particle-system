#ifndef SM_PARTICLE_H
#define SM_PARTICLE_H

#include <stddef.h>

#include <sm-api.h>
#include <sm-typedef.h>

typedef struct SM_ParticleSystem SM_ParticleSystem;
typedef struct SM_ParticleSystemParticle SM_ParticleSystemParticle;
typedef struct SM_ParticleSystemParticlePosition SM_ParticleSystemParticlePosition;
typedef struct SM_ParticleSystemParticleVelocity SM_ParticleSystemParticleVelocity;
typedef struct SM_ParticleSystemParticleColor SM_ParticleSystemParticleColor;

struct SM_ParticleSystemParticlePosition {
     SM_Float x;
     SM_Float y;
     SM_Float z;
};

struct SM_ParticleSystemParticleVelocity {
     SM_Float x;
     SM_Float y;
     SM_Float z;
};

struct SM_ParticleSystemParticleColor {
     SM_Float a;
     SM_Float r;
     SM_Float g;
     SM_Float b;
};

struct SM_ParticleSystemParticle {
     SM_ParticleSystemParticlePosition position;
     SM_ParticleSystemParticlePosition velocity;
     SM_ParticleSystemParticlePosition color;
     SM_Bool alive;
};

SM_API SM_ParticleSystem *SM_ParticleSystem_Create(void);
SM_API void SM_ParticleSystem_Destroy(SM_ParticleSystem *this);

SM_API size_t SM_ParticleSystem_GetParticleCount(const SM_ParticleSystem *this);

SM_API void SM_ParticleSystem_Emit(SM_ParticleSystem *this, SM_ParticleSystemParticle *particle);
SM_API void SM_ParticleSystem_Simulate(SM_ParticleSystem *this, SM_Float dt);
SM_API void SM_ParticleSystem_Render(SM_ParticleSystem *this);

#endif
