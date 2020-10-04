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
  float tri1[] = {
		      // first triangle
		      0.5f,  0.5f, 0.0f, // top right
		      0.5f, -0.45f, 0.0f, // bottom right
		     -0.45f,  0.5f, 0.0f // top left
  };
  float tri2[] = {
		      // second triangle
		      0.45f, -0.5f, 0.0f, // bottom right
		      -0.5f, -0.5f, 0.0f, // bottom left
		      -0.5f, 0.45f, 0.0f  // top left
  };

  unsigned int VBO[2];  // vertex buffer objects
  glGenBuffers(2, VBO); // generate multiple VBOs

  // first triangle
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri1),
	       tri1, GL_STATIC_DRAW); // copy our array into buffer
  // second triangle
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // create a GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri2),
	       tri2, GL_STATIC_DRAW); // copy our array into buffer


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

  // Orange Shader source
  const char *fs1 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
  // Yellow Shader source
  const char *fs2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

  // fragment shader object for orange
  unsigned int fragmentShader1;
  fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader1, 1, &fs1, NULL);
  glCompileShader(fragmentShader1);
  // check for compile-time error
  glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }  
  // fagment shader object for yellow
  unsigned int fragmentShader2;
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fs2, NULL);
  glCompileShader(fragmentShader2);

  // check for compile-time error
  glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }

  // shader program
  unsigned int shaderProgramYellow;
  shaderProgramYellow = glCreateProgram();
  glAttachShader(shaderProgramYellow, vertexShader);
  glAttachShader(shaderProgramYellow, fragmentShader2);
  glLinkProgram(shaderProgramYellow);

  // check for compile error
  glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
  if (!success)
    {
      glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
      std::cout << "ERROR::YELLOW::SHADER::PROGRAM::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }

  // shader program
  unsigned int shaderProgramOrange;
  shaderProgramOrange = glCreateProgram();
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShader1);
  glLinkProgram(shaderProgramOrange);

  // check for compile error
  glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
  if (!success)
    {
      glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
      std::cout << "ERROR::ORANGE::SHADER::PROGRAM::COMPILATION_FAILED\n" <<
	infoLog << std::endl;
    }

  
  // our vertex and fragment shaders are no longer needed
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  // vertex array object
  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);
  // first triangle
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // second triangle
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);



  glViewport(0, 0, 800, 600);

  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shaderProgramOrange);
      // draw first trinagle
      glBindVertexArray(VAO[0]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // draw second triangle
      glUseProgram(shaderProgramYellow);
      glBindVertexArray(VAO[1]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      
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
