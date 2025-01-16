#include "texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const char* filePath){

   

    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int width, heigth, nrChannels;


    bool isPng = (std::strcmp(getFileExtension(filePath), "png") == 0);
    stbi_set_flip_vertically_on_load(isPng);
    unsigned char *data = stbi_load(filePath, &width, &heigth, &nrChannels, 0);

    if(data){

      if(isPng){
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,heigth,0,GL_RGBA,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      else {
        // For other formats
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,heigth,0,GL_RGB,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

      }
        
    }
    else {
        std::cout << "Failed to load texture"<< ID << std::endl;
    }
    stbi_image_free(data);

}

const char* Texture::getFileExtension(const char* filePath) {
    const char* dot = strrchr(filePath, '.');
    if (!dot || dot == filePath) {
        return "";
    }
    return dot + 1;
}


void Texture::bind() const{
  glBindTexture(GL_TEXTURE_2D,ID);
}

void Texture::setActive() const{
  glActiveTexture(GL_TEXTURE0 + ID);
}

