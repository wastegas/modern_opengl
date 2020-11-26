/* demonstrate camera walk around world with mouse inputs */
/* add zooming using mouse scroll */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera positions
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


// initialize values for mouse input handler
bool firstMouse = true;
float yaw = -90.f;
// initialized to -90.0 deg since a yaw of 0.0 results a direction
// vectore pointing to the right so we rotate it a bit to the left
float pitch = 0.0f;
float lastX = 800.0f / 2.0; // half of width of window
float lastY = 800.0f / 2.0; // half of height of window
// field of view for zoom
float fov = 45.0f;

// frame delta time
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

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

  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("container.jpg", &width, &height,
				  &nrChannels, 0);
  if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
	       GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  else
    {
      std::cout << "Failed to load texture" << std::endl;
    }

  // free the image after generating
  stbi_image_free(data);

  // Texture 2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("awesomeface.png", &width, &height,
				  &nrChannels, 0);
  if (data)
    {
      // .png has transparancy thus an alpha channel GL_RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
	       GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  else
    {
      std::cout << "Failed to load texture" << std::endl;
    }

  // free the image after generating
  stbi_image_free(data);
  
  
  // points for our rectangle created with two triangles
  float vertices[] = {
	  // positions      texture coords
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 
	   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
           0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f


  };

  // translaction vectors for each cube
  glm::vec3 cubePositions[] = {
			       glm::vec3(0.0f, 0.0f, 0.0f),
			       glm::vec3(2.0f, 5.0f, 15.0f),
			       glm::vec3(-1.5f, -2.2f, -2.5f),
			       glm::vec3(-3.8f, -2.0f, -12.3f),
			       glm::vec3(2.4f, -0.4f, -3.5f),
			       glm::vec3(-1.7f, 3.0f, -7.5f),
			       glm::vec3(1.3f, -2.0f, -2.5f),
			       glm::vec3(1.5f, 2.0f, -2.5f),
			       glm::vec3(1.5f, 0.2f, -1.5f),
			       glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  unsigned int VBO;  // vertex buffer object
  glGenBuffers(1, &VBO); // generate with buffer id 1
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
	       vertices, GL_STATIC_DRAW); // copy our array into buffer


  Shader ourShader("./coord2.vs", "./coord2.fs", nullptr);

  // vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // positions attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);

  // texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glViewport(0, 0, 800, 600);

  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      // also clear the depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      
      // assign the texture to the fragment shader's sampler
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
      
      
      
      ourShader.use();

      // create transformations
      glm::mat4 model = glm::mat4(1.0f); // initialize to identity
      glm::mat4 view = glm::mat4(1.0f);
      glm::mat4 projection = glm::mat4(1.0f);
      // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      // standard setting for projection
      projection = glm::perspective(glm::radians(fov),
				    (float)SCR_WIDTH / (float) SCR_HEIGHT,
				    0.1f, 100.0f);
      // retrieve uniform location from shader
      unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
      unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
      unsigned int projectionLoc = glGetUniformLocation(ourShader.ID,
							"projection");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

      glBindVertexArray(VAO);
      for (unsigned int i = 0; i < 10; i++)
	{
	  model = glm::mat4(1.0f);
	  model = glm::translate(model, cubePositions[i]);
	  float angle = 20.f * i;
	  model = glm::rotate(model, glm::radians(angle),
			      glm::vec3(1.0f, 0.3f, 0.5f));
	  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	  glDrawArrays(GL_TRIANGLES, 0, 36);
	}
      
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
  const float cameraSpeed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, true);
    }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      cameraPos += cameraSpeed * cameraFront;
    }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      cameraPos -= cameraSpeed * cameraFront;
    }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))
	* cameraSpeed;
    }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))
	* cameraSpeed;
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

  float sensitivity = 0.1f; // adjust to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // make sure that when the pitch is out of bounds, screen doesn't get flipped
  if (pitch >= 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  fov -= (float)yoffset;
  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;
}
