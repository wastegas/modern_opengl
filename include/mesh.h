#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>

#include <string>
#include <vector>

using  std::string;
using  std::vector;

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

struct Texture {
  GLuint id;
  string type;
  string path;
};

class Mesh {
 public:
  // mesh Data
  vector<Vertex>    vertices;
  vector<GLuint>    indices;
  vector<Texture>   textures;
  GLuint VAO;

  // constructor
  Mesh(vector<Vertex> vertices, vector<GLuint> indices,
       vector<Texture> textures) {

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now we have our required ata
    setupMesh();
  }

  // render the mesh
  void Draw(Shader &shader) {
    // appropriate textures
    GLuint diffuseNr  = 1;
    GLuint specularNr = 1;
    GLuint normalNr   = 1;
    GLuint heightNr   = 1;
    for (GLuint i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit
                                        // before binding
      string number;
      string name = textures[i].type;
      if (name == "texture_diffuse")
	number = std::to_string(diffuseNr++); // transfer uint to stream
      else if (name == "texture_specular")
	number = std::to_string(specularNr++); // transfer uint to stream
      else if (name == "texture_height")
	number = std::to_string(heightNr++); // transfer uint to stream

      // now set the correct sampler to the correct texture uint
      glUniform1i(glGetUniformLocation(shader.ID,
					    (name + number).c_str()), i);
      // finally bind the texture
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good to set everything back to defaults once configured
    glActiveTexture(GL_TEXTURE0);
  }

 private:
  // render data
  GLuint VBO;
  GLuint EBO;

  // initialize all the buffer objects/arrays
  void setupMesh() {

    // create buffer/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Take advantage of struct and their memlry sequential layout
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
		 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
		 &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			  (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			  (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			  (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			  (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
  }
};

#endif // MESH_H
