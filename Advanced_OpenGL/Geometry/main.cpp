#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
#include "Light.cpp"
#include "model.h"
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubeMap(std::vector<std::string> faces);


glm::mat4 vertical_fov_project(float fovY, float aspect ,float front, float back);
unsigned int loadTexture(const char *path);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float aspect = (float) SCR_WIDTH / (float) SCR_HEIGHT;
float fov = 45.0f;

float points[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
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

    float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

    Camera cameroni(cameraPos, cameraUp);

    

    int main()
    {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Geometry_Shaders", NULL, NULL);

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

        //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        Shader testShader("shaders/shader.vs", "shaders/shader.fs", "shaders/geo.g");

        Model backpack("../../resources/backpack/backpack.obj");


    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
   
     glm::mat4 projection = vertical_fov_project(45.0f, aspect, 0.1, 100);
     glEnable(GL_DEPTH_TEST);
    
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
   
        processInput(window);
  
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cameroni.GetViewMatrix();
        testShader.use();
        testShader.setMat4("projection", projection);
        testShader.setMat4("view", view);
        testShader.setMat4("model", model);

        testShader.setFloat("time", glfwGetTime());
        backpack.Draw(testShader);
       
       

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
   

    glfwTerminate();
    return 0;
}


unsigned int loadTexture(char const* path){

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width,&height, &nrComponents, 0);
    if(data){

        GLenum format = GL_RGBA; 
        if (nrComponents == 1) format = GL_RED;
        if (nrComponents == 3) format = GL_RGB;
       
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

    }
    else {
        std::cout << "Texture loading failed : path" << path << ::std::endl;
        stbi_image_free(data);

    }
    return textureID;
}


// symmetric vertical fov projection matrix
// front can be substituted for near and back to far respectively
glm::mat4 vertical_fov_project(float fovY, float aspect ,float front, float back)
{
    const float deg2rad = glm::acos(-1.0f) / 180;

    float tangent = glm::tan(fovY/ 2 * deg2rad);
    float top = front * tangent;
    float right = top * aspect;

    glm::mat4x4 projection = 
    
    { front/right,             0,     0,    0,
      0,    front/top,               0,    0,
      0, 0, -(back+front) / (back-front),  -1,
      0, 0, -(2*back*front)/(back-front),  0
    };

    return projection;

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


        


    




