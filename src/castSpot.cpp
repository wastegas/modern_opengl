/* demonstrate materials */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow* window,GLdouble xoffset, GLdouble yoffset);
GLuint loadTexture(const char* path);

// screen size
const GLuint SCR_WIDTH = 1024;
const GLint SCR_HEIGHT = 768;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLboolean firstMouse = true;
GLfloat lastX = SCR_WIDTH / 2.0; // half of width of window
GLfloat lastY = SCR_HEIGHT / 2.0; // half of height of window

// frame delta time
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

// light position
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
  
  Shader lightingShader("./map_point.vs",
			"./map_point.fs");
  Shader lightCubeShader("./lightCube_point.vs",
			 "./lightCube_point.fs");
  
  GLfloat vertices[] = {
	  // positions         // normals        // coords      
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f

  };
  // translaction vectors for each cube
  glm::vec3 cubePositions[] = {
			       glm::vec3(0.0f, 0.0f, 0.0f),
			       glm::vec3(2.0f, 5.0f, -15.0f),
			       glm::vec3(-1.5f, -2.2f, -2.5f),
			       glm::vec3(-3.8f, -2.0f, -12.3f),
			       glm::vec3(2.4f, -0.4f, -3.5f),
			       glm::vec3(-1.7f, 3.0f, -7.5f),
			       glm::vec3(1.3f, -2.0f, -2.5f),
			       glm::vec3(1.5f, 2.0f, -2.5f),
			       glm::vec3(1.5f, 0.2f, -1.5f),
			       glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  // first configure the cube's VAO and VBO
  GLuint VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
	       vertices, GL_STATIC_DRAW); 

  glBindVertexArray(cubeVAO);
    // positions attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)0);
  glEnableVertexAttribArray(0);
  //  normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  // coords attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  // second configure the light's VAO (VBO stays the same; the
  // vertices are the same for the light object which is also
  // a 3D cube
  GLuint lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // we only need to bind the vbo (to link it with glVertexAttribPointer),
  // no need to fill it; the VBO's data already contains all we need
  // it's already bound, but we do it again for educational purposes
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)0);
  glEnableVertexAttribArray(0);
  
  // load textures
  GLuint diffuseMap = loadTexture("container2.png");
  GLuint specularMap = loadTexture("container2_specular.png");

  // shader configuration
  lightingShader.use();
  lightingShader.setInt("material.diffuse", 0);
  lightingShader.setInt("material.specular", 1);


  
  while(!glfwWindowShouldClose(window))
    {
      //per-frame time logic
      GLfloat currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // be sure to activate shader when setting uniforms/drawign objects
      lightingShader.use();
      lightingShader.setVec3("light.position", lightPos);
      lightingShader.setVec3("viewPos", camera.Position);

      // material properties
      lightingShader.setFloat("material.shininess", 64.0f);

      // set the intensities for light properties
      lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
      lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
      lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

      // attentuation constants
      lightingShader.setFloat("light.constant", 1.0f);
      lightingShader.setFloat("light.linear", 0.09f);
      lightingShader.setFloat("light.quadratic", 0.032f);

      // view projection transformations
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
		    (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
      glm::mat4 view = camera.GetViewMatrix();
      lightingShader.setMat4("projection", projection);
      lightingShader.setMat4("view", view);


      // bind diffuse map
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuseMap);
      // bind specular map
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularMap);
      glm::mat4 model = glm::mat4(1.0f);
      // render the cube
      glBindVertexArray(cubeVAO);
      for (GLuint i = 0; i < 10; i++)
	{ 
	  // world transformation
	  model = glm::translate(model, cubePositions[i]);
	  float angle = 20.0f * i;
	  model = glm::rotate(model, glm::radians(angle),
			      glm::vec3(1.0f, 0.3f, 0.5f));
	  
	  lightingShader.setMat4("model", model);

	  glDrawArrays(GL_TRIANGLES, 0, 36);
	}

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

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  const GLfloat cameraSpeed = 2.5f * deltaTime;
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
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
  if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos; // reversed since y-coord go from bottom to top
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

GLuint loadTexture(const char* path)
{
  GLuint textureID;
  glGenTextures(1, &textureID);

  GLint width, height, nrComponents;
  unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data)
    {
      GLenum format;
      if (nrComponents == 1)
	format = GL_RED;
      else if (nrComponents == 3)
	format = GL_RGB;
      else if (nrComponents == 4)
	format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
		   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		     GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    }
  else
    {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
    }

  return textureID;
}
