#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <shader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
					"LearnOpenGL", NULL, NULL);
  if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

  glEnable(GL_DEPTH_TEST);

  Shader shader("geometry.vs", "geometry.fs", "geometry.gs");
  
  // points for our triangle
  float points[] =
    {
     -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top left
     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bot right
     -0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bot left
  };

  unsigned int VBO;  // vertex buffer object
  glGenBuffers(1, &VBO); // generate with buffer id 1
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(points),
	       points, GL_STATIC_DRAW); // copy our array into buffer


  // vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glEnableVertexAttribArray(0);  
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)(2 * sizeof(float)));
  glBindVertexArray(0);

  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.use();
      glBindVertexArray(VAO);
      glDrawArrays(GL_POINTS, 0, 4);
      
      processInput(window);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, true);
    }
}
