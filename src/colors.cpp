/* demonstrate perspective */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0; // half of width of window
float lastY = SCR_HEIGHT / 2.0; // half of height of window

// frame delta time
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

  // configure global opengl state
  glEnable(GL_DEPTH_TEST);
  
  Shader lightingShader("./colors.vs", "./colors.fs");
  Shader lightCubeShader("./lighting1.vs", "./lighting1.fs");
  
  float vertices[] = {
	  // positions      
	  -0.5f, -0.5f, -0.5f, 
	   0.5f, -0.5f, -0.5f, 
	   0.5f,  0.5f, -0.5f,
	   0.5f,  0.5f, -0.5f,
	  -0.5f,  0.5f, -0.5f,
	  -0.5f, -0.5f, -0.5f,

	  -0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f,
	   0.5f,  0.5f,  0.5f,
	   0.5f,  0.5f,  0.5f,
	  -0.5f,  0.5f,  0.5f,
	  -0.5f, -0.5f,  0.5f,

	  -0.5f,  0.5f,  0.5f,
	  -0.5f,  0.5f, -0.5f,
	  -0.5f, -0.5f, -0.5f,
	  -0.5f, -0.5f, -0.5f,
	  -0.5f, -0.5f,  0.5f,
	  -0.5f,  0.5f,  0.5f,

	   0.5f,  0.5f,  0.5f,
	   0.5f,  0.5f, -0.5f,
	   0.5f, -0.5f, -0.5f,
	   0.5f, -0.5f, -0.5f,
	   0.5f, -0.5f,  0.5f,
	   0.5f,  0.5f,  0.5f,

	  -0.5f, -0.5f, -0.5f,
	   0.5f, -0.5f, -0.5f,
	   0.5f, -0.5f,  0.5f,
	   0.5f, -0.5f,  0.5f,
	  -0.5f, -0.5f,  0.5f,
	  -0.5f, -0.5f, -0.5f,

	  -0.5f,  0.5f, -0.5f,
	   0.5f,  0.5f, -0.5f,
	   0.5f,  0.5f,  0.5f,
	   0.5f,  0.5f,  0.5f,
	  -0.5f,  0.5f,  0.5f,
	  -0.5f,  0.5f, -0.5f

  };

  // first configure the cube's VAO and VBO
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
	       vertices, GL_STATIC_DRAW); 

  glBindVertexArray(cubeVAO);
    // positions attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);

  // second configure the light's VAO (VBO stays the same; the
  // vertices are the same for the light object which is also
  // a 3D cube
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // we only need to bind the vbo (to link it with glVertexAttribPointer),
  // no need to fill it; the VBO's data already contains all we need
  // it's already bound, but we do it again for educational purposes
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  
  
  while(!glfwWindowShouldClose(window))
    {
      //per-frame time logic
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // be sure to activate shader when setting uniforms/drawign objects
      lightingShader.use();
      // set our colors
      lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
      lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

      // view projection transformations
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
		    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      glm::mat4 view = camera.GetViewMatrix();
      lightingShader.setMat4("projection", projection);
      lightingShader.setMat4("view", view);

      // world transformation
      glm::mat4 model = glm::mat4(1.0f);
      lightingShader.setMat4("model", model);
      
      // render the cube
      glBindVertexArray(cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // also draw the lamp object
      lightCubeShader.use();
      lightCubeShader.setMat4("projection", projection);
      lightCubeShader.setMat4("view", view);
      model = glm::mat4(1.0f);
      model = glm::translate(model, lightPos); // move our lamp to lightPOS
      model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
      lightCubeShader.setMat4("model", model);

      glBindVertexArray(lightCubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      
      processInput(window);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  // deallocate all resources (optional)
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);
  glDeleteBuffers(1, &VBO);

  // terminate clearing all previously allocated GLFW resources
  glfwTerminate();
  
  
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  const float cameraSpeed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(FORWARD, deltaTime);
    }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(LEFT, deltaTime);
    }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(RIGHT, deltaTime);
    }
				  
}

// glfw: whenever the mouse mves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coord go from bottom to top
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

