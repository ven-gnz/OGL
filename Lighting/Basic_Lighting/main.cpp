#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float mix = 0.2;
float fov = 45.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);





float vertices[] = {
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
};

    glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
    

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float yaw = -90.0f;
    float pitch;
    float roll;
    bool firstMouse = true;

    float lastX = 400, lastY = 300;

    Camera cameroni(cameraPos, cameraUp);

int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "COLOR", NULL, NULL);
    
     if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
   
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    

     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    
    Shader lightingShader("shader.vs","shader.fs");
    Shader lightCubeShader("lightsource.vs","lightsource.fs");


    unsigned int VBO,cubeVAO;

     glGenVertexArrays(1, &cubeVAO);
     glBindVertexArray(cubeVAO);

     glGenBuffers(1 , &VBO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glEnable(GL_DEPTH_TEST);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
       
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);
  
        glClearColor(0.11f, 0.11f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("objectColor",glm::vec3(1.0f, 0.5f, 0.31f));
        lightingShader.setVec3("lightColor",glm::vec3(1.0f, 1.0f, 1.0f)); // White light

        projection = glm::perspective(glm::radians(cameroni.Zoom), 3.0f, 0.1f, 100.0f);
        view = cameroni.GetViewMatrix();

        lightingShader.setMat4("projection",projection);
        lightingShader.setMat4("view",view);
        
        
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model",model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        
        // Remember to share view and projection matrices!

        lightCubeShader.use();
        lightCubeShader.setMat4("projection",projection);
        lightingShader.setMat4("view",view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);    
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    
    return 0;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        cameroni.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_S)== GLFW_PRESS){
        cameroni.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS){
        cameroni.ProcessKeyboard(LEFT, deltaTime);
    }
     if(glfwGetKey(window, GLFW_KEY_D)== GLFW_PRESS){
        cameroni.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    cameroni.ProcessMouseMovement(xoffset,yoffset,true);
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 1.0f){
        fov = 1.0f;
    }
    if(fov > 45.0f){
        fov = 45.0f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
        


    




