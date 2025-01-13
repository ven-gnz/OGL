#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0"; 

const char *yelloworangeFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
 "   FragColor = vec4(ourColor,1.0);\n"
"}\0";

struct vec3 {
    float x;
    float y;
    float z;
};

float rect_vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Setting up shaders
    Vertex shader: issue creation from OpenGL, send sourcecode, compile, check for errors. Same algo for fragment.
    */ 

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "Vertex Shader not compiled\n" << infoLog << std::endl;
    }

    unsigned int yellowFragmentShader;
    yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader,1,&yelloworangeFragmentShaderSource,NULL);
    glCompileShader(yellowFragmentShader);

     glGetShaderiv(yellowFragmentShader,GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(yellowFragmentShader,512,NULL,infoLog);
        std::cout << "Yellow Fragment Shader not compiled\n" << infoLog << std::endl;
    }

    /*
    High level shader program : 
    linked version of executable and shaders. 
    This program is what we use in the main render loop.
    */ 

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,yellowFragmentShader);

    


    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) { glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);}


    glDeleteShader(vertexShader);
    glDeleteShader(yellowFragmentShader);


    float vert[] = {
        //vertices       // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   

    };

        /*
    Setting up data flow : 
    1) Setup a vertex Array object
    2) 
    Setup Vertex Buffer Object and bind it
    2) Stream (copy) the data from program to the bound buffer.
    */

    //unsigned int VBO, VAO, EBO;
    unsigned int VBO,VAO;
   

     glGenVertexArrays(1, &VAO);
     //glGenBuffers(1, &EBO);

     glGenBuffers(1 , &VBO);

     glBindVertexArray(VAO);
     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    // glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window))
    {
       
        processInput(window);

        // render
        // ------
        glClearColor(0.4f, 0.3f, 0.3f, 1.0f); // a lovely red wine puke
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
       // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);
      

    
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}