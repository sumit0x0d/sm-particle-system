#ifndef SM_PARTICLE_SYSTEM_H
#define SM_PARTICLE_SYSTEM_H

#include <stddef.h>

#include <sm-api.h>
#include <sm-typedef.h>

typedef struct SM_Particle_System SM_Particle_System;
typedef struct SM_Particle_System_Particle SM_Particle_System_Particle;
typedef struct SM_Particle_System_Particle_Position SM_Particle_System_Particle_Position;
typedef struct SM_Particle_System_Particle_Velocity SM_Particle_System_Particle_Velocity;
typedef struct SM_Particle_System_Particle_Color SM_Particle_System_Particle_Color;
typedef enum SM_Particle_System_Renderer_Type SM_Particle_System_Renderer_Type;

struct SM_Particle_System_Particle_Position {
   SM_Float x;
   SM_Float y;
   SM_Float z;
};

struct SM_Particle_System_Particle_Velocity {
   SM_Float x;
   SM_Float y;
   SM_Float z;
};

struct SM_Particle_System_Particle_Color {
   SM_Float a;
   SM_Float r;
   SM_Float g;
   SM_Float b;
};

enum SM_Particle_System_Renderer_Type {
   SM_PARTICLE_SYSTEM_RENDERER_OPENGL,
   SM_PARTICLE_SYSTEM_RENDERER_VULKAN,
};

struct SM_Particle_System_Particle {
   SM_Particle_System_Particle_Position position;
   SM_Particle_System_Particle_Velocity velocity;
   SM_Particle_System_Particle_Color color;
   SM_Boolean alive;
};

SM_API SM_Particle_System *sm_particle_system_create(SM_Particle_System_Renderer_Type renderer_type, size_t particle_capacity);

SM_API void sm_particle_system_destroy(SM_Particle_System *this);

SM_API size_t sm_particle_system_get_particle_count(const SM_Particle_System *this);

SM_API void sm_particle_system_emit(SM_Particle_System *this, SM_Particle_System_Particle *particle);

SM_API void sm_particle_system_simulate(SM_Particle_System *this, SM_Float dt);

SM_API void sm_particle_system_render(SM_Particle_System *this);

#endif
