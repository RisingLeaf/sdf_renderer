#ifndef PTREE_WINDOW_H
#define PTREE_WINDOW_H

#include <GL/glew.h>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <string>



class Window
{
  int width, height;
  const char* name;

  GLFWwindow *window;

  bool shouldClose = false;

public:
  Window(int width, int height, const char *name);
  ~Window();

  bool InitGL();
  void Destroy();
  void SwapBuffers();

  int GetWindowWidth() const { return width; }
  int GetWindowHeight() const { return height; }
  static std::string GetKeyName(int key);

  void Close() { shouldClose = true; };
  bool ShouldClose() { return glfwWindowShouldClose(window) || shouldClose; }

  void FreeCursor();
  void HideCursor();
};


#endif //PTREE_WINDOW_H