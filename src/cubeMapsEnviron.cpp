/* implement our camera class */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <map>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint loadTexture(const char* path);
GLuint loadCubemap(std::vector<const char*> faces);
// screen size
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// camera positions
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
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

  // tell gflw to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

  // configure global opengl state
  glEnable(GL_DEPTH_TEST);

  
  
  Shader shader("./cubeMapsEnviron.vs", "./cubeMapsEnviron.fs", nullptr);
  Shader skyboxShader("./skybox.vs", "./skybox.fs", nullptr); 
  
  // points for our rectangle created with two triangles
  float cubeVertices[] =
    {
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

  GLfloat skyboxVertices[] =
    {
     -1.0f,  1.0f, -1.0f,
     -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     -1.0f,  1.0f, -1.0f,

     -1.0f, -1.0f,  1.0f,
     -1.0f, -1.0f, -1.0f,
     -1.0f,  1.0f, -1.0f,
     -1.0f,  1.0f, -1.0f,
     -1.0f,  1.0f,  1.0f,
     -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

     -1.0f, -1.0f,  1.0f,
     -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     -1.0f, -1.0f,  1.0f,

     -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     -1.0f,  1.0f,  1.0f,
     -1.0f,  1.0f, -1.0f,

     -1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };
  
  // cube VBO, VAO
  GLuint cubeVAO, cubeVBO; 
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO); 
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),
	       &cubeVertices, GL_STATIC_DRAW); 
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(void*)(6 * sizeof(GLfloat)));


  // skybox VAO
  GLuint skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),
	       &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			(void*)0);

  // load textures
  GLuint cubeTexture = loadTexture("./container.jpg");

  std::vector<const char*> faces
    {
     "skybox/right.jpg",
     "skybox/left.jpg",
     "skybox/top.jpg",
     "skybox/bottom.jpg",
     "skybox/front.jpg",
     "skybox/back.jpg"
    };

  GLuint cubemapTexture = loadCubemap(faces);
  
  shader.use();
  shader.setInt("skybox", 0);

  skyboxShader.use();
  skyboxShader.setInt("skybox", 0);

  while(!glfwWindowShouldClose(window))
    {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // draw scene as normal
      shader.use();
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = camera.GetViewMatrix();
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
				    (float)SCR_WIDTH / (float) SCR_HEIGHT,
				    0.1f, 100.0f);
      
      shader.setMat4("model", model);
      shader.setMat4("view", view);
      shader.setMat4("projection", projection);
      shader.setVec3("cameraPos", camera.Position);

      // render cubes=
      glBindVertexArray(cubeVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      // draw skybox as last
      glDepthFunc(GL_LEQUAL); // change depth function so depth test
                             // passes when values are equal to the
                             // depth buffer's content
      skyboxShader.use();
      view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
      skyboxShader.setMat4("view", view);
      skyboxShader.setMat4("projection", projection);
      // skybox cube
      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // set depth func back to default
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &skyboxVBO);

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

/*
 * loads a cubemap texture from 6 individual faces
 * order:
 * +x (right)
 * -x (left)
 * +y (top)
 * -y (bottom)
 * +Z (front)
 * -Z (back)
 */
GLuint loadCubemap(std::vector<const char*> faces)
{
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  GLint width, height, nrChannels;
  for (GLuint i = 0; i < faces.size(); i++) {
    unsigned char* data = stbi_load(faces[i], &width, &height,
				   &nrChannels, 0);
    if (data)
      {
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
		     width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		     data);
	stbi_image_free(data);
      }
    else
      {
	std::cout << "Cubemap texture failed to load at path: " << faces[i]
		  << std::endl;
	stbi_image_free(data);
      }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
