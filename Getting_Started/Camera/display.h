#ifndef DISPLAY_H
#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class Display{

  private:
    
    int width, height;

  public:
  GLFWwindow* window;
  Display(int width, int height);
  

  
  void swapBuffers();
  bool shouldClose() const;
  void pollEvents();



};

#endif