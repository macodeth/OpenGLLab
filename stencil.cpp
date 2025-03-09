#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"

#include<iostream>
#include<algorithm>
#include<cstdint>

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void move_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void updateDelta();
unsigned int loadTexture(const char* path);
float clamp(float up, float down, float val);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));

glm::vec3 CubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader objectShader("stencil_shader.vert", "stencil_shader.frag");
    Shader outlineShader("stencil_outline_shader.vert", "stencil_outline_shader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // position           texture coordinates
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    unsigned stride = 5 * sizeof(float);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // texture coordinate attibute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    unsigned int image = loadTexture("container.jpg");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        updateDelta();
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindVertexArray(VAO);
        

        objectShader.use();
        objectShader.setInt("image", 0);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image);

        // render cube objects
        glBindVertexArray(VAO);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, CubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i),
                glm::vec3(1.0f, 0.3f, 0.5f));
            objectShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / stride);
        }

        // render outlines
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        outlineShader.use();
        outlineShader.setMat4("view", view);
        outlineShader.setMat4("projection", projection);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, CubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i),
                glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
            outlineShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / stride);
        }

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
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
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float clamp(float up, float down, float val) {
    if (val > up) return up;
    if (val < down) return down;
    return val;
}

void updateDelta() {
    float time = glfwGetTime();
    deltaTime = time - lastFrame;
    lastFrame = time;
}

void move_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        firstMouse = true;
        return;
    }
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.ProcessMouseScroll(static_cast<float>(ypos));
}

unsigned int loadTexture(const char* path) {
    // initialize texture object
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);     // all upcoming GL_TEXTURE_2D operations have effect on this object
    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    GLenum format = GL_RGBA;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return textureId;
}