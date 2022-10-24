#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shader_s.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Transformaciones", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader shader1("shader1.vs", "shaderTira.fs");
    Shader shader2("shader1.vs", "shaderAsiento.fs");
    Shader shader3("shader1.vs", "shaderRana.fs");
    Shader shader4("shader1.vs", "shaderZanahoria.fs");
    Shader shader5("shader1.vs", "shaderCentro.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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
    
    unsigned int VBOs[100], VAOs[100], EBO[100];
    glGenVertexArrays(100, VAOs); 
    glGenBuffers(100, VBOs);
    glGenBuffers(100, EBO);

    // Tira
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTiras), verticesTiras, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Asiento
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCirculo1), verticesCirculo1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Rana 
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indicesRanas), indicesRanas, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Zanahoria 
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indicesZanahorias), indicesZanahorias, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //CIRCULOS
    // 1 
    glBindVertexArray(VAOs[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indicesCentro1), indicesCentro1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // 2 
    glBindVertexArray(VAOs[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indicesCentro2), indicesCentro2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 0.8f, 1.0f); //Fondo color quartz
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // first container
        // ---------------
        //TIRAS
            shader1.use();
            shader1.setInt("texture", 0);
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            // get their uniform location and set matrix (using glm::value_ptr)
            unsigned int transformLoc = glGetUniformLocation(shader1.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            // with the uniform matrix set, draw the first container
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 46);

            // 2nd transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value

            // now with the uniform matrix being replaced with new transformations, draw it again.
            glDrawArrays(GL_TRIANGLE_FAN, 0, 46);

            // 3th transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 46);

            // 4th transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 46);

        //ASIENTO
            shader2.use();
            shader2.setInt("texture2", 1);
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            // get their uniform location and set matrix (using glm::value_ptr)
            transformLoc = glGetUniformLocation(shader2.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 40);

            // 2nd transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 40);


            // 3th transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 40);

            // 4th transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 40);

        //RANAS
            shader3.use();
            shader3.setInt("texture3", 2);
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            // get their uniform location and set matrix (using glm::value_ptr)
            transformLoc = glGetUniformLocation(shader3.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

            // 2nd transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

            // 3th transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

            // 4th transformation
            // ---------------------
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 30);
        
         //ZANAHORIAS
            shader4.use();
            shader4.setInt("texture3", 2);
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transformLoc = glGetUniformLocation(shader4.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindVertexArray(VAOs[3]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 27);

            //2nd
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 27);            
           
            // 3th
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 27);
            
            // 4th
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transform = glm::rotate(transform, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value
            glDrawArrays(GL_TRIANGLE_FAN, 0, 27);


        //Centro
            shader5.use();
            shader5.setInt("texture3", 2);
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transformLoc = glGetUniformLocation(shader5.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindVertexArray(VAOs[4]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

            shader2.use();
            shader2.setInt("texture3", 2);
            transform = glm::mat4(1.0f); // reset it to identity matrix
            transformLoc = glGetUniformLocation(shader2.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindVertexArray(VAOs[5]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 34);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteBuffers(1, EBO);

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