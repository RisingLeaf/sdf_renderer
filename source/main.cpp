#include "Events.h"
#include "Shader.h"
#include "Window.h"
#include "source/Log.h"

#include <GLFW/glfw3.h>
#include <ctime>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>



glm::mat3 LookAt(glm::vec3 forwardVec, glm::vec3 upVec)
{
  glm::vec3 Z = glm::normalize(forwardVec);
  glm::vec3 X = glm::normalize(glm::cross(forwardVec, upVec));
  glm::vec3 Y = glm::normalize(glm::cross(X, forwardVec));

  return glm::mat3( X, Y, Z );
};



int main(int argc, const char **argv)
{
  Window window(1000, 1000, "SDF Render");
  window.InitGL();

  Shader shader("../source/shaders/sdf.vert", "../source/shaders/sdf.frag");
  shader.LoadUniformLocation("u_time");
  shader.LoadUniformLocation("origin");
  shader.LoadUniformLocation("rotMat");

  std::vector<float> vertices = {
    -1., -1.,
    -1.,  1.,
     1., -1.,

     1.,  1.,
     1., -1.,
    -1.,  1.
  };
  unsigned VAO, VBO;
  glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  GLint posAttrib = glGetAttribLocation(shader.ShaderId(), "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  


  double first = 0.;
  double second = 0.;
  double delta = 0.;
  double u_time = 1.;
  int f = 0;
  int s = 0;
  int h = 0;
  glm::vec3 up(0.0f, 1.0f, 0.0f);
  glm::vec3 origin(2.5, 5., 0.);
  glm::mat3 rotMat(1.);
  glm::vec3 forwardDirection(1., 0., 0.);
  bool cameraMoving = false;
  float yaw = 0.;
  float pitch = 0.;
  float lastX = 0.;
  float lastY = 0.;
  while(!window.ShouldClose())
  {
    delta = (second - first) / CLOCKS_PER_SEC;
    first = std::clock();


    std::vector<EventHandler::Event> &events = EventHandler::RetrieveEvents();
    for(const auto &event : events)
    {
      if(event.type == EventHandler::EventType::KEYDOWN || event.type == EventHandler::EventType::KEYUP)
      {
        int m = event.type == EventHandler::EventType::KEYDOWN ? 1 : -1;
        switch(event.key)
        {
          case GLFW_KEY_W:
            f += m;
            break;
          case GLFW_KEY_S:
            f -= m;
            break;
          case GLFW_KEY_A:
            s -= m;
            break;
          case GLFW_KEY_D:
            s += m;
            break;
          case GLFW_KEY_SPACE:
            h += m;
            break;
          case GLFW_KEY_LEFT_SHIFT:
            h -= m;
            break;
          default:
            break;
        }
      }
      else if(event.type == EventHandler::EventType::MOUSE_BUTTONDOWN)
      {
        lastX = event.x;
        lastY = event.y;
        cameraMoving = true;
      }
      else if(event.type == EventHandler::EventType::MOUSE_BUTTONUP)
        cameraMoving = false;
      else if(event.type == EventHandler::EventType::MOUSE_MOVE && cameraMoving)
      {
        yaw   -= lastX - event.x;
        pitch -= lastY - event.y;
        lastX = event.x;
        lastY = event.y;

        forwardDirection = glm::normalize(glm::vec3(
          cos(yaw) * cos(pitch),
          sin(pitch),
          sin(yaw) * cos(pitch)
        ));
      }
    }

    const int SPEED = 20;
    if(f != 0.)
      origin += glm::normalize(forwardDirection) * static_cast<float>(delta * SPEED * f);
    if(h != 0.)
      origin += up * static_cast<float>(delta * SPEED * h);
    if(s != 0)
      origin += glm::normalize(glm::cross(forwardDirection, up)) * static_cast<float>(delta * SPEED * s);

    u_time += delta * 1000;


    // DRAW

    // Construct the view rotation matrix
    rotMat = LookAt(forwardDirection, up);

    glClear(GL_COLOR_BUFFER_BIT);
    shader.bind();

    glBindVertexArray(VAO);
	  glBindBuffer(GL_ARRAY_BUFFER, VBO);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glUniform1f(shader.UniformLocation("u_time"), u_time);

    glUniform3fv(shader.UniformLocation("origin"), 1, glm::value_ptr(origin));
    glUniformMatrix3fv(shader.UniformLocation("rotMat"), 1, GL_FALSE, glm::value_ptr(rotMat));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

    window.SwapBuffers();
    second = std::clock();
    Log::Status(std::to_string(1/delta) + glm::to_string(origin));
  }
  return 0;
}