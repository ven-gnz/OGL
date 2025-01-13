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
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"; 

const char *orangeShaderFramgmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
 "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char *yelloworangeFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
 "   FragColor = vec4(0.7f, 0.8f, 0.2f, 1.0f);\n"
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

    unsigned int orangeFragmentShader,yellowFragmentShader;
    orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFragmentShader,1, &orangeShaderFramgmentShaderSource,NULL);
    glCompileShader(orangeFragmentShader);

    glGetShaderiv(orangeFragmentShader,GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(orangeFragmentShader,512,NULL,infoLog);
        std::cout << "Orange Fragment Shader not compiled\n" << infoLog << std::endl;
    }

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

    unsigned int orangeShaderProgram, yellowShaderProgram;
    orangeShaderProgram = glCreateProgram();
    glAttachShader(orangeShaderProgram,vertexShader);
    glAttachShader(orangeShaderProgram,orangeFragmentShader);

    /*
    Just south of two hours of debugging I gave up and compared the source code to the tutorials.
    Turns out I erased the linking command when I was trying to clean up the code to make it easier to debug and compare.
    Live and learn!
    */
    glLinkProgram(orangeShaderProgram);

    glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);

    if(!success) { glGetProgramInfoLog(orangeShaderProgram,512,NULL,infoLog); }

    
    glDeleteShader(orangeFragmentShader);

    yellowShaderProgram = glCreateProgram();
    glAttachShader(yellowShaderProgram,vertexShader);
    glAttachShader(yellowShaderProgram,yellowFragmentShader);

    glLinkProgram(yellowShaderProgram);

    glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);

    if(!success) { glGetProgramInfoLog(yellowShaderProgram,512,NULL,infoLog);}


    glDeleteShader(vertexShader);
    glDeleteShader(yellowFragmentShader);


    float vert[] = {
            -0.0f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.25f,  0.5f, 0.0f, // triangle 0

    };

    float vert2[] = {            
            0.5f, -0.5f, 0.0f,
            1.0f, -0.5,  0.0f,
            0.75f, 0.5f, 0.0f // triangle 1 };
    };


        /*
    Setting up data flow : 
    1) Setup a vertex Array object
    2) 
    Setup Vertex Buffer Object and bind it
    2) Stream (copy) the data from program to the bound buffer.
    */

    //unsigned int VBO, VAO, EBO;
    unsigned int VBO[2];
    unsigned int VAO[2];

     glGenVertexArrays(1, &VAO[0]);
     glGenVertexArrays(1, &VAO[1]);
     //glGenBuffers(1, &EBO);

     glGenBuffers(1 , &VBO[0]);

     glBindVertexArray(VAO[0]);
     glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    // glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1 , &VBO[1]);
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert2), vert2, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window))
    {
       
        processInput(window);

        // render
        // ------
        glClearColor(0.4f, 0.3f, 0.3f, 1.0f); // a lovely red wine puke
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(orangeShaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
       // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);
       glUseProgram(yellowShaderProgram);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES,0,3);

    
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteVertexArrays(1, &VAO[1]);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &VBO[1]);
    glDeleteProgram(orangeShaderProgram);

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