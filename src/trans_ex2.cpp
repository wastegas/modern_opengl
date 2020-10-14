/* demonstrate transformation */

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

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	  // positions      colors            texture coords
	  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
	  0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
	  -0.5f,-0.5f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	  -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,1.0f, 0.0f, 1.0f// top left
  };

  unsigned int indices[] = {
			    0, 1, 3, // first triangle
			    1, 2, 3  // second triangle
  };
  
  unsigned int VBO;  // vertex buffer object
  glGenBuffers(1, &VBO); // generate with buffer id 1
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
	       vertices, GL_STATIC_DRAW); // copy our array into buffer


  Shader ourShader("./trans1.vs", "./trans1.fs");

  // vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // positions attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);

  // colour attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // element buffer object
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	       GL_STATIC_DRAW);

  glViewport(0, 0, 800, 600);


  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);





  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      
      // assign the texture to the fragment shader's sampler
      
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);


      glm::mat4 trans = glm::mat4(1.0f); // create identity matrix
      trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
      trans = glm::rotate(trans, (float)glfwGetTime(),
		      glm::vec3(0.0, 0.0, 1.0)); // rotate on z axis
      unsigned int transfromLoc = glGetUniformLocation(ourShader.ID,
						       "transform");
      glUniformMatrix4fv(transfromLoc, 1, GL_FALSE, glm::value_ptr(trans));
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      
      // second container
      trans = glm::mat4(1.0f); // reset back to identity
      trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
      float scaleValue = sin(glfwGetTime());
      trans = glm::scale(trans, glm::vec3(scaleValue, scaleValue, scaleValue));
      transfromLoc = glGetUniformLocation(ourShader.ID,
						       "transform");
      glUniformMatrix4fv(transfromLoc, 1, GL_FALSE, glm::value_ptr(trans));

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      
      
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