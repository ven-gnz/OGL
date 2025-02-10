#ifndef TEXTURE_OLD_H
#define TEXTURE_OLD_H

#include <stb/stb_image.h>

class Texture_old
{
  public:
  unsigned int ID;

  Texture_old(const char* filePath);

  void bind() const;
  void setActive() const;
  unsigned int getID() const;

  private:
  const char* getFileExtension(const char* filePath);

};


#endif