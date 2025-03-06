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

float positions[] = {
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

float normals[] = {

      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,

      0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,

     -1.0f,  0.0f,  0.0f,
     -1.0f,  0.0f,  0.0f,
     -1.0f,  0.0f,  0.0f,
     -1.0f,  0.0f,  0.0f,
     -1.0f,  0.0f,  0.0f,
     -1.0f,  0.0f,  0.0f,

      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,

      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,

      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f

};

float texCoords[] = {

    // Back face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    // Front face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    // Left face
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // Right face
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // Bottom face
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    // Top face
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
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

    std::vector<std::string> textures_faces = {
    "../../resources/skybox/right.jpg",
    "../../resources/skybox/left.jpg",
    "../../resources/skybox/top.jpg",
    "../../resources/skybox/bottom.jpg",
    "../../resources/skybox/front.jpg",
    "../../resources/skybox/back.jpg"
    };
    

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLSL", NULL, NULL);
    
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
    
    Shader red("shaders/shader.vs","shaders/red.frag");
    Shader blue("shaders/shader.vs", "shaders/blue.frag");
    Shader green("shaders/shader.vs", "shaders/green.frag");
    Shader yellow("shaders/shader.vs", "shaders/yellow.frag");

    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(red.ID, "Matrices");
    unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(blue.ID, "Matrices");
    unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(green.ID, "Matrices");
    unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(yellow.ID, "Matrices");

    glUniformBlockBinding(red.ID, uniformBlockIndexRed, 0);
    glUniformBlockBinding(blue.ID, uniformBlockIndexBlue, 0);
    glUniformBlockBinding(green.ID, uniformBlockIndexGreen, 0);
    glUniformBlockBinding(yellow.ID, uniformBlockIndexYellow, 0);

    

   // unsigned int frontTexture = loadTexture("../../resources/marble.jpg"); // 0
   // unsigned int backTexture = loadTexture("../../resources/metal.png"); // 1
   // unsigned int texture = loadTexture("../../resources/container.jpg");

    unsigned int uboMatrices;
    glGenBuffers(1 ,&uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));


    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1 , &cubeVBO);
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    size_t pos = sizeof(positions);
    size_t norm = sizeof(normals);
    size_t tex = sizeof(texCoords);

    //glBufferData(GL_ARRAY_BUFFER, pos + norm + tex, nullptr, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, pos, &positions);
    //glBufferSubData(GL_ARRAY_BUFFER, pos, norm, &normals);
    //glBufferSubData(GL_ARRAY_BUFFER, pos+norm, tex, &texCoords);
    
    glBufferData(GL_ARRAY_BUFFER, pos, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pos, &positions);
    

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3, GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));
    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));
    //glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);


    
    //blancoShader.setInt("frontTexture", 0); // marble
    //blancoShader.setInt("backTexture", 1); // metal
   
    glm::mat4 projection = vertical_fov_project(45.0f, aspect, 0.1, 100);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
   
        processInput(window);
  
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cameroni.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
       
        glBindVertexArray(cubeVAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, frontTexture);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, backTexture);
        red.use();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        red.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        green.use();
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.0f));
        green.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        yellow.use();
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
        yellow.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        blue.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
        blue.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
       
       

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
   
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);


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


unsigned int loadCubeMap(std::vector < std::string> faces) 
{

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    unsigned char* data;
    for (int i = 0; i < 6; i++) {
        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        

        else {
            std::cout << "Texture loading failed :" << textures_faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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


        


    




