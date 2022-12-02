#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void reflect(unsigned int vao, int toDraw, Shader ourShader, glm::mat4 transform);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.f, 0.5f, 2.0f);
float change = 0.02f;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile our shader zprogram
    // ------------------------------------
    // Una luz
    Shader lightingShader("Shaders/2.2.basic_lighting.vs", "Shaders/2.2.basic_lighting.fs");
    //Cubo de luz
    Shader lightCubeShader("Shaders/2.2.light_cube.vs", "Shaders/2.2.light_cube.fs");
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    float verticesCentro[] = {
        // Cara Enfrente
        0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f,// J
        0.025f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // E
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //Cero

        0.025f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f,//E
        0.07f, 0.068f, 0.0f, 0.0f, 0.0f, 1.0f,//F
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,//Cero

        0.07f, 0.068f, 0.0f, 0.0f, 0.0f, 1.0f,//F
        0.1f, 0.025f, 0.0f, 0.0f, 0.0f, 1.0f, //G
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //Cero

        0.1f, 0.025f, 0.0f, 0.0f, 0.0f, 1.0f, //G
        0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //K
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //Cero

        // Cara Atrás
        0.0f, 0.1f, -0.1f, 0.0f, 0.0f, -1.0f,// J'
        0.025f, 0.1f, -0.1f, 0.0f, 0.0f, -1.0f, // E'
        0.0f, 0.0f, -0.1f, 0.0f, 0.0f, -1.0f, //Cero'

        0.025f, 0.1f, -0.1f, 0.0f, 0.0f, -1.0f,//E'
        0.07f, 0.068f, -0.1f, 0.0f, 0.0f, -1.0f,//F'
        0.0f, 0.0f, -0.1f, 0.0f, 0.0f, -1.0f,//Cero

        0.07f, 0.068f, -0.1f, 0.0f, 0.0f, -1.0f,//F'
        0.1f, 0.025f, -0.1f, 0.0f, 0.0f, -1.0f, //G'
        0.0f, 0.0f, -0.1f, 0.0f, 0.0f,-1.0f, //Cero'

        0.1f, 0.025f, -0.1f, 0.0f, 0.0f, -1.0f, //G'
        0.1f, 0.0f, -0.1f, 0.0f, 0.0f, -1.0f, //K'
        0.0f, 0.0f, -0.1f, 0.0f, 0.0f, -1.0f, //Cero'

        // Lados
        0.025f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // E
        0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,// J
        0.0f, 0.1f, -0.1f, 0.0f, 1.0f, 0.0f,// J'

        0.025f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // E
        0.0f, 0.1f, -0.1f, 0.0f, 1.0f, 0.0f,// J'
        0.025f, 0.1f, -0.1f, 0.0f, 1.0f, -0.0f, // E'

        0.07f, 0.068f, 0.0f, 0.0064f, 0.009f, 0.0f,//F
        0.025f, 0.1f, 0.0f, 0.0064f, 0.009f, 0.0f, // E
        0.025f, 0.1f, -0.1f, 0.0064f, 0.009f, 0.0f, // E'

        0.07f, 0.068f, 0.0f, 0.0064f, 0.009f, 0.0f,//F
        0.025f, 0.1f, -0.1f, 0.0064f, 0.009f, 0.0f, // E'
        0.07f, 0.068f, -0.1f, 0.0064f, 0.009f, 0.0f,//F'

        0.1f, 0.025f, 0.0f, 0.0086f, 0.006f, 0.0f, //G
        0.07f, 0.068f, 0.0f, 0.0086f, 0.006f, 0.0f,//F
        0.07f, 0.068f, -0.1f, 0.0086f, 0.006f, 0.0f,//F'

        0.1f, 0.025f, 0.0f, 0.0086f, 0.006f, 0.0f, //G
        0.07f, 0.068f, -0.1f, 0.0086f, 0.006f, 0.0f,//F'
        0.1f, 0.025f, -0.1f, 0.0086f, 0.006f, 0.0f, //G'

        0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //K
        0.1f, 0.025f, 0.0f, 1.0f, 0.0f, 0.0f, //G
        0.1f, 0.025f, -0.1f, 1.0f, 0.0f, 0.0f, //G'

        0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //K
        0.1f, 0.025f, -0.1f, 1.0f, 0.0f, 0.0f, //G'
        0.1f, 0.0f, -0.1f, 1.0f, 0.0f, 0.0f, //K'

    };

    // Falta acomodar eso
    float verticesR[] = {

        // Cara Enfrente
        0.0f,0.2f,0.4f, 0.0f, 0.0f, 1.0f,//D
        0.07f,0.26f,0.4f, 0.0f, 0.0f, 1.0f,//L
        0.095f,0.16f,0.4f, 0.0f, 0.0f, 1.0f,//C

        0.095f,0.16f,0.4f, 0.0f, 0.0f, 1.0f,//C
        0.2f,0.2f,0.4f, 0.0f, 0.0f, 1.0f,//M
        0.16f,0.1f,0.4f, 0.0f, 0.0f, 1.0f,//H

        0.16f,0.1f,0.4f, 0.0f, 0.0f, 1.0f,//H
        0.26f,0.07f,0.4f, 0.0f, 0.0f, 1.0f,//N
        0.2f,0.0f,0.4f, 0.0f, 0.0f, 1.0f,//I

        0.0f,0.2f,0.4f, 0.0f, 0.0f, 1.0f,//D
        0.095f,0.16f,0.4f, 0.0f, 0.0f, 1.0f,//C
        0.025f,0.1f,0.4f, 0.0f, 0.0f, 1.0f, //E 

        0.07f,0.068f,0.4f, 0.0f, 0.0f, 1.0f,//F
        0.095f,0.16f,0.4f, 0.0f, 0.0f, 1.0f,//C
        0.16f,0.1f,0.4f, 0.0f, 0.0f, 1.0f,//H 

        0.1,0.025,0.4f, 0.0f, 0.0f, 1.0f,//G
        0.16f,0.1f,0.4f, 0.0f, 0.0f, 1.0f,//H
        0.2f,0.0f,0.4f, 0.0f, 0.0f, 1.0f,//I 


        // Cara Detrás
        0.0f,0.2f,0.3f, 0.0f, 0.0f, -1.0f,//D'
        0.07f,0.26f,0.3f, 0.0f, 0.0f, -1.0f,//L'
        0.095f,0.16f,0.3f, 0.0f, 0.0f, -1.0f,//C'

        0.095f,0.16f,0.3f, 0.0f, 0.0f, -1.0f,//C'
        0.2f,0.2f,0.3f, 0.0f, 0.0f, -1.0f,//M'
        0.16f,0.1f,0.3f, 0.0f, 0.0f, -1.0f,//H'

        0.16f,0.1f,0.3f, 0.0f, 0.0f, -1.0f,//H'
        0.26f,0.07f,0.3f, 0.0f, 0.0f, -1.0f,//N'
        0.2f,0.0f,0.3f, 0.0f, 0.0f, -1.0f,//I'

        0.0f,0.2f,0.3f, 0.0f, 0.0f, -1.0f,//D'
        0.095f,0.16f,0.3f, 0.0f, 0.0f, -1.0f,//C'
        0.025f,0.1f,0.3f, 0.0f, 0.0f, -1.0f, //E'

        0.07f,0.068f,0.3f, 0.0f, 0.0f, -1.0f,//F'
        0.095f,0.16f,0.3f, 0.0f, 0.0f, -1.0f,//C'
        0.16f,0.1f,0.3f, 0.0f, 0.0f, -1.0f,//H'

        0.1,0.025,0.3f, 0.0f, 0.0f, -1.0f,//G'
        0.16f,0.1f,0.3f, 0.0f, 0.0f, -1.0f,//H'
        0.2f,0.0f,0.3f, 0.0f, 0.0f, -1.0f,//I' 

        // Lados
        0.07f, 0.26f, 0.4f , -0.006f, 0.007f, 0.0f, //L
        0.0f,0.2f,0.4f, -0.006f, 0.007f, 0.0f,//D
        0.0f,0.2f,0.3f, -0.006f, 0.007f, 0.0f,//D'

        0.07f, 0.26f, 0.4f , -0.006f, 0.007f, 0.0f, //L
        0.0f,0.2f,0.3f, -0.006f, 0.007f, 0.0f,//D'
        0.07f,0.26f,0.3f, -0.006f, 0.007f, 0.0f,//L'

        0.095f,0.16f,0.4f, 0.01f, 0.0025f, 0.0f,//C
        0.07f, 0.26f, 0.4f , 0.01f, 0.0025f, 0.0f, //L
        0.07f,0.26f,0.3f, 0.01f, 0.0025f, 0.0f,//L'

        0.095f,0.16f,0.4f, 0.01f, 0.0025f, 0.0f,//C
        0.07f,0.26f,0.3f, 0.01f, 0.0025f, 0.0f,//L'
        0.095f,0.16f,0.3f, 0.01f, 0.0025f, 0.0f,//C'

        0.2f,0.2f,0.4f, -0.01f, 0.0025f, 0.0f,//M
        0.095f,0.16f,0.4f, -0.01f, 0.0025f, 0.0f,//C
        0.095f,0.16f,0.3f, -0.01f, 0.0025f, 0.0f,//C'

        0.2f,0.2f,0.4f, -0.01f, 0.0025f, 0.0f,//M
        0.095f,0.16f,0.3f, -0.01f, 0.0025f, 0.0f,//C'
        0.2f,0.2f,0.3f, -0.01f, 0.0025f, 0.0f,//M'

        0.16f,0.1f,0.4f, 0.01f, -0.004f, 0.0f,//H 
        0.2f,0.2f,0.4f, 0.01f, -0.004f, 0.0f,//M
        0.2f,0.2f,0.3f, 0.01f, -0.004f, 0.0f,//M'

        0.16f,0.1f,0.4f, 0.01f, -0.004f, 0.0f,//H 
        0.2f,0.2f,0.3f, 0.01f, -0.004f, 0.0f,//M'
        0.16f,0.1f,0.3f, 0.01f, -0.004f, 0.0f,//H'

        0.26f,0.07f,0.4f, 0.01f, 0.004f, 0.0f,//N
        0.16f,0.1f,0.4f, 0.01f, 0.004f, 0.0f,//H 
        0.16f,0.1f,0.3f, 0.01f, 0.004f, 0.0f,//H'

        0.26f,0.07f,0.4f, 0.01f, 0.004f, 0.0f,//N
        0.16f,0.1f,0.3f, 0.01f, 0.004f, 0.0f,//H'
        0.26f,0.07f,0.3f, 0.01f, 0.004f, 0.0f,//N'

        0.2f,0.0f,0.4f, 0.007f, -0.006f, 0.0f,//I
        0.26f,0.07f,0.4f, 0.007f, -0.006f, 0.0f,//N
        0.26f,0.07f,0.3f, 0.007f, -0.006f, 0.0f,//N'

        0.2f,0.0f,0.4f, 0.007f, -0.006f, 0.0f,//I
        0.26f,0.07f,0.3f, 0.007f, -0.006f, 0.0f,//N'
        0.2f,0.0f,0.3f, 0.007f, -0.006f, 0.0f,//I' 
    };

    float verticesAm[] = {

        // Cara Enfrente
        0.0f,0.1f,0.8f, 0.0f, 0.0f, 1.0f, //J
        0.0f,0.2f,0.8f, 0.0f, 0.0f, 1.0f, //D  
        0.025f,0.1f,0.8f, 0.0f, 0.0f, 1.0f,//E

        0.025f,0.1f,0.8f, 0.0f, 0.0f, 1.0f,//E
        0.095f,0.16f,0.8f, 0.0f, 0.0f, 1.0f,//C 
        0.07f,0.068f,0.8f, 0.0f, 0.0f, 1.0f,//F

        0.07f,0.068f,0.8f, 0.0f, 0.0f, 1.0f,//F
        0.16f,0.1f,0.8f, 0.0f, 0.0f, 1.0f,//H
        0.1,0.025,0.8f, 0.0f, 0.0f, 1.0f,//G

        0.1,0.025,0.8f, 0.0f, 0.0f, 1.0f,//G 
        0.1f,0.0f,0.8f, 0.0f, 0.0f, 1.0f,//K 
        0.2f,0.0f,0.8f, 0.0f, 0.0f, 1.0f,//I 

        // Cara Atras
        0.0f,0.1f,0.7f, 0.0f, 0.0f, -1.0f, //J'
        0.0f,0.2f,0.7f, 0.0f, 0.0f, -1.0f, //D'  
        0.025f,0.1f,0.7f, 0.0f, 0.0f, -1.0f,//E'

        0.025f,0.1f,0.7f, 0.0f, 0.0f, -1.0f,//E'
        0.095f,0.16f,0.7f, 0.0f, 0.0f, -1.0f,//C' 
        0.07f,0.068f,0.7f, 0.0f, 0.0f, -1.0f,//F'

        0.07f,0.068f,0.7f, 0.0f, 0.0f, -1.0f,//F'
        0.16f,0.1f,0.7f, 0.0f, 0.0f, -1.0f,//H'
        0.1,0.025,0.7f, 0.0f, 0.0f, -1.0f,//G'

        0.1,0.025,0.7f, 0.0f, 0.0f, -1.0f,//G'
        0.1f,0.0f,0.7f, 0.0f, 0.0f, -1.0f,//K'
        0.2f,0.0f,0.7f, 0.0f, 0.0f, -1.0f,//I'

        // Lados
        0.025f,0.1f,0.8f, 0.01f, 0.0025f, 0.0f,//E
        0.0f,0.2f,0.8f, 0.01f, 0.0025f, 0.0f, //D
        0.0f,0.2f,0.7f, 0.01f, 0.0025f, 0.0f, //D'

        0.025f,0.1f,0.8f, 0.01f, 0.0025f, 0.0f,//E
        0.0f,0.2f,0.7f, 0.01f, 0.0025f, 0.0f, //D'
        0.025f,0.1f,0.7f, 0.01f, 0.0025f, 0.0f,//E'

        0.095f,0.16f,0.8f, -0.006f, 0.007f, 0.0f,//C
        0.025f,0.1f,0.8f, -0.006f, 0.007f, 0.0f,//E 
        0.025f,0.1f,0.7f, -0.006f, 0.007f, 0.0f,//E'

        0.095f,0.16f,0.8f, -0.006f, 0.007f, 0.0f,//C
        0.025f,0.1f,0.7f, -0.006f, 0.007f, 0.0f,//E'
        0.095f,0.16f,0.7f, -0.006f, 0.007f, 0.0f,//C' 

        0.07f,0.068f,0.8f, 0.0092f, -0.0025f, 0.0f,//F
        0.095f,0.16f,0.8f, 0.0092f, -0.0025f, 0.0f,//C
        0.095f,0.16f,0.7f, 0.0092f, -0.0025f, 0.0f,//C' 

        0.07f,0.068f,0.8f, 0.0092f, -0.0025f, 0.0f,//F
        0.095f,0.16f,0.7f, 0.0092f, -0.0025f, 0.0f,//C'
        0.07f,0.068f,0.7f, 0.0092f, -0.0025f, 0.0f,//F' 

        0.16f,0.1f,0.8f, -0.0032f, 0.009f, 0.0f,//H
        0.07f,0.068f,0.8f, -0.0032f, 0.009f, 0.0f,//F
        0.07f,0.068f,0.7f, -0.0032f, 0.009f, 0.0f,//F' 

        0.16f,0.1f,0.8f, -0.0032f, 0.009f, 0.0f,//H
        0.07f,0.068f,0.7f, -0.0032f, 0.009f, 0.0f,//F' 
        0.16f,0.1f,0.7f, -0.0032f, 0.009f, 0.0f,//H'

        0.1,0.025,0.8f, 0.0075f, -0.006f, 0.0f,//G
        0.16f,0.1f,0.8f, 0.0075f, -0.006f, 0.0f,//H
        0.16f,0.1f,0.7f, 0.0075f, -0.006f, 0.0f,//H'

        0.1,0.025,0.8f, 0.0075f, -0.006f, 0.0f,//G
        0.16f,0.1f,0.7f, 0.0075f, -0.006f, 0.0f,//H'
        0.1,0.025,0.7f, 0.0075f, -0.006f, 0.0f,//G'

        0.2f,0.0f,0.8f, 0.0025f, 0.01f, 0.0f,//I
        0.1,0.025,0.8f, 0.0025f, 0.01f, 0.0f,//G
        0.1,0.025,0.7f, 0.0025f, 0.01f, 0.0f,//G'

        0.2f,0.0f,0.8f, 0.0025f, 0.01f, 0.0f,//I
        0.1,0.025,0.7f, 0.0025f, 0.01f, 0.0f,//G'
        0.2f,0.0f,0.7f, 0.0025f, 0.01f, 0.0f,//I'
    };
    
    // first, configure the cube's VAO (and VBO)
    unsigned int VBOs[10], VAOs[10];
    glGenVertexArrays(10, VAOs);
    glGenBuffers(10, VBOs);
    
    //===========Centro Amarillo====================
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCentro), verticesCentro, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[0]);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    //===========Circulo Rojo====================
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesR), verticesR, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[1]);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    //===========Circulo Amarillo====================
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAm), verticesAm, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[2]);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO, lightVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    // Setup style
    ImGui::StyleColorsDark();
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGui_ImplGlfwGL3_NewFrame(); 

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            ImGui::Begin("Controles");
            ImGui::Text("w - avanzar camara \n");
            ImGui::Text("s - retroceder camara \n");
            ImGui::Text("d - ir  la derecha camara \n");
            ImGui::Text("a - ir  la izquierda camara \n");
            ImGui::Text("I - avanzar luz \n");
            ImGui::Text("K - retroceder luz \n");
            ImGui::Text("J - luz izquierda \n");
            ImGui::Text("L - luz derecha\n");
            ImGui::Text("o - luz arriba \n");
            ImGui::Text("p - luz abajo\n");
            ImGui::End();

        }

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        glm::mat4 model;

        // Rendering
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
       
        // La luz
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Shader Teselación
        lightingShader.use();
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        //Centro
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.setVec3("objectColor", 0.87f, 0.734f, 0.082f);
        lightingShader.setFloat("alpha", 1.0); //Para que sea sólido

        // world transformation
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 100);
        reflect(VAOs[0], 100, lightingShader, model);
        
        //Circulo rojo
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        lightingShader.setFloat("alpha", 1.0); //Para que sea sólido

        // world transformation
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 200);
        reflect(VAOs[1], 200, lightingShader, model);
        
        //Circulo amarillo
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.setVec3("objectColor", 0.87f, 0.734f, 0.082f);
        lightingShader.setFloat("alpha", 0.5); //Para que sea traslúcido

        // world transformation
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, 200);
        reflect(VAOs[2], 200, lightingShader, model);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAOs);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, VBOs);
    


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
        
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
        

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos.z = lightPos.z - change;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos.z = lightPos.z + change;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos.x = lightPos.x - change;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos.x = lightPos.x + change;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        lightPos.y = lightPos.y - change;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightPos.y = lightPos.y + change;
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void reflect(unsigned int vao, int toDraw, Shader ourShader, glm::mat4 transform) {

    //Transformacion (reflexion x)
    transform = glm::scale(transform, glm::vec3(1, -1, 1));
    ourShader.setMat4("model", transform);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, toDraw);
    //Transformacion (reflexion y)
    transform = glm::scale(transform, glm::vec3(-1, 1, 1));
    ourShader.setMat4("model", transform);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, toDraw);
    //Transformacion (reflexion x)
    transform = glm::scale(transform, glm::vec3(1, -1, 1));
    ourShader.setMat4("model", transform);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, toDraw);
}
