#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "display.h"
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float mix = 0.2;

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main()
{
    
    
    Display display(SCR_WIDTH,SCR_HEIGHT);
    Shader ourShader("shader.vs","shader.fs");
    Texture container("../../resources/container.jpg");
    Texture face("../../resources/face.png");


    unsigned int VBO, VAO;

     glGenVertexArrays(1, &VAO);
     glGenBuffers(1 , &VBO);

     glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

        ourShader.use();
        ourShader.setInt("texture1",0);
        ourShader.setInt("texture2",1);

    glEnable(GL_DEPTH_TEST);  
    while (!display.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        display.processInput();

        ourShader.setFloat("mixValue",mix);
        glClearColor(0.4f, 0.3f, sin(glfwGetTime()), 1.0f); // a lovely red wine puke
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        container.bind();

        glActiveTexture(GL_TEXTURE1);
        face.bind();

        /*
        Exercise 2

        As it name might suggest, the view matrix's coordinate vector describes the point where the projection takes
        place from. It is our point of view to the world, from where applying the perspective takes place.

        In this scene, changing the x or y greatly will quickly lead to seeing nothing as the world space is not entirely
        filled with cubes. Some sort of dynamic filling of the world space would be cool to learn after/towards the end
        of the course. And maybe loading bigger models or renders f.ex a procedurally generated amalgamation of cubical
        mess.
        */
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        // view = glm::translate(view, glm::vec3(0.0f,0.0f,7.0f));
        // view = glm::translate(view, glm::vec3(0.0f,10.0f,7.0f));
        // view = glm::translate(view, glm::vec3(2.0f,2.0f,2.0f));
        ourShader.setMat4("view",view);

        /* Exercise 1

        The field of view ratio seems to affect how far the perceived objects are. It is the relationship,
        or the scale between these. By decreasing it, the near plane approaches camera. By increasing it, the near plane
        escapes the camera. 45 degree angle preserves the uniform triangles which can be imagined as a side of the frustum.


        The aspect ratio describes the relationship or scale between width and height. By default, our width was 800 and height
        600, giving a nice aspect ratio of 1.333... By decreasing it, two things happen:
        1) the perceived width of our scene increases; the perspective frustum widens
        2) the perceived height of our scene decreases; the perspective frustum lengthens.
        
        By increasing, the opposites occur.
        */

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 3.0f, 0.1f, 100.0f);
        ourShader.setMat4("projection",projection);
        glBindVertexArray(VAO);

        for(unsigned int i = 0; i < std::size(cubePositions); i++){
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = i % 3 == 0 ? 90.0f*glfwGetTime() : 0.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(angle, angle, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    
        display.swapBuffers();
        display.pollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
 

    glfwTerminate();
    return 0;
}




