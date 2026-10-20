#include <stdio.h>
#include <time.h>

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
   size_t i;

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

   pSystem = sm_particle_system_create(SM_PARTICLE_SYSTEM_RENDERER_OPENGL, 1000);
   if (!pSystem) {
      fprintf(stderr, "Failed to create particle system\n");
      glfwDestroyWindow(window);
      glfwTerminate();
      return -1;
   }
   for (i = 0; i < 100; i++) {
      psBody.position.x = rand() % 640;
      psBody.position.y = rand() % 480;
      psBody.position.z = 0;
      psBody.velocity.x = rand() % 20 - 10;
      psBody.velocity.y = rand() % 20 - 10;
      psBody.velocity.z = 0;
      psBody.color.a = rand() % 256;
      psBody.color.r = rand() % 256;
      psBody.color.g = rand() % 256;
      psBody.color.b = rand() % 256;
      psBody.alive = SM_BOOLEAN_TRUE;
      sm_particle_system_emit(pSystem, &psBody);
   }

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
