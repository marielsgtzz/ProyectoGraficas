#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <camera.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting positions
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 camLightPos(0.0f, 0.0f, 15.0f);
glm::vec3 focoLightPos(0.0f, 2.0f, 0.0f);
float change = 0.002f;

// object positions
glm::vec3 posPiso(0.0f, 5.0f, 0.0f);
glm::vec3 posRana1(3.0f, 0.0f, 3.0f);
glm::vec3 posRana2(-3.0f, 0.0f, 3.0f);
glm::vec3 posRana3(3.0f, 0.0f, -3.0f);
glm::vec3 posRana4(-3.0f, 0.0f, -3.0f);
glm::vec3 posEsquina1(7.0f, -0.15f, 8.0f);
glm::vec3 posEsquina2(-7.0f, -0.15f, 8.0f);
glm::vec3 posEsquina3(7.0f, -0.15f, -8.0f);
glm::vec3 posEsquina4(-7.0f, -0.15f, -8.0f);
glm::vec3 posAsiento1(3.0f, -1.5f, 3.0f);
glm::vec3 posAsiento2(-3.0f, -1.5f, 3.0f);
glm::vec3 posAsiento3(3.0f, -1.5f, -3.0f);
glm::vec3 posAsiento4(-3.0f, -1.5f, -3.0f);
glm::vec3 posZanahoria1(5.0f, -2.0f, 0.0f);
glm::vec3 posZanahoria2(-5.0f, -2.0f, 0.0f);
glm::vec3 posCentro(0.0f, 0.0f, 0.0f);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("Shaders/2.2.basic_lighting.vs", "Shaders/2.2.basic_lighting.fs");
    Shader lightCubeShader("Shaders/2.2.light_cube.vs", "Shaders/2.2.light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // posicion de los 2 focos estaticos
    glm::vec3 pointLightPositions[] = {
        glm::vec3(4.0f, 2.0f, 4.0f),
        glm::vec3(-4.0f, 2.0f, -4.0f),
    };

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
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };
    float verticesSuelo[]{
        // abajo
        -10.0f, -10.0f, -10.0f,  0.0f, 1.0f, 0.0f,
         10.0f, -10.0f, -10.0f, 0.0f, 1.0f, 0.0f,
         10.0f, -10.0f,  10.0f, 0.0f, 1.0f, 0.0f,
         10.0f, -10.0f,  10.0f, 0.0f, 1.0f, 0.0f,
        -10.0f, -10.0f,  10.0f, 0.0f, 1.0f, 0.0f,
        -10.0f, -10.0f, -10.0f, 0.0f, 1.0f, 0.0f,
    };
    float verticesRanas[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.2f,  0.2f, -0.2f, 0.0f, 0.0f, -1.0f,
         0.2f,  0.2f, -0.2f, 0.0f, 0.0f, -1.0f,
        -0.2f,  0.2f, -0.2f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,0.0f, 0.0f, 1.0f,
         0.2f,  0.2f,  0.2f,0.0f, 0.0f, 1.0f,
         0.2f,  0.2f,  0.2f,0.0f, 0.0f, 1.0f,
        -0.2f,  0.2f,  0.2f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,0.0f, 0.0f, 1.0f,

        -0.2f,  0.2f,  0.2f, -1.0f, 0.0f, 0.0f,
        -0.2f,  0.2f, -0.2f,-1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,-1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,-1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,-1.0f, 0.0f, 0.0f,
        -0.2f,  0.2f,  0.2f,-1.0f, 0.0f, 0.0f,

         0.2f,  0.2f,  0.2f, 1.0f, 0.0f, 0.0f,
         0.2f,  0.2f, -0.2f,1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,1.0f, 0.0f, 0.0f,
         0.2f,  0.2f,  0.2f,1.0f, 0.0f, 0.0f,

         // abajo
        -0.5f, -0.5f, -0.5f,0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,0.0f, -1.0f, 0.0f,

        // arriba
        -0.2f,  0.2f, -0.2f,  0.0f, 1.0f, 0.0f,
         0.2f,  0.2f, -0.2f, 0.0f, 1.0f, 0.0f,
         0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
         0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        -0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        -0.2f,  0.2f, -0.2f, 0.0f, 1.0f, 0.0f,
    };
    float verticesEsquinas[]{
        -1.75f, -1.75f, -1.75f,0.0f,  0.0f, -1.0f,
         1.75f, -1.75f, -1.75f,0.0f,  0.0f, -1.0f,
         0.4f,  0.4f, -0.4f,0.0f,  0.0f, -1.0f,
         0.4f,  0.4f, -0.4f,0.0f,  0.0f, -1.0f,
        -0.4f,  0.4f, -0.4f,0.0f,  0.0f, -1.0f,
        -1.75f, -1.75f, -1.75f,0.0f,  0.0f, -1.0f,

        -1.75f, -1.75f,  1.75f,0.0f,  0.0f,  1.0f,
         1.75f, -1.75f,  1.75f,0.0f,  0.0f,  1.0f,
         0.4f,  0.4f,  0.4f,0.0f,  0.0f,  1.0f,
         0.4f,  0.4f,  0.4f,0.0f,  0.0f,  1.0f,
        -0.4f,  0.4f,  0.4f,0.0f,  0.0f,  1.0f,
        -1.75f, -1.75f, 1.75f,0.0f,  0.0f,  1.0f,

        -0.4f,  0.4f,  0.4f,-1.0f,  0.0f,  0.0f,
        -0.4f,  0.4f, -0.4f,-1.0f,  0.0f,  0.0f,
        -1.75f, -1.75f, -1.75f,-1.0f,  0.0f,  0.0f,
        -1.75f, -1.75f, -1.75f,-1.0f,  0.0f,  0.0f,
        -1.75f, -1.75f, 1.75f,-1.0f,  0.0f,  0.0f,
        -0.4f,  0.4f,  0.4f,-1.0f,  0.0f,  0.0f,

         0.4f,  0.4f,  0.4f,1.0f,  0.0f,  0.0f,
         0.4f,  0.4f, -0.4f,1.0f,  0.0f,  0.0f,
         1.75f, -1.75f, -1.75f,1.0f,  0.0f,  0.0f,
         1.75f, -1.75f, -1.75f,1.0f,  0.0f,  0.0f,
         1.75f, -1.75f,  1.75f,1.0f,  0.0f,  0.0f,
         0.4f,  0.4f,  0.4f,1.0f,  0.0f,  0.0f,

         // abajo
        -1.75f, -1.75f, -1.75f,0.0f, -1.0f,  0.0f,
         1.75f, -1.75f, -1.75f,0.0f, -1.0f,  0.0f,
         1.75f, -1.75f,  1.75f,0.0f, -1.0f,  0.0f,
         1.75f, -1.75f,  1.75f,0.0f, -1.0f,  0.0f,
        -1.75f, -1.75f,  1.75f,0.0f, -1.0f,  0.0f,
        -1.75f, -1.75f, -1.75f,0.0f, -1.0f,  0.0f,

        // arriba
        -0.4f,  0.4f, -0.4f,0.0f,  1.0f,  0.0f,
         0.4f,  0.4f, -0.4f,0.0f,  1.0f,  0.0f,
         0.4f,  0.4f,  0.4f,0.0f,  1.0f,  0.0f,
         0.4f,  0.4f,  0.4f,0.0f,  1.0f,  0.0f,
        -0.4f,  0.4f,  0.4f,0.0f,  1.0f,  0.0f,
        -0.4f,  0.4f, -0.4f,0.0f,  1.0f,  0.0f,
    };
    float verticesAsientos[]{
        -0.3f, -0.3f, -0.3f,0.0f,  0.0f, -1.0f,
         0.3f, -0.3f, -0.3f,0.0f,  0.0f, -1.0f,
         0.8f,  0.8f, -0.8f,0.0f,  0.0f, -1.0f,
         0.8f,  0.8f, -0.8f,0.0f,  0.0f, -1.0f,
        -0.8f,  0.8f, -0.8f,0.0f,  0.0f, -1.0f,
        -0.3f, -0.3f, -0.3f,0.0f,  0.0f, -1.0f,

        -0.3f, -0.3f,  0.3f,0.0f,  0.0f,  1.0f,
         0.3f, -0.3f,  0.3f,0.0f,  0.0f,  1.0f,
         0.8f,  0.8f,  0.8f,0.0f,  0.0f,  1.0f,
         0.8f,  0.8f,  0.8f,0.0f,  0.0f,  1.0f,
        -0.8f,  0.8f,  0.8f,0.0f,  0.0f,  1.0f,
        -0.3f, -0.3f, 0.3f,0.0f,  0.0f,  1.0f,

        -0.8f,  0.8f,  0.8f,-1.0f,  0.0f,  0.0f,
        -0.8f,  0.8f, -0.8f,-1.0f,  0.0f,  0.0f,
        -0.3f, -0.3f, -0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f, -0.3f, -0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f, -0.3f, 0.3f,-1.0f,  0.0f,  0.0f,
        -0.8f,  0.8f,  0.8f,-1.0f,  0.0f,  0.0f,

         0.8f,  0.8f,  0.8f,1.0f,  0.0f,  0.0f,
         0.8f,  0.8f, -0.8f,1.0f,  0.0f,  0.0f,
         0.3f, -0.3f, -0.3f,1.0f,  0.0f,  0.0f,
         0.3f, -0.3f, -0.3f,1.0f,  0.0f,  0.0f,
         0.3f, -0.3f,  0.3f,1.0f,  0.0f,  0.0f,
         0.8f,  0.8f,  0.8f,1.0f,  0.0f,  0.0f,

         // abajo
        -0.3f, -0.3f, -0.3f,  0.0f, -1.0f,  0.0f,
         0.3f, -0.3f, -0.3f,  0.0f, -1.0f,  0.0f,
         0.3f, -0.3f,  0.3f,  0.0f, -1.0f,  0.0f,
         0.3f, -0.3f,  0.3f,  0.0f, -1.0f,  0.0f,
        -0.3f, -0.3f,  0.3f,  0.0f, -1.0f,  0.0f,
        -0.3f, -0.3f, -0.3f,  0.0f, -1.0f,  0.0f,

        // arriba
        -0.8f,  0.8f, -0.8f,0.0f,  1.0f,  0.0f,
         0.8f,  0.8f, -0.8f,0.0f,  1.0f,  0.0f,
         0.8f,  0.8f,  0.8f,0.0f,  1.0f,  0.0f,
         0.8f,  0.8f,  0.8f,0.0f,  1.0f,  0.0f,
        -0.8f,  0.8f,  0.8f,0.0f,  1.0f,  0.0f,
        -0.8f,  0.8f, -0.8f, 0.0f,  1.0f,  0.0f,
    };
    float verticesZanahorias1[] = {
        -1.5f, -0.2f, -0.2f,0.0f,  0.0f, -1.0f,
         1.5f, -0.2f, -0.2f,0.0f,  0.0f, -1.0f,
         1.5f,  0.2f, -0.2f,0.0f,  0.0f, -1.0f,
         1.5f,  0.2f, -0.2f,0.0f,  0.0f, -1.0f,
        -1.5f,  0.2f, -0.2f,0.0f,  0.0f, -1.0f,
        -1.5f, -0.2f, -0.2f,0.0f,  0.0f, -1.0f,

        -1.5f, -0.2f,  0.2f,0.0f,  0.0f,  1.0f,
         1.5f, -0.2f,  0.2f,0.0f,  0.0f,  1.0f,
         1.5f,  0.2f,  0.2f,0.0f,  0.0f,  1.0f,
         1.5f,  0.2f,  0.2f,0.0f,  0.0f,  1.0f,
        -1.5f,  0.2f,  0.2f,0.0f,  0.0f,  1.0f,
        -1.5f, -0.2f,  0.2f,0.0f,  0.0f,  1.0f,

        -0.2f,  0.2f,  0.2f,-1.0f,  0.0f,  0.0f,
        -0.2f,  0.2f, -0.2f,-1.0f,  0.0f,  0.0f,
        -0.2f, -0.2f, -0.2f,-1.0f,  0.0f,  0.0f,
        -0.2f, -0.2f, -0.2f,-1.0f,  0.0f,  0.0f,
        -0.2f, -0.2f,  0.2f,-1.0f,  0.0f,  0.0f,
        -0.2f,  0.2f,  0.2f,-1.0f,  0.0f,  0.0f,

         0.2f,  0.2f,  0.2f, 1.0f,  0.0f,  0.0f,
         0.2f,  0.2f, -0.2f, 1.0f,  0.0f,  0.0f,
         0.2f, -0.2f, -0.2f, 1.0f,  0.0f,  0.0f,
         0.2f, -0.2f, -0.2f, 1.0f,  0.0f,  0.0f,
         0.2f, -0.2f,  0.2f, 1.0f,  0.0f,  0.0f,
         0.2f,  0.2f,  0.2f, 1.0f,  0.0f,  0.0f,

         // abajo
        -1.5f, -0.2f, -0.2f, 0.0f, -1.0f,  0.0f,
         1.5f, -0.2f, -0.2f, 0.0f, -1.0f,  0.0f,
         1.5f, -0.2f,  0.2f, 0.0f, -1.0f,  0.0f,
         1.5f, -0.2f,  0.2f, 0.0f, -1.0f,  0.0f,
        -1.5f, -0.2f,  0.2f, 0.0f, -1.0f,  0.0f,
        -1.5f, -0.2f, -0.2f, 0.0f, -1.0f,  0.0f,

        // arriba
        -1.5f,  0.2f, -0.2f,0.0f,  1.0f,  0.0f,
         1.5f,  0.2f, -0.2f,0.0f,  1.0f,  0.0f,
         1.5f,  0.2f,  0.2f,0.0f,  1.0f,  0.0f,
         1.5f,  0.2f,  0.2f,0.0f,  1.0f,  0.0f,
        -1.5f,  0.2f,  0.2f,0.0f,  1.0f,  0.0f,
        -1.5f,  0.2f, -0.2f,0.0f,  1.0f,  0.0f,
    };
    float verticesCentro[]{
        -0.3f, -0.3f, -0.3f,0.0f,  0.0f, -1.0f,
         0.3f, -0.3f, -0.3f,0.0f,  0.0f, -1.0f,
         0.3f,  0.3f, -0.3f,0.0f,  0.0f, -1.0f,
         0.3f,  0.3f, -0.3f,0.0f,  0.0f, -1.0f,
        -0.3f,  0.3f, -0.3f,0.0f,  0.0f, -1.0f,
        -0.3f, -0.3f, -0.3f,0.0f,  0.0f, -1.0f,

        -0.3f, -0.3f,  0.3f,0.0f,  0.0f,  1.0f,
         0.3f, -0.3f,  0.3f,0.0f,  0.0f,  1.0f,
         0.3f,  0.3f,  0.3f,0.0f,  0.0f,  1.0f,
         0.3f,  0.3f,  0.3f,0.0f,  0.0f,  1.0f,
        -0.3f,  0.3f,  0.3f,0.0f,  0.0f,  1.0f,
        -0.3f, -0.3f,  0.3f,0.0f,  0.0f,  1.0f,

        -0.3f,  0.3f,  0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f,  0.3f, -0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f, -0.3f, -0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f, -0.3f, -0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f, -0.3f,  0.3f,-1.0f,  0.0f,  0.0f,
        -0.3f,  0.3f,  0.3f,-1.0f,  0.0f,  0.0f,

         0.3f,  0.3f,  0.3f, 1.0f,  0.0f,  0.0f,
         0.3f,  0.3f, -0.3f, 1.0f,  0.0f,  0.0f,
         0.3f, -0.3f, -0.3f, 1.0f,  0.0f,  0.0f,
         0.3f, -0.3f, -0.3f, 1.0f,  0.0f,  0.0f,
         0.3f, -0.3f,  0.3f, 1.0f,  0.0f,  0.0f,
         0.3f,  0.3f,  0.3f, 1.0f,  0.0f,  0.0f,

         // abajo
        -0.3f, -0.3f, -0.3f, 0.0f, -1.0f,  0.0f,
         0.3f, -0.3f, -0.3f, 0.0f, -1.0f,  0.0f,
         0.3f, -0.3f,  0.3f, 0.0f, -1.0f,  0.0f,
         0.3f, -0.3f,  0.3f, 0.0f, -1.0f,  0.0f,
        -0.3f, -0.3f,  0.3f, 0.0f, -1.0f,  0.0f,
        -0.3f, -0.3f, -0.3f, 0.0f, -1.0f,  0.0f,

        // arriba
        -0.3f,  0.3f, -0.3f,0.0f,  1.0f,  0.0f,
         0.3f,  0.3f, -0.3f,0.0f,  1.0f,  0.0f,
         0.3f,  0.3f,  0.3f,0.0f,  1.0f,  0.0f,
         0.3f,  0.3f,  0.3f,0.0f,  1.0f,  0.0f,
        -0.3f,  0.3f,  0.3f,0.0f,  1.0f,  0.0f,
        -0.3f,  0.3f, -0.3f,0.0f,  1.0f,  0.0f,
    };
    
    // Piso
    unsigned int VBOs, VAOs;
    glGenVertexArrays(1, &VAOs);
    glGenBuffers(1, &VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSuelo), verticesSuelo, GL_STATIC_DRAW);
    glBindVertexArray(VAOs);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // RANAS
   // 1
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // 2
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 4
    unsigned int VBO4, VAO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO4);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ESQUINAS
    // 1
    unsigned int VBO5, VAO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO5);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // 2
    unsigned int VBO6, VAO6;
    glGenVertexArrays(1, &VAO6);
    glGenBuffers(1, &VBO6);
    glBindBuffer(GL_ARRAY_BUFFER, VBO6);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO6);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // 3
    unsigned int VBO7, VAO7;
    glGenVertexArrays(1, &VAO7);
    glGenBuffers(1, &VBO7);
    glBindBuffer(GL_ARRAY_BUFFER, VBO7);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO7);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // 4
    unsigned int VBO8, VAO8;
    glGenVertexArrays(1, &VAO8);
    glGenBuffers(1, &VBO8);
    glBindBuffer(GL_ARRAY_BUFFER, VBO8);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO8);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // ASIENTOS
    // 1
    unsigned int VBO9, VAO9;
    glGenVertexArrays(1, &VAO9);
    glGenBuffers(1, &VBO9);
    glBindBuffer(GL_ARRAY_BUFFER, VBO9);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO9);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2
    unsigned int VBO10, VAO10;
    glGenVertexArrays(1, &VAO10);
    glGenBuffers(1, &VBO10);
    glBindBuffer(GL_ARRAY_BUFFER, VBO10);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO10);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3
    unsigned int VBO11, VAO11;
    glGenVertexArrays(1, &VAO11);
    glGenBuffers(1, &VBO11);
    glBindBuffer(GL_ARRAY_BUFFER, VBO11);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO11);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 4
    unsigned int VBO12, VAO12;
    glGenVertexArrays(1, &VAO12);
    glGenBuffers(1, &VBO12);
    glBindBuffer(GL_ARRAY_BUFFER, VBO12);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO12);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ZANAHORIAS
    // 1
    unsigned int VBO13, VAO13;
    glGenVertexArrays(1, &VAO13);
    glGenBuffers(1, &VBO13);
    glBindBuffer(GL_ARRAY_BUFFER, VBO13);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias1), verticesZanahorias1, GL_STATIC_DRAW);
    glBindVertexArray(VAO13);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2
    unsigned int VBO14, VAO14;
    glGenVertexArrays(1, &VAO14);
    glGenBuffers(1, &VBO14);
    glBindBuffer(GL_ARRAY_BUFFER, VBO14);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias1), verticesZanahorias1, GL_STATIC_DRAW);
    glBindVertexArray(VAO14);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3
    unsigned int VBO15, VAO15;
    glGenVertexArrays(1, &VAO15);
    glGenBuffers(1, &VBO15);
    glBindBuffer(GL_ARRAY_BUFFER, VBO15);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias1), verticesZanahorias1, GL_STATIC_DRAW);
    glBindVertexArray(VAO15);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 4
    unsigned int VBO16, VAO16;
    glGenVertexArrays(1, &VAO16);
    glGenBuffers(1, &VBO16);
    glBindBuffer(GL_ARRAY_BUFFER, VBO16);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias1), verticesZanahorias1, GL_STATIC_DRAW);
    glBindVertexArray(VAO16);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Centro
    unsigned int VBO17, VAO17;
    glGenVertexArrays(1, &VAO17);
    glGenBuffers(1, &VBO17);
    glBindBuffer(GL_ARRAY_BUFFER, VBO17);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCentro), verticesCentro, GL_STATIC_DRAW);
    glBindVertexArray(VAO17);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    lightingShader.use();

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    // Setup style
    ImGui::StyleColorsDark();
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // render loop
    // -----------
    float inc = 0.0;
    while (!glfwWindowShouldClose(window))
    {

        ImGui_ImplGlfwGL3_NewFrame();
        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {

            ImGui::Begin("Controles");
            ImGui::Text("Movimientos camara\n");
            ImGui::Text("w - avanzar || s - retroceder || d - derecha || a - izquierda ");

            ImGui::Text("Movimientos reflector\n");
            ImGui::Text("I - avanzar || K - retroceder || J - izquierda || L - derecha || O - arriba || P - abajo ");

            ImGui::Text("Movimientos foco\n");
            ImGui::Text("T - avanzar || G - retroceder || F - izquierda || H - derecha || U - arriba || Y - abajo ");
            ImGui::End();

        }
        // per-frame time logic
       // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //PISO
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 1.0f, 0.8f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        // world transformation
        glm::mat4 model;
        model = glm::mat4(1.0f);
        model = glm::translate(model, posPiso);
        lightingShader.setMat4("model", model);
        // render the cube
        glBindVertexArray(VAOs);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // spotLight
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("camLightPos", camLightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // Foco omnidimensional derecha
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032);

        // Foco omnidimensional izquierda
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032);

        // Foco omnidimensional centro
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("focoLightPos", focoLightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // also draw the lamp object(s)
        //REFLECTOR MOVIBLE
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //REFLECTOR CAMARA
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, camLightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //FOCO CENTRO
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, focoLightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //FOCOS ESTATICOS
        for (unsigned int i = 0; i < 2; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        if (inc >= 0 && inc < 0.6) {
            inc += 0.0002f;
            // ESQUINAS
            // 1
            lightingShader.use();
            lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
            lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, posEsquina1);
            model = glm::translate(model, glm::vec3(1.0f, -7.0f, 0.0f));
            model = glm::translate(model, glm::vec3(1.0f, 7.0f*inc, 0.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(VAO5);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // 2
            lightingShader.use();
            lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
            lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, posEsquina2);
            model = glm::translate(model, glm::vec3(-1.0f, -7.0f, 0.0f));
            model = glm::translate(model, glm::vec3(-1.0f, 7.0f * inc, 0.0f));
            //model = glm::translate(model, glm::vec3(1.0f, -3.0f, 0.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(VAO6);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // 3
            lightingShader.use();
            lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
            lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, posEsquina3);
            model = glm::translate(model, glm::vec3(1.0f, -7.0f, 0.0f));
            model = glm::translate(model, glm::vec3(1.0f, 7.0f * inc, 0.0f));
            //model = glm::translate(model, glm::vec3(1.0f, -3.0f, 0.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(VAO7);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // 4
            lightingShader.use();
            lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
            lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, posEsquina4);
            model = glm::translate(model, glm::vec3(-0.5f, -7.0f, 0.0f));
            model = glm::translate(model, glm::vec3(-0.5f, 7.0f * inc, 0.0f));
            //model = glm::translate(model, glm::vec3(1.0f, -3.0f, 0.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(VAO8);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        else if (inc >= 0.6 && inc < 1.0){
            // ESQUINAS
                // 1
                lightingShader.use();
                lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
                lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                lightingShader.setMat4("projection", projection);
                lightingShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::translate(model, posEsquina1);
                model = glm::translate(model, glm::vec3(1.0f, -3.0f, 0.0f));
                lightingShader.setMat4("model", model);
                glBindVertexArray(VAO5);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // 2
                lightingShader.use();
                lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
                lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                lightingShader.setMat4("projection", projection);
                lightingShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::translate(model, posEsquina2);
                model = glm::translate(model, glm::vec3(-1.0f, -3.0f, 0.0f));
                lightingShader.setMat4("model", model);
                glBindVertexArray(VAO6);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // 3
                lightingShader.use();
                lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
                lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                lightingShader.setMat4("projection", projection);
                lightingShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::translate(model, posEsquina3);
                model = glm::translate(model, glm::vec3(1.0f, -3.0f, 0.0f));
                lightingShader.setMat4("model", model);
                glBindVertexArray(VAO7);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // 4
                lightingShader.use();
                lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
                lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                lightingShader.setMat4("projection", projection);
                lightingShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::translate(model, posEsquina4);
                model = glm::translate(model, glm::vec3(-1.0f, -3.0f, 0.0f));
                lightingShader.setMat4("model", model);
                glBindVertexArray(VAO8);
                glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // RANAS
       // 1
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.5f, 0.8f, 0.8f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("posRana1", posRana1);
        lightingShader.setVec3("viewPos", camera.Position);

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        // world transformation
        model = glm::mat4(1.0f);
        model = glm::translate(model, posRana1);
        model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
        lightingShader.setMat4("model", model);
        // render cube
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 2
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.5f, 0.8f, 0.8f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posRana2);
        model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 3
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.5f, 0.8f, 0.8f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posRana3);
        model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 4
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.5f, 0.8f, 0.8f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posRana4);
        model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO4);
        glDrawArrays(GL_TRIANGLES, 0, 36);


       

        // ASIENTOS
        // 1
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.8f, 0.7f, 0.5f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posAsiento1);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO9);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 2
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.8f, 0.7f, 0.5f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posAsiento2);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO10);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 3
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.8f, 0.7f, 0.5f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posAsiento3);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO11);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 4
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.8f, 0.7f, 0.5f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posAsiento4);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO12);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ZANAHORIAS
        // 1
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.9f, 0.5f, 0.1f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posZanahoria1);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO13);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 2
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.9f, 0.5f, 0.1f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posZanahoria2);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO14);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 3
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.9f, 0.5f, 0.1f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posZanahoria2);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, glm::vec3(5.0f, -2.0f, 5.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO15);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 4
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.9f, 0.5f, 0.1f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posZanahoria2);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, glm::vec3(-5.0f, -2.0f, 5.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO16);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // CENTRO
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.3f, 0.5f, 0.9f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCentro);
        model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO17);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &VAOs);
    glDeleteBuffers(1, &VBO);

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
    //Movimiento camara y reflector
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camLightPos.z = camLightPos.z - deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camLightPos.z = camLightPos.z + deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camLightPos.x = camLightPos.x - deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camLightPos.x = camLightPos.x + deltaTime;
    }

    //Movimiento foco omnidireccional centro
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        focoLightPos.z = focoLightPos.z - change;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        focoLightPos.z = focoLightPos.z + change;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        focoLightPos.x = focoLightPos.x - change;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        focoLightPos.x = focoLightPos.x + change;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        focoLightPos.y = focoLightPos.y - change;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        focoLightPos.y = focoLightPos.y + change;

    //Movimiento reflector
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

    camLightPos.x = xpos;
    camLightPos.y = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}