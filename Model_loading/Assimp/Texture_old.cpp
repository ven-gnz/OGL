#include "Texture_old.h"
#include <glad/glad.h>
#include <iostream>

Texture_old::Texture_old(const char* filePath){

    glGenTextures(1, &ID);
    int width, heigth, nrChannels;

    unsigned char *data = stbi_load(filePath, &width, &heigth, &nrChannels, 0);

    if(data){
      GLenum conf;

        if(nrChannels == 1)      { conf = GL_RED; }
        if(nrChannels == 3)      { conf = GL_RGB; }
        else if(nrChannels == 4) { conf = GL_RGBA; }

      glBindTexture(GL_TEXTURE_2D, ID);
      glTexImage2D(GL_TEXTURE_2D, 0, conf, width,heigth,0,conf,GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      stbi_image_free(data);

    }
    else {
        std::cout << "Failed to load texture"<< ID << std::endl;
    }
    

}

const char* Texture_old::getFileExtension(const char* filePath) {
    const char* dot = strrchr(filePath, '.');
    if (!dot || dot == filePath) {
        return "";
    }
    return dot + 1;
}


void Texture_old::bind() const{
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture_old::setActive() const{
  glActiveTexture(GL_TEXTURE0 + ID);
}

