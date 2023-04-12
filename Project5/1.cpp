#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
//openGL�������ΪһЩAPI��ʵ������һЩ������ʵ�ֵĹ淶��OpenGL��������һ��״̬������˵������context������ʽ����һ���ṹ��
//���Ǳ�̾���ȥ�������״̬ȥ����Ⱦ����
//����������������-��id����������-�����������������Ŀ��λ��-��ͨ����������������Ŀ��λ�õ�һЩѡ�ͬʱ�������Ҳ���¼ѡ����Ϣ-�������������°�
//���������
//һ����� glad ��ȡһ�� glxxx �����ĺ���ָ�롣�� glfw �������ϵͳ�Ĵ��ڹ����������� framebuffer��Ȼ��OpenGL �����滭��
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     -0.5f,0.5f,0.0f
};
unsigned int indices[] = {
    0,1,3,
    1,2,3
};
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"//�����˶������Ե�λ��ֵ��0
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /*������Ҫ����GLFW����Ҫʹ�õ�OpenGL�汾��3.3��
    ����GLFW���ڴ���OpenGL������ʱ�����ʵ��ĵ�����
    ��Ҳ����ȷ���û���û���ʵ���OpenGL�汾֧�ֵ�������޷����С�
    ���ǽ����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3��
    ����ͬ����ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core - profile)��
    ��ȷ����GLFW������Ҫʹ�ú���ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ���û�������Ѳ�����Ҫ�����������ԣ���*/
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    /*ͨ�������������һ�����ڶ��󣨺������صľ���һ��ָ��GLFWwindow���͵Ķ����ָ�룩
    ǰ��������Ϊ����������*/
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//��GLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //�����߼����������Զ���Ķ��������ݻ���shader����ɶ����͵��Դ������ã�ͬʱ���ú�GPU���Դ��ͨ�ŷ�ʽ��Ȼ���GPU�»���ָ�����Ȼ�ͻ�ȥ�Դ���������Ҫ��ʲô���ݣ����ԣ��Լ�ʲôshader
    //��һ����shader
    unsigned int vertexShader;//����������ɫ������
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//������������һ������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//ָ�������ɫ�������shader����
    glCompileShader(vertexShader);//�������shader����

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);//��ƬԪ��ɫ������ͬ������

    unsigned int shaderProgram;//ָ����ɫ�����������ɶ����ɫ���ϲ�Ȼ��������ɵĶ�����Ҳ��opengl���ջ��Ƶ�ʱ���õ�
    shaderProgram = glCreateProgram();//��ʼ��
    glAttachShader(shaderProgram, vertexShader);//���������ȱ���õ�����shader
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);//������Ҫ���ӣ�������д�����Ҫ�ȱ������Ӳ���ִ�У�
    
    //���ˣ�GPU�Ѿ�֪����ζԶ������ݽ�����ɫ����ɫ��������shaderprogram������

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //new�����֮������Ҫdelete��û�õĶ�������shader program�Ѿ���ȫ���������
    //�ڶ�����vertex attribute
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//�Ȱ�VAO����������֮�󻭻�ʱҪ�õ�Ψ��Ҫ�õ��Զ������֮һ����һ����shader program)
    //Ϊʲô��������VBO EBO�������Զ��������Ϊ����ֻ�����������޸�GL_ARRAY_BUFFER��GL_ELEMENT_ARRAY_BUFFER������ϵͳ�ڲ�������
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//��VBO����ָ�����ͣ�VBO���Ǹ�array buffer���͵Ķ�������ֻ�ܸ�����GL_ARRAY_BUFFER��
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//���û���������ݸ��Ƶ���ǰ�󶨵Ļ�����

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //���ˣ�����������ݾ��Ѿ������Դ����ˣ�������һ����VBO�Ķ������������������������������GL_ARRAY_BUFFER
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //��һ������ָ������Ҫ���õĶ������ԣ�Ҳ����λ�ã���������ɫ��shader�������Ѿ��涨����0��
    //�ڶ�������ָ���������ԵĴ�С����һ��vec3���Դ���3
    //�������������������� vec*��Ĭ�϶���float
    //���ĸ��������������Ƿ�ϣ�����ݱ���׼����-1,1֮��
    //����������������������������Ķ���������֮��ļ���������������������
    glEnableVertexAttribArray(0);//�����Ƕ�������λ��ֵ�������������������úõĶ�������
    glBindBuffer(GL_ARRAY_BUFFER, 0);//�Ƚ��VBO�ٽ��VAO
    glBindVertexArray(0);//�������Ƕ�VAO VBO����󶨣���Ϊ��ǰ���ǵ������Ѿ���ɣ������ڲ��Ѿ�����˸�����Ϣ
    //ע�ⲻҪ�ڽ��VAO֮ǰ���EBO������VAO�ò���EBO������

    glViewport(0, 0, 800, 600);//ͨ������glViewport���������ô��ڵ�ά��Ҳ���Ƕ����ӿ�
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ע��ص�����
    while (!glfwWindowShouldClose(window))//����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó�����
    {
        processInput(window);
        //������������Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT);//ͨ������glClear�����������Ļ����ɫ���壬������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ���
        //
        glUseProgram(shaderProgram);//�������shader program����
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        //����drawarrays��ʹ���Ѿ������shader program���Ѿ�enable�Ķ������������Լ���VAO��Ӱ󶨵�VBO�������ݻ���ָ��ͼԪ
        //��һ��������ͼԪ���ͣ��ڶ����Ƕ����������ʼ��������������Ҫ�����ٸ�����
        //����drawelements�������ٴ�vertexbuffer�������ݽ��л��ƣ����Ǵ�element array buffer��ȡ���ݣ�������
        glfwSwapBuffers(window);//�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
        glfwPollEvents();
        //���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);//�ͷ���Դ
    /*
    ˫����(Double Buffer)
    Ӧ�ó���ʹ�õ������ͼʱ���ܻ����ͼ����˸�����⡣ 
    ������Ϊ���ɵ�ͼ����һ���ӱ����Ƴ����ģ����ǰ��մ����ң����϶��������صػ��ƶ��ɵġ�
    ����ͼ������˲����ʾ���û�������ͨ��һ��һ�����ɵģ���ᵼ����Ⱦ�Ľ���ܲ���ʵ��
    Ϊ�˹����Щ���⣬����Ӧ��˫������Ⱦ����Ӧ�ó���ǰ���屣�������������ͼ����������Ļ����ʾ��
    �����еĵ���Ⱦָ����ں󻺳��ϻ��ơ������е���Ⱦָ��ִ����Ϻ����ǽ���(Swap)ǰ����ͺ󻺳壬����ͼ����������Գ�����֮ǰ�ᵽ�Ĳ���ʵ�о������ˡ�*/
    glfwTerminate();//�ͷ�/ɾ��֮ǰ�ķ����������Դ
    return 0;
}