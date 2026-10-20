#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad.h>

#include "sm-particle-system-opengl.h"

#define VERTEX_SHADER_PATH "/home/sumit/Projects/sm-particle-system/source/shaders/vertex-shader.glsl"
#define FRAGMENT_SHADER_PATH "/home/sumit/Projects/sm-particle-system/source/shaders/fragment-shader.glsl"

struct SM_Particle_System_OpenGL {
   GLuint vertex_array_object;
   GLuint vertex_buffer_object;
   GLuint program;
};

static char *_load_shader_source(const char *filename)
{
   FILE *file;
   long source_size;
   char *source;
   size_t bytes_read;

   file = fopen(filename, "rb");

   if (!file) {
      fprintf(stderr, "Particle renderer: failed to open shader file: %s\n", filename);
      return NULL;
   }

   if (fseek(file, 0, SEEK_END) != 0 || (source_size = ftell(file)) < 0 || fseek(file, 0, SEEK_SET) != 0) {
      fprintf(stderr, "Particle renderer: failed to read shader file: %s\n", filename);
      fclose(file);
      return NULL;
   }

   source = (char *)malloc((size_t)source_size + 1);

   if (!source) {
      fprintf(stderr, "Particle renderer: failed to allocate shader source\n");
      fclose(file);
      return NULL;
   }

   bytes_read = fread(source, 1, (size_t)source_size, file);

   if (bytes_read != (size_t)source_size || ferror(file)) {
      fprintf(stderr, "Particle renderer: failed to read shader file: %s\n", filename);
      free(source);
      fclose(file);
      return NULL;
   }

   source[source_size] = '\0';
   fclose(file);

   return source;
}

static GLuint _compile_shader(GLenum type, const char *filename)
{
   char *source;
   const GLchar *shader_source;
   GLuint shader;
   GLint compiled;

   source = _load_shader_source(filename);
   
   if (!source) {
      return 0;
   }

   shader = glCreateShader(type);
   
   if (!shader) {
      fprintf(stderr, "Particle renderer: failed to create shader\n");
      free(source);
      return 0;
   }

   shader_source = source;
   glShaderSource(shader, 1, &shader_source, NULL);
   glCompileShader(shader);
   free(source);
   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

   if (!compiled) {
      GLchar log[1024];
      GLsizei log_length = 0;
      glGetShaderInfoLog(shader, sizeof(log), &log_length, log);
      fprintf(stderr, "Particle renderer: shader compilation failed: %.*s\n", (int)log_length, log);
      glDeleteShader(shader);
      return 0;
   }

   return shader;
}

static GLuint _create_program(void)
{
   GLuint vertex_shader;
   GLuint fragment_shader;
   GLuint program;
   GLint linked;

   vertex_shader = _compile_shader(GL_VERTEX_SHADER, VERTEX_SHADER_PATH);

   if (!vertex_shader) {
      return 0;
   }

   fragment_shader = _compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);

   if (!fragment_shader) {
      glDeleteShader(vertex_shader);
      return 0;
   }

   program = glCreateProgram();

   if (!program) {
      fprintf(stderr, "Particle renderer: failed to create shader program\n");
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      return 0;
   }

   glAttachShader(program, vertex_shader);
   glAttachShader(program, fragment_shader);
   glLinkProgram(program);
   glGetProgramiv(program, GL_LINK_STATUS, &linked);
   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);

   if (!linked) {
      GLchar log[1024];
      GLsizei log_length = 0;
      glGetProgramInfoLog(program, sizeof(log), &log_length, log);
      fprintf(stderr, "Particle renderer: shader linking failed: %.*s\n", (int)log_length, log);
      glDeleteProgram(program);
      return 0;
   }

   return program;
}

SM_Particle_System_OpenGL *sm_particle_system_opengl_create(void)
{


   return (SM_Particle_System_OpenGL *)calloc(1, sizeof(SM_Particle_System_OpenGL));
}

void sm_particle_system_opengl_destroy(SM_Particle_System_OpenGL *this)
{
   if (this->vertex_array_object && glad_glDeleteVertexArrays) {
      glDeleteVertexArrays(1, &this->vertex_array_object);
   }

   if (this->vertex_buffer_object && glad_glDeleteBuffers) {
      glDeleteBuffers(1, &this->vertex_buffer_object);
   }

   if (this->program && glad_glDeleteProgram) {
      glDeleteProgram(this->program);
   }

   free(this);
}

void sm_particle_system_opengl_render(SM_Particle_System_OpenGL *this, const float *vertex_data, size_t particle_count)
{
   if (!this || !vertex_data || particle_count == 0) {
      return;
   }

   if (!glad_glCreateShader || !glad_glShaderSource || !glad_glCompileShader ||
      !glad_glGetShaderiv || !glad_glGetShaderInfoLog || !glad_glDeleteShader ||
      !glad_glCreateProgram || !glad_glAttachShader || !glad_glLinkProgram ||
      !glad_glGetProgramiv || !glad_glGetProgramInfoLog || !glad_glDeleteProgram ||
      !glad_glGenVertexArrays || !glad_glGenBuffers || !glad_glBindVertexArray ||
      !glad_glBindBuffer || !glad_glBufferData || !glad_glVertexAttribPointer ||
      !glad_glEnableVertexAttribArray || !glad_glEnable || !glad_glUseProgram ||
      !glad_glDrawArrays) {
      fprintf(stderr, "Particle renderer: OpenGL functions are not loaded\n");
      return;
   }

   this->program = _create_program();

   if (!this->program) {
      return;
   }

   glGenVertexArrays(1, &this->vertex_array_object);
   glGenBuffers(1, &this->vertex_buffer_object);
   
   if (!this->vertex_array_object || !this->vertex_buffer_object) {
      fprintf(stderr, "Particle renderer: failed to create OpenGL buffers\n");
      if (this->vertex_array_object && glad_glDeleteVertexArrays) {
         glDeleteVertexArrays(1, &this->vertex_array_object);
      }
      if (this->vertex_buffer_object && glad_glDeleteBuffers) {
         glDeleteBuffers(1, &this->vertex_buffer_object);
      }
      glDeleteProgram(this->program);
      this->vertex_array_object = 0;
      this->vertex_buffer_object = 0;
      this->program = 0;
      return;
   }

   glBindVertexArray(this->vertex_array_object);
   glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_object);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   glUseProgram(this->program);
   glBindVertexArray(this->vertex_array_object);
   glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_object);
   glEnable(GL_PROGRAM_POINT_SIZE);
   glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(particle_count * 6 * sizeof(*vertex_data)), vertex_data, GL_STREAM_DRAW);
   glDrawArrays(GL_POINTS, 0, (GLsizei)particle_count);
   glBindVertexArray(0);
   glUseProgram(0);
}