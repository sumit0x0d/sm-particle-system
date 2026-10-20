#include <stdio.h>

#define GLFW_INCLUDE_NONE

#include "../third-party/glad/glad.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <sm-particle-system.h>

int main(void)
{
   GLFWwindow *window;
   SM_Particle_System *pSystem;
   SM_Particle_System_Particle psBody;
   double previous_time;

   glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
   if (!glfwInit()) {
      return -1;
   }

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

   window = glfwCreateWindow(640, 480, "Particle System", NULL, NULL);
   if (!window) {
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);
   if (!gladLoadGL()) {
      fprintf(stderr, "Failed to load OpenGL functions\n");
      glfwDestroyWindow(window);
      glfwTerminate();
      return -1;
   }

   psBody.position.x = 320;
   psBody.position.y = 240;
   psBody.position.z = 0;
   psBody.velocity.x = 20;
   psBody.velocity.y = 15;
   psBody.velocity.z = 0;
   psBody.color.a = 255;
   psBody.color.r = 255;
   psBody.color.g = 80;
   psBody.color.b = 20;
   psBody.alive = SM_BOOLEAN_TRUE;

   pSystem = sm_particle_system_create(SM_PARTICLE_SYSTEM_RENDERER_OPENGL, 1000);
   if (!pSystem) {
      fprintf(stderr, "Failed to create particle system\n");
      glfwDestroyWindow(window);
      glfwTerminate();
      return -1;
   }

   sm_particle_system_emit(pSystem, &psBody);
   previous_time = glfwGetTime();

   while (!glfwWindowShouldClose(window)) {
      double current_time = glfwGetTime();
      SM_Float dt = (SM_Float)(current_time - previous_time);
      previous_time = current_time;

      sm_particle_system_simulate(pSystem, dt);
      glClear(GL_COLOR_BUFFER_BIT);
      sm_particle_system_render(pSystem);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   sm_particle_system_destroy(pSystem);
   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}
