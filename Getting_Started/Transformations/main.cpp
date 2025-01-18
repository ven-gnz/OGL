#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
    // positions              // texture coords
     0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,      1.0f, 0.0,   // bottom right
    -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,      0.0f, 1.0f    // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

/*
glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
glm::mat4 trans = glm::mat4(1.0f);
trans = glm::translate(trans, glm::vec3(1.0f,1.0f,0.0f));
vec = trans * vec;
std::cout << vec.x << vec.y << vec.z << std::endl;
*/
 

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


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

        ourShader.use();
        ourShader.setInt("texture1",0);
        ourShader.setInt("texture2",1);


        int cubes = 111;
        const float angleStride = glm::two_pi<float>() / cubes;

    
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

        glm::mat4 trans = glm::mat4(1.0f);

        /*
        Doing the matrix operations in reverse order changes the logic of function composition. Previously we translated the vertices, and applied rotation to the translated vertices.
        Therefore the cube spun in the bottom corner. The outer function was the translation and inner function was the rotation. Or as a computer scientist, we should think of this
        as a stack. First we apply the translation, then the rotation. So the translation is rotated.

        By flipping, we instantiate the stack or mapping in reverse order. Now we rotate the translation. This is apparent from looking at the top left corner. It stays in place,
        and our cube spins around this center point of 0.

        Further proof can propably be found from linear algebra. This is "extensive maths minor" version of what's happening.
        */

       unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");


    for(int i = 0; i < cubes; i++){

        float angle = glfwGetTime() + angleStride*i;

        trans = glm::mat4(1.0f);
    
        trans = glm::translate(trans, glm::vec3(0.5*cos(angle),0.5*sin(angle),0.0));
        trans = glm::scale(trans, glm::vec3(0.3,0.3,0.3));
        
        ourShader.use();
        transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6 , GL_UNSIGNED_INT, 0);


    }
        


        // clear the transform

   

    
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