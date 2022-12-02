#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shader_s.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include <iostream>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"

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
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 45.0f;

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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("Shaders/7.3.camera.vs", "Shaders/7.3.camera.fs");
    // Una luz
    Shader lightingShader("Shaders/2.2.basic_lighting.vs", "Shaders/2.2.basic_lighting.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //float vertices[] = {
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    //};
    //// world space positions of our cubes
    //glm::vec3 cubePositions[] = {
    //    glm::vec3(0.0f,  0.0f,  0.0f),
    //    glm::vec3(2.0f,  5.0f, -15.0f),
    //    glm::vec3(-1.5f, -2.2f, -2.5f),
    //    glm::vec3(-3.8f, -2.0f, -12.3f),
    //    glm::vec3(2.4f, -0.4f, -3.5f),
    //    glm::vec3(-1.7f,  3.0f, -7.5f),
    //    glm::vec3(1.3f, -2.0f, -2.5f),
    //    glm::vec3(1.5f,  2.0f, -2.5f),
    //    glm::vec3(1.5f,  0.2f, -1.5f),
    //    glm::vec3(-1.3f,  1.0f, -1.5f)
    //};
    //unsigned int VBO, VAO;
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// texture coord attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);


    //// load and create a texture 
    //// -------------------------
    //unsigned int texture;
    //// texture 1
    //// ---------
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //// set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load image, create texture and generate mipmaps
    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //unsigned char* data = stbi_load("C:/Users/MGUTIERRZA/Desktop/ProyectoGraficas/P2/Visualizacion3D/Dependencies/Imagenes/baldosa.jpg", &width, &height, &nrChannels, 0);    if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);


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


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture", 0);

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

        //// bind textures on corresponding texture units
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture);

        //// activate shader
        //ourShader.use();

        //// pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //// camera/view transformation
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        //// render boxes
        //glBindVertexArray(VAO);
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        //    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        glm::mat4 model;

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // Shader Teselaci�n
        lightingShader.use();
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        lightingShader.setVec3("objectColor", 0.87f, 0.734f, 0.082f);
        lightingShader.setFloat("alpha", 0.5); //Para que sea trasl�cido

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
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void reflect(unsigned int vao, int toDraw, Shader ourShader, glm::mat4 transform) {

    //glm::mat4 transform = glm::mat4(1.0f);
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
