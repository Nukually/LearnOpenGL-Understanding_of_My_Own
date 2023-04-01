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

    //����Ҫ��Ⱦһ�����Σ��������ĸ����㣬ÿ����������Դ�ǰ�����Ϊ���飬�ֱ���λ�ã���ɫ����������
    float vertices[] = {

         0.5f,0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f,1.0f,
         0.5f,-0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
         -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,0.0f,
         -0.5f,0.5f,0.0f,   1.0f,1.0f,0.0f,    0.0f,1.0f
    };
    //Ҫָ���⼸������Ҫ������ӡ�����Ϊ0,1,3��������������һ�������Σ�����Ϊ1,2,3��������һ��
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };
    //�����ǹ�����������
    //����VBO��VAO��ʡ�˻��ƶ������ʱ CPU �� GPU ֮���ͨѶʱ�䣬�������Ⱦ���ܣ���������������Ǹ���������ġ�
    unsigned int VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    //���洴���������
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //���ȣ���VAO���Ը�֪OpenGL�����ʹ�����VAO����VBO�������͡�
    glBindVertexArray(VAO);
    //�󶨵�һ��VBO�������VBO��д�����ݣ���֪VAO����ν������VBO����Ϣ��Ȼ�󣬽�����VBO��glBufferData����������������ռ��Լ��洢���ݣ�����������vertices��������ݾͿ��Դ���VBO��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glVertexAttribPointer����;�ǽ�������Դ���VAO�����enable�����ͬʱÿ�����Ե�����Ҳ�����£�Ҳ�������ĵ�0,1,2��
    //ֱ��VAO�����ԣ��Կ�GPU����֪����ղ�д��vertices��������ݷֱ��Ӧʲô���ԣ�����Ϊһ��
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
    //��GL_TEXTURE_2D��ȥ
    glBindTexture(GL_TEXTURE_2D, texture1);
    //�����Կ���λ���������˷�����
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;//��ɫͨ���ĸ���
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        //����һ�еĺ�����������һ��������һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
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
    //�����Կ���λ���������˷�����
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
        ourShader.use();//����������use��������Ҫ�Ǵ����˵���glUseProgram(shaderProgram)�������
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //����Ҳ����������delete�Ǹ�shader program����
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