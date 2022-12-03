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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 camLightPos(0.0f, 0.0f, 3.0f);
glm::vec3 focoLightPos(0.0f, 2.0f, 0.0f);
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
    Shader shader1("Shaders/shader1.vs", "Shaders/shaderTira.fs");
    Shader shader2("Shaders/shader1.vs", "Shaders/shaderAsiento.fs");
    Shader shader3("Shaders/shader1.vs", "Shaders/shaderRana.fs");
    Shader shader4("Shaders/shader1.vs", "Shaders/shaderZanahoria.fs");
    Shader shader5("Shaders/shader1.vs", "Shaders/shaderCentro.fs");
    Shader shader6("Shaders/shader1.vs", "Shaders/shaderEsquina.fs");
    Shader shader7("Shaders/shader1.vs", "Shaders/shaderFondo.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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
    
    float verticesTiras[] = {
        0.4f, 0.4f, 0.0f, 0.0f,  0.0f,  0.0f,// centro del fan
        0.1f, 0.24f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.085f, 0.225f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.07f, 0.234f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.065f, 0.24f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.055f, 0.26f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.05f, 0.28f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.046f, 0.3f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.044f, 0.32f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.045f, 0.35f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.05f, 0.4f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.055f, 0.44f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.067f, 0.49f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.09f, 0.55f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.15f, 0.65f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.25f, 0.746f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.3f, 0.773f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.35f, 0.79f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.4f, 0.8f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.45f, 0.8f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.5f, 0.795f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.55f, 0.783f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.6f, 0.757f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.66f, 0.72f, 0.0f,  0.0f,  0.0f,  0.0f, // mitad 23
        0.757f, 0.6f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.783f, 0.55f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.795f, 0.5f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.8f, 0.45f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.8f, 0.4f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.79f, 0.35f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.773f, 0.3f, 0.0f,  0.0f,  0.0f,  0.0f,
        0.746f, 0.25f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.65f, 0.15f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.55f, 0.09f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.49f, 0.067f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.44f, 0.055f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.4f, 0.05f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.35f, 0.045f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.32f, 0.044f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.3f, 0.046f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.28f, 0.05f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.26f, 0.055f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.24f, 0.065f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.234f, 0.07f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.225f, 0.085f, 0.0f, 0.0f,  0.0f,  0.0f,
        0.24f, 0.1f, 0.0f, 0.0f,  0.0f,  0.0f,
    };

    //Asiento
    float verticesCirculo1[] = {
        0.4f, 0.4f, 0.0f, // centro del fan
        0.15f, 0.15f, 0.0f,
        0.13f, 0.175f, 0.0f,
        0.115f, 0.2f, 0.0f,
        0.105f, 0.23f, 0.0f,
        0.1f, 0.26f, 0.0f,
        0.1f, 0.28f, 0.0f,
        0.1f, 0.34f, 0.0f,
        0.105f, 0.37f, 0.0f,
        0.12f, 0.42f, 0.0f,
        0.15f, 0.5f, 0.0f,
        0.2f, 0.59f, 0.0f,
        0.25f, 0.645f, 0.0f,
        0.3f, 0.685f, 0.0f,
        0.35f, 0.71f, 0.0f,
        0.4f, 0.722f, 0.0f,
        0.45f, 0.72f, 0.0f,
        0.5f, 0.71f, 0.0f,
        0.55f, 0.69f, 0.0,
        0.6f, 0.65f, 0.0, // mitad 20
        0.65f, 0.6f, 0.0f,
        0.69f, 0.55f, 0.0,
        0.71f, 0.5f, 0.0f,
        0.72f, 0.45f, 0.0f,
        0.722f, 0.4f, 0.0f,
        0.71f, 0.35f, 0.0f,
        0.685f, 0.3f, 0.0f,
        0.645f, 0.25f, 0.0f,
        0.59f, 0.2f, 0.0f,
        0.5f, 0.15f, 0.0f,
        0.42f, 0.12f, 0.0f,
        0.37f, 0.105f, 0.0f,
        0.34f, 0.1f, 0.0f,
        0.28f, 0.1f, 0.0f,
        0.26f, 0.1f, 0.0f,
        0.23f, 0.105f, 0.0f,
        0.2f, 0.115f, 0.0f,
        0.175f, 0.13f, 0.0f,
        0.15f, 0.15f, 0.0f
    };

    //Ranas
    float indicesRanas[] = {
        0.26f, 0.26f, 0.0f, // centro del fan
        0.21f, 0.29f, 0.0f,
        0.207f, 0.3f, 0.0f,
        0.205f, 0.33f, 0.0f,
        0.206f, 0.37f, 0.0f,
        0.207f, 0.39f, 0.0f,
        0.222f, 0.42f, 0.0f,
        0.26f, 0.465f, 0.0f,
        0.31f, 0.498f, 0.0f,
        0.33f, 0.482f, 0.0f,
        0.34f, 0.46f, 0.0f,
        0.39f, 0.5f, 0.0f,
        0.41f, 0.51f, 0.0f,
        0.47f, 0.524f, 0.0f,
        0.5f, 0.521f, 0.0f,
        0.51f, 0.515f, 0.0f, //mitad 16
        0.521f, 0.5f, 0.0f,
        0.524f, 0.47f, 0.0f,
        0.51f, 0.41f, 0.0f,
        0.5f, 0.39f, 0.0f,
        0.46f, 0.34f, 0.0f,
        0.482f, 0.33f, 0.0f,
        0.498f, 0.31f, 0.0f,
        0.465f, 0.26f, 0.0f,
        0.42f, 0.222f, 0.0f,
        0.39f, 0.207f, 0.0f,
        0.37f, 0.206f, 0.0f,
        0.33f, 0.205f, 0.0f,
        0.3f, 0.207f, 0.0f,
        0.29f, 0.21f, 0.0f,
    };

    //Zanahorias
    float indicesZanahorias[] = {
        0.0f, 0.75f, 0.0f, // centro de fan
        0.0f, 0.987f, 0.0f,
        0.02f, 0.98f, 0.0f,
        0.035f, 0.97f, 0.0f,
        0.05f, 0.95f, 0.0f,
        0.071f, 0.9f, 0.0f,
        0.07f, 0.85f, 0.0f,
        0.067f, 0.8f, 0.0f,
        0.062f, 0.75f, 0.0f,
        0.05f, 0.65f, 0.0f,
        0.029f, 0.5f, 0.0f,
        0.02f, 0.46f, 0.0f,
        0.015f, 0.44f, 0.0f,
        0.01f, 0.43f, 0.0f,
        0.0f, 0.425f, 0.0f, // mitad
        -0.015f, 0.44f, 0.0f,
        -0.02f, 0.46f, 0.0f,
        -0.029f, 0.5f, 0.0f,
        -0.05f, 0.65f, 0.0f,
        -0.062f, 0.75f, 0.0f,
        -0.067f, 0.8f, 0.0f,
        -0.07f, 0.85f, 0.0f,
        -0.071f, 0.9f, 0.0f,
        -0.05f, 0.95f, 0.0f,
        -0.035f, 0.97f, 0.0f,
        -0.02f, 0.98f, 0.0f,
        0.0f, 0.987f, 0.0f,
    };

    // Centro 1
    float indicesCentro1[] = {
        0.0f, 0.0f, 0.0f, // centro de fan
        0.0f, 0.15f, 0.0f,
        0.04f, 0.1445f, 0.0f,
        0.08f, 0.127f, 0.0f,
        0.1f, 0.112f, 0.0f,
        0.12f, 0.09f, 0.0f,
        0.14f, 0.054f, 0.0f,
        0.1487f, 0.02f, 0.0f,
        0.15f, 0.0f, 0.0f, // 1/4 
        0.1487f, -0.02f, 0.0f,
        0.14f, -0.054f, 0.0f,
        0.12f, -0.09f, 0.0f,
        0.1f, -0.112f, 0.0f,
        0.08f, -0.127f, 0.0f,
        0.04f, -0.1445f, 0.0f,
        0.0f, -0.15f, 0.0f, // 1/2
        -0.04f, -0.1445f, 0.0f,
        -0.08f, -0.127f, 0.0f,
        -0.1f, -0.112f, 0.0f,
        -0.12f, -0.09f, 0.0f,
        -0.14f, -0.054f, 0.0f,
        -0.1487f, -0.02f, 0.0f,
        -0.15f, 0.0f, 0.0f, // 3/4 
        -0.1487f, 0.02f, 0.0f,
        -0.14f, 0.054f, 0.0f,
        -0.12f, 0.09f, 0.0f,
        -0.1f, 0.112f, 0.0f,
        -0.08f, 0.127f, 0.0f,
        -0.04f, 0.1445f, 0.0f,
        0.0f, 0.15f, 0.0f
    };

    // Centro 2
    float indicesCentro2[] = {
        0.0f, 0.0f, 0.0f, // centro de fan
        0.0f, 0.08f, 0.0f,
        0.01f, 0.0793f, 0.0f,
        0.03f, 0.074f, 0.0f,
        0.05f, 0.0625f, 0.0f,
        0.065f, 0.0465f, 0.0f,
        0.0742f, 0.03f, 0.0f,
        0.07935f, 0.01f, 0.0f,
        0.0781f, 0.017f, 0.0f,
        0.08f, 0.0f, 0.0f, // 1/4
        0.0781f, -0.017f, 0.0f,
        0.07935f, -0.01f, 0.0f,
        0.0742f, -0.03f, 0.0f,
        0.065f, -0.0465f, 0.0f,
        0.05f, -0.0625f, 0.0f,
        0.03f, -0.074f, 0.0f,
        0.01f, -0.0793f, 0.0f,
        0.0f, -0.08f, 0.0f, // 1/2
        -0.01f, -0.0793f, 0.0f,
        -0.03f, -0.074f, 0.0f,
        -0.05f, -0.0625f, 0.0f,
        -0.065f, -0.0465f, 0.0f,
        -0.0742f, -0.03f, 0.0f,
        -0.07935f, -0.01f, 0.0f,
        -0.0781f, -0.017f, 0.0f,
        -0.08f, 0.0f, 0.0f, // 3/4
        -0.0781f, 0.017f, 0.0f,
        -0.07935f, 0.01f, 0.0f,
        -0.0742f, 0.03f, 0.0f,
        -0.065f, 0.0465f, 0.0f,
        -0.05f, 0.0625f, 0.0f,
        -0.03f, 0.074f, 0.0f,
        -0.01f, 0.0793f, 0.0f,
        0.0f, 0.08f, 0.0f
    };

    //Esquinas fondo
    float verticesEsquinas[] = {
        -1.0f, 1.6f, 0.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, 1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
        -0.4f, 1.6f, 0.0f,  0.0f,  0.0f,  0.0f,
    };

    //Triangulos fondo
    float verticesFondo[] = {
        -2.0f, 1.4f, 0.0f,  0.0f,  0.0f,  -1.0f,
        -1.4f, 2.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
         -1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
    };

    float verticesFondo2[] = {
       0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.4f, 2.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.4f, 2.0f, 0.0f, 0.0f,  0.0f,  0.0f,
    };

    // posicion de los 2 focos estaticos
    glm::vec3 pointLightPositions[] = {
        glm::vec3(1.75f, 2.0f, 1.75f),
        glm::vec3(-1.75f, 2.0f, -1.75f),
    };

    unsigned int VBOs[100], VAOs[100];
    glGenVertexArrays(100, VAOs);
    glGenBuffers(100, VBOs);


    //Fondo
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFondo), verticesFondo, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[7]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*glBindBuffer(GL_ARRAY_BUFFER, VBOs[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFondo2), verticesFondo2, GL_STATIC_DRAW);
    glBindVertexArray(VAOs[8]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

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

        glm::mat4 transform;
        unsigned int transformLoc;

        ////Fondo - triangulos
        //shader7.use();
        //shader7.setInt("texture7", 7);
        //transform = glm::mat4(1.0f); // reset it to identity matrix
        //transform = glm::translate(transform, glm::vec3(1.0f, -1.0f, 0.0f));
        //transformLoc = glGetUniformLocation(shader7.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        //glBindVertexArray(VAOs[7]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        //lightingShader.use();
        //lightingShader.setVec3("objectColor", 0.8f, 0.8f, 0.73f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //lightingShader.setVec3("verticesFondo", verticesFondo);
        //lightingShader.setVec3("viewPos", camera.Position);
        //lightingShader.setVec3("objectColor", 0.8f, 0.8f, 0.73f);
        //lightingShader.setVec3("camLightPos", camLightPos);
        //lightingShader.setVec3("viewPos", camera.Position);

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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);


        // render cube
        glBindVertexArray(cubeVAO);
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);


        //lightingShader.use();
        //lightingShader.setMat4("projection", projection);
        //lightingShader.setMat4("view", view);
        //transform = glm::mat4(1.0f); // reset it to identity matrix
        //transform = glm::translate(transform, glm::vec3(1.0f, -1.0f, 0.0f));
        //transform = glm::scale(transform, glm::vec3(2));
        //lightingShader.setMat4("transform", transform);
        //glBindVertexArray(VAOs[7]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);


        glBindVertexArray(VAOs[7]);
        transform = glm::mat4(1.0f); // reset it to identity matrix
        lightingShader.setMat4("transform", transform);
        glDrawArrays(GL_TRIANGLES, 0, 15);
        

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

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
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