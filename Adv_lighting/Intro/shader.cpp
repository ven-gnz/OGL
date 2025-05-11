#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);

      std::stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();
    

      vertexCode   = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      
      if (geometryPath != nullptr) {
          gShaderFile.open(geometryPath);
          std::stringstream gShaderStream;
          gShaderStream << gShaderFile.rdbuf();
          gShaderFile.close();
          geometryCode = gShaderStream.str();
      }
    }

     catch(std::ifstream::failure e){
      std::cout << "ERROR::SHADER::FILE_NOT_READ" <<std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex,GL_COMPILE_STATUS, &success);
    if(!success){
       glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&fShaderCode,NULL);
    glCompileShader(fragment);

     if(!success){
       glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

     if (geometryPath != nullptr) {

         const char* gShaderCode = geometryCode.c_str();
         geometry = glCreateShader(GL_GEOMETRY_SHADER);
         glShaderSource(geometry, 1, &gShaderCode, NULL);
         glCompileShader(geometry);
     }

     if (!success) {
         glGetShaderInfoLog(vertex, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
     }



    ID = glCreateProgram();
    glAttachShader(ID,vertex);
    glAttachShader(ID,fragment);
    if (geometryPath != nullptr) {
        glAttachShader(ID, geometry);
    }
    
    glLinkProgram(ID);

    glGetProgramiv(ID,GL_LINK_STATUS, &success);
    if(!success){
      glGetProgramInfoLog(ID,512,NULL,infoLog);
      std::cout << "linking failed" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }
    
        
}
      
  
  void Shader::use() const{ glUseProgram(ID); }


  void Shader::setBool(const std::string& name, bool value) const{
      glUniform1i(glGetUniformLocation(ID,name.c_str()), value);
  }
  void Shader::setInt(const std::string& name, int value) const{
      glUniform1i(glGetUniformLocation(ID, name.c_str()),value);
  }
  void Shader::setFloat(const std::string& name, float value) const{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
  }

  void Shader::setMat4(const std::string& name, glm::mat4 mat)const{
    int matLoc = glGetUniformLocation(ID,name.c_str());
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(mat));
  }

  void Shader::setVec3(const std::string& name, glm::vec3 v) const {
      int vecLoc = glGetUniformLocation(ID,name.c_str());
      glUniform3fv(vecLoc,1,glm::value_ptr(v));
  }

  void Shader::setVec2(const std::string& name, glm::vec2 v) const {
      int vecL = glGetUniformLocation(ID, name.c_str());
      glUniform2fv(vecL, 1, glm::value_ptr(v));
  }

  void Shader::setVec3(const std::string&name, float a, float b, float c) const{
    int vecLoc = glGetUniformLocation(ID,name.c_str());
    glm::vec3 temp = glm::vec3(a,b,c);
    glUniform3fv(vecLoc, 1, glm::value_ptr(temp));
  }

  
    


