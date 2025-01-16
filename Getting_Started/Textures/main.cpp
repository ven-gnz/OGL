#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include <iostream>
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float mix = 0.2;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello_TEX", NULL, NULL);
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

    Shader ourShader("shader.vs","shader.fs");
    Texture container("../../resources/container.jpg");
    Texture face("../../resources/face.png");


float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

 

    unsigned int VBO, VAO, EBO;

     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &EBO);
     glGenBuffers(1 , &VBO);

     glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

/*
    The weird bug was due to
    :  how I misunderstood the vertex streaming with the attributepointer and streamed them faultily to the GPU - this was further supported by
    one of the vertices assuming the position of 1.0, 1.0 - the first texture coordinate

    In the source code provided, it all made more sense. 
    We have 8 floats per row. T
    
    he enableVertexAttribArray is received in the vertex shader as layout
    (location = 0). So we have to make sure next element of type aPos in the receiving end is in fact a position.
    Same with color, and texture coordinate. So really, this was just a simple beginner mistake I could have maybe avoided had I more experience programming
    with languages where memory operations are not handled for the programmer.
*/
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8 *sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    while (!glfwWindowShouldClose(window))
    {
       
        processInput(window);

        ourShader.setFloat("mixValue",mix);
        glClearColor(0.4f, 0.3f, 0.3f, 1.0f); // a lovely red wine puke
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        container.bind();

        glActiveTexture(GL_TEXTURE1);
        face.bind();
         
        glBindVertexArray(VAO);

        ourShader.use();
        ourShader.setInt("texture1",0);
        ourShader.setInt("texture2",1);
        
       // glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES,0,3);
        
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

    
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
 

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS) {
        mix = 0.7;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS) {{
        mix = 0.2;
    }

    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}