#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using  std::string;
using  std::fstream;
using  std::cout;
using  std::endl;
using  std::vector;

GLuint TextureFromFile(const char *path, const string &directory,
		       bool gamma = false);

class Model {

 public:
  // model data
  vector<Texture> textures_loaded; // stores all teh textures loaded
  vector<Mesh>    meshes;
  string          directory;
  bool            gammaCorrection;

  // constructor expects a filepath to a 3D model
  Model(string const &path, bool gamma = false)
    : gammaCorrection(gamma) {

    loadModel(path);
  }

  // draws the model and thus all its meshes
  void Draw(Shader &shader) {
    for (GLuint i = 0; i < meshes.size(); i++) {
      meshes[i].Draw(shader);
    }
  }

 private:
  // loads a model with supported ASSIMP extensions from file
  void loadModel(string const &path) {
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
					     aiProcess_GenSmoothNormals |
					     aiProcess_FlipUVs |
					     aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
	!scene->mRootNode) {
      cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
      return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);


  }

  // process a node in a recursive fashion
  void processNode(aiNode *node, const aiScene *scene) {

    // process each mesh located at the current node
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
      // the node object only contains indices to index the
      // actual objects in the scene
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    // after we process all the meshes, we recursively
    // process the children nodes
    for (GLuint i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }

  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {


    // data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;


    
    // walk through each of the mesh's vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++) {

      Vertex vertex;
      glm::vec3 vector; // we declare a place holder vector
      // positions
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      // normals
      if (mesh->HasNormals()) {
	vector.x = mesh->mNormals[i].x;
	vector.y = mesh->mNormals[i].y;
	vector.z = mesh->mNormals[i].z;
	vertex.Normal = vector;
      }
      // texture coordinates
      if (mesh->mTextureCoords[0]) { // does the mesh contain texture coords
	glm::vec2 vec;
	// a vertex can contain up to 8 different texture coordinates
	// we thus make the assumption that we won't
	// use models where a vertex can have multiple texture coordinates
	// so we will take the first set (0).
	vec.x = mesh->mTextureCoords[0][i].x;
	vec.y = mesh->mTextureCoords[0][i].y;
	vertex.TexCoords = vec;
	// tangent
	vector.x = mesh->mTangents[i].x;
	vector.y = mesh->mTangents[i].y;
	vector.z = mesh->mTangents[i].z;
	vertex.Tangent = vector;
	// bitangent
	vector.x = mesh->mBitangents[i].x;
	vector.y = mesh->mBitangents[i].y;
	vector.z = mesh->mBitangents[i].z;
	vertex.Bitangent = vector;
      }
      else
	vertex.TexCoords = glm::vec2(0.0f, 0.0f);

      vertices.push_back(vertex);
    }

    // now walk through the mesh's faces and retrieve the corresponding
    // vertex indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      // retrieve all indices of teh face and store them
      // in the index vector
      for (GLuint j = 0; j < face.mNumIndices; j++) {
	//cout << "processing mesh index " <<  << endl;
	indices.push_back(face.mIndices[j]);
      }
    }


    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // we assume a convention for sampler names in the shaders. Each diffuse
    // should be named as 'texture_diffuseN' where N is a sequential number
    // ranging from 1 to MAX_SAMPLER_NUMBER
    // same applies to other texture as the following list summarizes:
    // diffuse: texture.diffuseN
    // specular: texture.specularN
    // normal:  texture.normalN

    // 1.diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material,
						       aiTextureType_DIFFUSE,
						       "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular
    vector<Texture> specularMaps = loadMaterialTextures(material,
							aiTextureType_SPECULAR,
							"texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    vector<Texture> normalMaps = loadMaterialTextures(material,
						      aiTextureType_HEIGHT,
						      "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    vector<Texture> heightMaps = loadMaterialTextures(material,
						      aiTextureType_AMBIENT,
						      "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


    // return a mesh object created from the extracted data
    return Mesh(vertices, indices, textures);
 
  }

  // checks all material textures of a given type and loads the textures if
  // they're not loaded yet. the required info is returned as
  // a Texture struct
  vector<Texture> loadMaterialTextures(aiMaterial *mat,
				       aiTextureType type,
				       string typeName) {
    vector<Texture> textures;
    
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      // check if texture was loaded before and if so, continue
      // to the next iteration: skip loading a new texture
      bool skip = false;
      for (GLuint j = 0; j < textures_loaded.size(); j++) {
	if (strcmp(textures_loaded[j].path.data(),
		   str.C_Str()) == 0) {
	  textures.push_back(textures_loaded[j]);
	  skip = true;
	  break;
	}
      }
      if (!skip) { // if texture hasn't been loaded, load it
	Texture texture;
	texture.id = TextureFromFile(str.C_Str(), this->directory);
	texture.type = typeName;
	texture.path = str.C_Str();
	textures.push_back(texture);
	textures_loaded.push_back(texture); // store it as a texture
      }
    }

    return textures;
  }
	  
};

GLuint TextureFromFile(const char *path, const string &directory, bool gamma) {
  string filename = string(path);
  filename = directory + '/' + filename;

  GLuint textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(),
				  &width,
				  &height,
				  &nrComponents, 0);
  
  if (data) {
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

  } else {
    std::cout << "Texture failed to load at path: " <<
      path << std::endl;
    stbi_image_free(data);
  }


  return textureID;
}


#endif // MODEL_H
