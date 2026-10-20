#ifndef SM_PARTICLE_SYSTEM_OPENGL_H
#define SM_PARTICLE_SYSTEM_OPENGL_H

#include <stddef.h>

typedef struct SM_Particle_System_OpenGL SM_Particle_System_OpenGL;

SM_Particle_System_OpenGL *sm_particle_system_opengl_create(void);

void sm_particle_system_opengl_destroy(SM_Particle_System_OpenGL *this);

void sm_particle_system_opengl_render(SM_Particle_System_OpenGL *this, const float *vertex_data, size_t particle_count);

#endif