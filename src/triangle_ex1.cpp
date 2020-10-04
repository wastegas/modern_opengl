/* Draw 2 traingles by adding more vertices. */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

  // points for our triangle
  float vertices[] = {
		      // first triangle
		      0.5f,  0.5f, 0.0f, // top right
		      0.5f, -0.45f, 0.0f, // bottom right
		     -0.45f,  0.5f, 0.0f, // top left
		      // second triangle
		      0.45f, -0.5f, 0.0f, // bottom right
		      -0.5f, -0.5f, 0.0f, // bottom left
		      -0.5f, 0.45f, 0.0f  // top left
  };

  unsigned int VBO;  // vertex buffer object
  glGenBuffers(1, &VBO); // generate with buffer id 1
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
	       vertices, GL_STATIC_DRAW); // copy our array into buffer

  // vertex shader source
  const char *vs =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

  // vertex shader object
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vs, NULL);
  glCompileShader(vertexShader);

  // check for compile-time erros
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }

  // fragment shader source
  const char *fs =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

  // fragment shader object
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fs, NULL);
  glCompileShader(fragmentShader);

  // check for compile-time error
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }

  // shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for compile error
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
    {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }

  // activate our shader program
  glUseProgram(shaderProgram);

  // our vertex and fragment shaders are no longer needed
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);


  glViewport(0, 0, 800, 600);

  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      
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
