#include "Window.h"

#include "Events.h"
#include "Log.h"

#include <string>



namespace
{
  void error_callback(int error, const char* description)
  {
    Log::Error("glfw error_callback: " + std::string(description));
  }

  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    EventHandler::Event event;

    switch(action)
    {
    case GLFW_PRESS:
      event.type = EventHandler::EventType::KEYDOWN;
      break;
    case GLFW_RELEASE:
      event.type = EventHandler::EventType::KEYUP;
      break;
    case GLFW_REPEAT:
    default:
      event.type = EventHandler::EventType::NONE;
      break;
    }

    event.key = key;
    event.mods = mods;
    
    EventHandler::AddEvent(event);
  }

  int window_width, window_height;

  void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
  {
    EventHandler::Event event;
    
    glfwGetWindowSize(window, &window_width, &window_height);
    
    event.type = EventHandler::EventType::MOUSE_MOVE;
    event.x = ((xpos / window_width) * 2.) - 1.;
    event.y = (((ypos / window_height) * 2.) - 1.) * -1.;

    EventHandler::AddEvent(event);
  }

  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    EventHandler::Event event;
    switch(action)
    {
    case GLFW_PRESS:
      event.type = EventHandler::EventType::MOUSE_BUTTONDOWN;
      break;
    case GLFW_RELEASE:
      event.type = EventHandler::EventType::MOUSE_BUTTONUP;
      break;
    default:
      event.type = EventHandler::EventType::NONE;
      break;
    }

    glfwGetWindowSize(window, &window_width, &window_height);
  
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    event.x = ((xpos / window_width) * 2.) - 1.;
    event.y = (((ypos / window_height) * 2.) - 1.) * -1.;

    EventHandler::AddEvent(event);
  }
};



Window::Window(int width, int height, const char * name)
: width{ width }, height{ height }, name{ name }
{
  Log::Status("Window init: name=\"" + std::string(name) + "\" width=" +std::to_string(width) + " height=" + std::to_string(height));
  
  if(!glfwInit())
    Log::Error("GLFW failed on glfwInit");

  glfwSetErrorCallback(error_callback);

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_SAMPLES, 4);

  window = glfwCreateWindow(width, height, name, NULL, NULL);
  if(!window)
  {
    glfwTerminate();
    Log::Error("GLFW failed on glfwCreateWindow");
  }
}

Window::~Window()
{
  Destroy();
}

bool Window::InitGL()
{
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewInit();

  glfwSetKeyCallback(window, key_callback);

glfwSetCursorPosCallback(window, cursor_pos_callback);
glfwSetMouseButtonCallback(window, mouse_button_callback);

  const unsigned char *versionchar = glGetString(GL_VERSION);
  if(versionchar)
  {
    std::string version(reinterpret_cast<const char*>(versionchar));
    Log::Status("Window created; OpenGL Version: " + version);
  }
  else
    Log::Warning("Window was unable to obtain OpenGL Version.");
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  return true;
}

void Window::Destroy()
{
  glfwDestroyWindow(window);
}

void Window::SwapBuffers()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
  glfwGetFramebufferSize(window, &width, &height);
}

std::string Window::GetKeyName(int key)
{
  const char *name = glfwGetKeyName(key, 0);
  return name ? name : "no-name-defined";
}

void Window::FreeCursor()
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::HideCursor()
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
