#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <sm-particle-system.h>

int main(void)
{
     srand(time(NULL));
     GLFWwindow* window;

     if (!glfwInit()) {
          return -1;
     }

     SM_ParticleSystemParticle psBody = {
          .position = {rand() % 640, rand() % 480, 0},
          .velocity = {rand() % 640, rand() % 640, 0},
          .color = {rand() % 255, rand() % 255, rand() % 255},
          .alive = 1
     };

     // ParticleSystemCreateInfo pscInfo = {
     //      .particle = psBody,
     //      .particle_count = 1000
     // };

     SM_ParticleSystem *pSystem = SM_ParticleSystem_Create();
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
     glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
     glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

     window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
     if (!window) {
          glfwTerminate();
          return -1;
     }

     glfwMakeContextCurrent(window);
     gladLoadGL();

     while (!glfwWindowShouldClose(window)) {
          glClear(GL_COLOR_BUFFER_BIT);
          glfwSwapBuffers(window);
          glfwPollEvents();
     }

     glfwDestroyWindow(window);
     glfwTerminate();

     return 0;
}
