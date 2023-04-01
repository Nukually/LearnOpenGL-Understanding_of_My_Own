#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"stb_image.h"
#include "shader_s.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

    //我们要渲染一个矩形，矩形有四个顶点，每个顶点的属性从前到后分为三组，分别是位置，颜色和纹理坐标
    float vertices[] = {

         0.5f,0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f,1.0f,
         0.5f,-0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
         -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,0.0f,
         -0.5f,0.5f,0.0f,   1.0f,1.0f,0.0f,    0.0f,1.0f
    };
    //要指明这几个顶点要如何连接。索引为0,1,3的三个顶点连成一个三角形，索引为1,2,3的连成另一个
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };
    //下面是规则属性配置
    //采用VBO、VAO节省了绘制多个物体时 CPU 与 GPU 之间的通讯时间，提高了渲染性能，缓冲区大多数都是干这个工作的。
    unsigned int VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    //下面创建缓冲对象
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //首先，绑定VAO，以告知OpenGL程序该使用这个VAO来对VBO做出解释。
    glBindVertexArray(VAO);
    //绑定第一个VBO，向这个VBO中写入数据，告知VAO该如何解释这个VBO的信息；然后，解绑这个VBO。glBufferData用来给缓冲对象分配空间以及存储数据，这样我们在vertices里面的数据就可以存入VBO了
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glVertexAttribPointer的用途是将这个属性存入VAO并激活（enable激活），同时每个属性的索引也会留下（也就是下文的0,1,2）
    //直到VAO，电脑（显卡GPU）才知道你刚才写在vertices里面的数据分别对应什么属性，几个为一组
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    unsigned int texture1;
    glGenTextures(1, &texture1);
    //绑到GL_TEXTURE_2D上去
    glBindTexture(GL_TEXTURE_2D, texture1);
    //告诉显卡如何画（纹理过滤方法）
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;//颜色通道的个数
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        //下面一行的函数用来生成一张纹理。第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "fail to load picture" << std::endl;
    }
    stbi_image_free(data);
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //告诉显卡如何画（纹理过滤方法）
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texturel"), 0);
    ourShader.setInt("texture2", 1);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        //float offset = 0.5f;
        //ourShader.setFloat("xOffset", offset);
        ourShader.use();//调用这个类的use方法，主要是代替了调用glUseProgram(shaderProgram)这个函数
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //这里也不用再自行delete那个shader program对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}