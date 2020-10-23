/* demonstrate perspective */

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

// screen size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

  unsigned int VBO;  // vertex buffer object
  glGenBuffers(1, &VBO); // generate with buffer id 1
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
	       vertices, GL_STATIC_DRAW); // copy our array into buffer


  Shader ourShader("./coord2.vs", "./coord2.fs");

  // vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // positions attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);


  glViewport(0, 0, 800, 600);

  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      // also clear the depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
      // rotate backwards around x-axis
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
			  glm::vec3(0.5f, 1.0f, 0.0f));
      // move camera backwards on z-axis (negative towards the front)
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      // standard setting for projection
      projection = glm::perspective(glm::radians(45.0f),
				    (float)SCR_WIDTH / (float) SCR_HEIGHT,
				    0.1f, 100.0f);
      // retrieve uniform location from shader
      unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
      unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
      unsigned int projectionLoc = glGetUniformLocation(ourShader.ID,
							"projection");
      // pass them to the shaders (3 different ways all the same)
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      // note this can be set outside of the rendering
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      
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
