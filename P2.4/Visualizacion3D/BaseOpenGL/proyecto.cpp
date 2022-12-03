#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <shader_s.h>
#include <shader.h>
#include <camera.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// posiciones
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
//glm::vec3 posZanahoria3(0.0f, 0.0f, 0.0f);
//glm::vec3 posZanahoria4(5.0f, -2.0f, 0.0f);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto 2 - Figuras", NULL, NULL);
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
    Shader lightingShader("1.colors.vs", "1.colors.fs");
    Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float verticesSuelo[]{
        // abajo
        -10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f,  10.0f,
         10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f, -10.0f,
    };

    float verticesRanas[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.2f,  0.2f, -0.2f,
         0.2f,  0.2f, -0.2f,
        -0.2f,  0.2f, -0.2f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.2f,  0.2f,  0.2f,
         0.2f,  0.2f,  0.2f,
        -0.2f,  0.2f,  0.2f,
        -0.5f, -0.5f,  0.5f,

        -0.2f,  0.2f,  0.2f,
        -0.2f,  0.2f, -0.2f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.2f,  0.2f,  0.2f,

         0.2f,  0.2f,  0.2f,
         0.2f,  0.2f, -0.2f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.2f,  0.2f,  0.2f,

         // abajo
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        // arriba
        -0.2f,  0.2f, -0.2f,
         0.2f,  0.2f, -0.2f,
         0.2f,  0.2f,  0.2f,
         0.2f,  0.2f,  0.2f,
        -0.2f,  0.2f,  0.2f,
        -0.2f,  0.2f, -0.2f,
    };

    float verticesEsquinas[]{
        -1.75f, -1.75f, -1.75f,
         1.75f, -1.75f, -1.75f,
         0.4f,  0.4f, -0.4f,
         0.4f,  0.4f, -0.4f,
        -0.4f,  0.4f, -0.4f,
        -1.75f, -1.75f, -1.75f,

        -1.75f, -1.75f,  1.75f,
         1.75f, -1.75f,  1.75f,
         0.4f,  0.4f,  0.4f,
         0.4f,  0.4f,  0.4f,
        -0.4f,  0.4f,  0.4f,
        -1.75f, -1.75f, 1.75f,

        -0.4f,  0.4f,  0.4f,
        -0.4f,  0.4f, -0.4f,
        -1.75f, -1.75f, -1.75f,
        -1.75f, -1.75f, -1.75f,
        -1.75f, -1.75f, 1.75f,
        -0.4f,  0.4f,  0.4f,

         0.4f,  0.4f,  0.4f,
         0.4f,  0.4f, -0.4f,
         1.75f, -1.75f, -1.75f,
         1.75f, -1.75f, -1.75f,
         1.75f, -1.75f,  1.75f,
         0.4f,  0.4f,  0.4f,

         // abajo
        -1.75f, -1.75f, -1.75f,
         1.75f, -1.75f, -1.75f,
         1.75f, -1.75f,  1.75f,
         1.75f, -1.75f,  1.75f,
        -1.75f, -1.75f,  1.75f,
        -1.75f, -1.75f, -1.75f,

        // arriba
        -0.4f,  0.4f, -0.4f,
         0.4f,  0.4f, -0.4f,
         0.4f,  0.4f,  0.4f,
         0.4f,  0.4f,  0.4f,
        -0.4f,  0.4f,  0.4f,
        -0.4f,  0.4f, -0.4f,
    };

    float verticesAsientos[]{
        -0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.8f,  0.8f, -0.8f,
         0.8f,  0.8f, -0.8f,
        -0.8f,  0.8f, -0.8f,
        -0.3f, -0.3f, -0.3f,

        -0.3f, -0.3f,  0.3f,
         0.3f, -0.3f,  0.3f,
         0.8f,  0.8f,  0.8f,
         0.8f,  0.8f,  0.8f,
        -0.8f,  0.8f,  0.8f,
        -0.3f, -0.3f, 0.3f,

        -0.8f,  0.8f,  0.8f,
        -0.8f,  0.8f, -0.8f,
        -0.3f, -0.3f, -0.3f,
        -0.3f, -0.3f, -0.3f,
        -0.3f, -0.3f, 0.3f,
        -0.8f,  0.8f,  0.8f,

         0.8f,  0.8f,  0.8f,
         0.8f,  0.8f, -0.8f,
         0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f, -0.3f,  0.3f,
         0.8f,  0.8f,  0.8f,

         // abajo
        -0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f, -0.3f,  0.3f,
         0.3f, -0.3f,  0.3f,
        -0.3f, -0.3f,  0.3f,
        -0.3f, -0.3f, -0.3f,

        // arriba
        -0.8f,  0.8f, -0.8f,
         0.8f,  0.8f, -0.8f,
         0.8f,  0.8f,  0.8f,
         0.8f,  0.8f,  0.8f,
        -0.8f,  0.8f,  0.8f,
        -0.8f,  0.8f, -0.8f,
    };

    float verticesZanahorias1[] = {
        -1.5f, -0.2f, -0.2f,
         1.5f, -0.2f, -0.2f,
         1.5f,  0.2f, -0.2f,
         1.5f,  0.2f, -0.2f,
        -1.5f,  0.2f, -0.2f,
        -1.5f, -0.2f, -0.2f,

        -1.5f, -0.2f,  0.2f,
         1.5f, -0.2f,  0.2f,
         1.5f,  0.2f,  0.2f,
         1.5f,  0.2f,  0.2f,
        -1.5f,  0.2f,  0.2f,
        -1.5f, -0.2f,  0.2f,

        -0.2f,  0.2f,  0.2f,
        -0.2f,  0.2f, -0.2f,
        -0.2f, -0.2f, -0.2f,
        -0.2f, -0.2f, -0.2f,
        -0.2f, -0.2f,  0.2f,
        -0.2f,  0.2f,  0.2f,

         0.2f,  0.2f,  0.2f,
         0.2f,  0.2f, -0.2f,
         0.2f, -0.2f, -0.2f,
         0.2f, -0.2f, -0.2f,
         0.2f, -0.2f,  0.2f,
         0.2f,  0.2f,  0.2f,

         // abajo
        -1.5f, -0.2f, -0.2f,
         1.5f, -0.2f, -0.2f,
         1.5f, -0.2f,  0.2f,
         1.5f, -0.2f,  0.2f,
        -1.5f, -0.2f,  0.2f,
        -1.5f, -0.2f, -0.2f,

        // arriba
        -1.5f,  0.2f, -0.2f,
         1.5f,  0.2f, -0.2f,
         1.5f,  0.2f,  0.2f,
         1.5f,  0.2f,  0.2f,
        -1.5f,  0.2f,  0.2f,
        -1.5f,  0.2f, -0.2f,
    };

    float verticesCentro[]{
        -0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f,  0.3f, -0.3f,
         0.3f,  0.3f, -0.3f,
        -0.3f,  0.3f, -0.3f,
        -0.3f, -0.3f, -0.3f,

        -0.3f, -0.3f,  0.3f,
         0.3f, -0.3f,  0.3f,
         0.3f,  0.3f,  0.3f,
         0.3f,  0.3f,  0.3f,
        -0.3f,  0.3f,  0.3f,
        -0.3f, -0.3f,  0.3f,

        -0.3f,  0.3f,  0.3f,
        -0.3f,  0.3f, -0.3f,
        -0.3f, -0.3f, -0.3f,
        -0.3f, -0.3f, -0.3f,
        -0.3f, -0.3f,  0.3f,
        -0.3f,  0.3f,  0.3f,

         0.3f,  0.3f,  0.3f,
         0.3f,  0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f, -0.3f,  0.3f,
         0.3f,  0.3f,  0.3f,

         // abajo
        -0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f, -0.3f,  0.3f,
         0.3f, -0.3f,  0.3f,
        -0.3f, -0.3f,  0.3f,
        -0.3f, -0.3f, -0.3f,

        // arriba
        -0.3f,  0.3f, -0.3f,
         0.3f,  0.3f, -0.3f,
         0.3f,  0.3f,  0.3f,
         0.3f,  0.3f,  0.3f,
        -0.3f,  0.3f,  0.3f,
        -0.3f,  0.3f, -0.3f,
    };

    // Piso
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSuelo), verticesSuelo, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // RANAS
    // 1
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 3
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4
    unsigned int VBO4, VAO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRanas), verticesRanas, GL_STATIC_DRAW);
    glBindVertexArray(VAO4);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ESQUINAS
    // 1
    unsigned int VBO5, VAO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO5);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2
    unsigned int VBO6, VAO6;
    glGenVertexArrays(1, &VAO6);
    glGenBuffers(1, &VBO6);
    glBindBuffer(GL_ARRAY_BUFFER, VBO6);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO6);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 3
    unsigned int VBO7, VAO7;
    glGenVertexArrays(1, &VAO7);
    glGenBuffers(1, &VBO7);
    glBindBuffer(GL_ARRAY_BUFFER, VBO7);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO7);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4
    unsigned int VBO8, VAO8;
    glGenVertexArrays(1, &VAO8);
    glGenBuffers(1, &VBO8);
    glBindBuffer(GL_ARRAY_BUFFER, VBO8);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEsquinas), verticesEsquinas, GL_STATIC_DRAW);
    glBindVertexArray(VAO8);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ASIENTOS
    // 1
    unsigned int VBO9, VAO9;
    glGenVertexArrays(1, &VAO9);
    glGenBuffers(1, &VBO9);
    glBindBuffer(GL_ARRAY_BUFFER, VBO9);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO9);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2
    unsigned int VBO10, VAO10;
    glGenVertexArrays(1, &VAO10);
    glGenBuffers(1, &VBO10);
    glBindBuffer(GL_ARRAY_BUFFER, VBO10);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO10);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 3
    unsigned int VBO11, VAO11;
    glGenVertexArrays(1, &VAO11);
    glGenBuffers(1, &VBO11);
    glBindBuffer(GL_ARRAY_BUFFER, VBO11);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO11);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 4
    unsigned int VBO12, VAO12;
    glGenVertexArrays(1, &VAO12);
    glGenBuffers(1, &VBO12);
    glBindBuffer(GL_ARRAY_BUFFER, VBO12);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAsientos), verticesAsientos, GL_STATIC_DRAW);
    glBindVertexArray(VAO12);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ZANAHORIAS
    // 1
    unsigned int VBO13, VAO13;
    glGenVertexArrays(1, &VAO13);
    glGenBuffers(1, &VBO13);
    glBindBuffer(GL_ARRAY_BUFFER, VBO13);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias1), verticesZanahorias1, GL_STATIC_DRAW);
    glBindVertexArray(VAO13);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2
    unsigned int VBO14, VAO14;
    glGenVertexArrays(1, &VAO14);
    glGenBuffers(1, &VBO14);
    glBindBuffer(GL_ARRAY_BUFFER, VBO14);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias1), verticesZanahorias1, GL_STATIC_DRAW);
    glBindVertexArray(VAO14);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //// 3
    //unsigned int VBO15, VAO15;
    //glGenVertexArrays(1, &VAO15);
    //glGenBuffers(1, &VBO15);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO15);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias2), verticesZanahorias2, GL_STATIC_DRAW);
    //glBindVertexArray(VAO15);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// 4
    //unsigned int VBO16, VAO16;
    //glGenVertexArrays(1, &VAO16);
    //glGenBuffers(1, &VBO16);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO16);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZanahorias2), verticesZanahorias2, GL_STATIC_DRAW);
    //glBindVertexArray(VAO16);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

     // Centro
    unsigned int VBO17, VAO17;
    glGenVertexArrays(1, &VAO17);
    glGenBuffers(1, &VBO17);
    glBindBuffer(GL_ARRAY_BUFFER, VBO17);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCentro), verticesCentro, GL_STATIC_DRAW);
    glBindVertexArray(VAO17);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
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

        // PISO
        // Activate shader when setting uniforms/drawing objects
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
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // RANAS
        // 1
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.5f, 0.8f, 0.8f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posRana1);
        lightingShader.setMat4("model", model);
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
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO4);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ESQUINAS
        // 1
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.37f, 0.39f, 0.38f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posEsquina1);
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
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO8);
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
        lightingShader.setMat4("model", model);
        glBindVertexArray(VAO14);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //// 3
        //lightingShader.use();
        //lightingShader.setVec3("objectColor", 0.9f, 0.5f, 0.1f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //lightingShader.setMat4("projection", projection);
        //lightingShader.setMat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, posZanahoria3);
        //lightingShader.setMat4("model", model);
        //glBindVertexArray(VAO15);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //// 4
        //lightingShader.use();
        //lightingShader.setVec3("objectColor", 0.9f, 0.5f, 0.1f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //lightingShader.setMat4("projection", projection);
        //lightingShader.setMat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, posZanahoria4);
        //lightingShader.setMat4("model", model);
        //glBindVertexArray(VAO16);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // CENTRO
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.3f, 0.5f, 0.9f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCentro);
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
    glDeleteVertexArrays(1, &VAO);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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