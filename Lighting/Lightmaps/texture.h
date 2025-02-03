#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb/stb_image.h>

class Texture
{
  public:
  unsigned int ID;

  Texture(const char* filePath);

  void bind() const;
  void setActive() const;
  unsigned int getID() const;

  private:
  const char* getFileExtension(const char* filePath);

};


#endif