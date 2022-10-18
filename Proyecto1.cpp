// Axel Giuseppe Flores Aranda
// CU: 181218

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shader_s.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Para transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Dark cyan - Tiras
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.4f, 0.6f, 0.8f, 1.0f); \n"
"}\n\0";

// Cafe - Circulos
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.7f, 0.5f, 1.0f); \n"
"}\n\0";

// Light cyan/aqua - Ranas
const char* fragmentShader3Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.5f, 0.8f, 0.8f, 1.0f); \n"
"}\n\0";

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto 1", NULL, NULL);
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

    // build and compile our shader program
    //Shader ourShader("5.1.transform.vs", "5.1.transform.fs");

//================================================================================================================================================
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER); // fragment shader de las tiras alrededor de los circulos
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER); // fragment shader de los circulos
    unsigned int fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER); // fragment shader de las ranas (poligonos) 

//================================================================================================================================================
    unsigned int shaderProgram1 = glCreateProgram(); // tira
    unsigned int shaderProgram2 = glCreateProgram(); // circulo
    unsigned int shaderProgram3 = glCreateProgram(); // circulo

//================================================================================================================================================
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // tira
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader1);
    // circulo
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader2);
    // rana
    glShaderSource(fragmentShader3, 1, &fragmentShader3Source, NULL);
    glCompileShader(fragmentShader3);

    // Link de los objetos
//================================================================================================================================================
    // link de tiras
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    // link de circulos
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    // link de ranas
    glAttachShader(shaderProgram3, vertexShader);
    glAttachShader(shaderProgram3, fragmentShader3);
    glLinkProgram(shaderProgram3);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
//================================================================================================================================================
    // Tiras
    float verticesTiras[] = {
        0.4f, 0.4f, 0.0f, // centro del fan
        0.1f, 0.24f, 0.0f,
        0.085f, 0.225f, 0.0f,
        0.07f, 0.234f, 0.0f,
        0.065f, 0.24f, 0.0f,
        0.055f, 0.26f, 0.0f,
        0.05f, 0.28f, 0.0f,
        0.046f, 0.3f, 0.0f,
        0.044f, 0.32f, 0.0f,
        0.045f, 0.35f, 0.0f,
        0.05f, 0.4f, 0.0f,
        0.055f, 0.44f, 0.0f,
        0.067f, 0.49f, 0.0f,
        0.09f, 0.55f, 0.0f,
        0.15f, 0.65f, 0.0f,
        0.25f, 0.746f, 0.0f,
        0.3f, 0.773f, 0.0f,
        0.35f, 0.79f, 0.0f,
        0.4f, 0.8f, 0.0f,
        0.45f, 0.8f, 0.0f,
        0.5f, 0.795f, 0.0f,
        0.55f, 0.783f, 0.0f,
        0.6f, 0.757f, 0.0f,
        0.66f, 0.72f, 0.0f, // mitad 23
        0.757f, 0.6f, 0.0f,
        0.783f, 0.55f, 0.0f,
        0.795f, 0.5f, 0.0f,
        0.8f, 0.45f, 0.0f,
        0.8f, 0.4f, 0.0f,
        0.79f, 0.35f, 0.0f,
        0.773f, 0.3f, 0.0f,
        0.746f, 0.25f, 0.0f,
        0.65f, 0.15f, 0.0f,
        0.55f, 0.09f, 0.0f,
        0.49f, 0.067f, 0.0f,
        0.44f, 0.055f, 0.0f,
        0.4f, 0.05f, 0.0f,
        0.35f, 0.045f, 0.0f,
        0.32f, 0.044f, 0.0f,
        0.3f, 0.046f, 0.0f,
        0.28f, 0.05f, 0.0f,
        0.26f, 0.055f, 0.0f,
        0.24f, 0.065f, 0.0f,
        0.234f, 0.07f, 0.0f,
        0.225f, 0.085f, 0.0f,
        0.24f, 0.1f, 0.0f 
    };

    // Circulos
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

    // Ranas
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

    unsigned int VBOs[100], VAOs[100], EBO[100];
    glGenVertexArrays(100, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(100, VBOs);
    glGenBuffers(100, EBO);

//================================================================================================================================================
    // Tira 1
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTiras), verticesTiras, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Circulo 1
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCirculo1), verticesCirculo1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Rana 1
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indicesRanas), indicesRanas, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 0.8f, 1.0f); //Fondo color quartz
        glClear(GL_COLOR_BUFFER_BIT);

//================================================================================================================================================
        // Tira 1
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 46);

        // Circulo 1
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 40);

        // Rana 1
        glUseProgram(shaderProgram3);
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBO);
    glDeleteProgram(shaderProgram1);


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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
