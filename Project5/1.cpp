#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
//openGL可以理解为一些API但实际上是一些不包含实现的规范，OpenGL程序本身是一个状态（或者说上下文context），形式上像一个结构体
//我们编程就是去调整这个状态去做渲染工作
//工作流：创建对象-》id保存其引用-》绑定这个对象到上下文目标位置-》通过操作对象来设置目标位置的一些选项，同时这个对象也会记录选项信息-》解绑对象与重新绑定
//你好三角形
//一般采用 glad 获取一堆 glxxx 函数的函数指针。用 glfw 管理操作系统的窗口管理器给到的 framebuffer，然后OpenGL 在上面画画
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
    "layout (location = 0) in vec3 aPos;\n"//定义了顶点属性的位置值：0
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
    /*我们需要告诉GLFW我们要使用的OpenGL版本是3.3，
    这样GLFW会在创建OpenGL上下文时做出适当的调整。
    这也可以确保用户在没有适当的OpenGL版本支持的情况下无法运行。
    我们将主版本号(Major)和次版本号(Minor)都设为3。
    我们同样明确告诉GLFW我们使用的是核心模式(Core - profile)。
    明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集（没有我们已不再需要的向后兼容特性）。*/
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    /*通过这个函数创建一个窗口对象（函数返回的就是一个指向GLFWwindow类型的对象的指针）
    前三个参数为长，宽，标题*/
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//让GLFW将我们窗口的上下文设置为当前线程的主上下文
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //核心逻辑：把我们自定义的东西（数据或者shader）设成对象，送到显存里面存好，同时设置好GPU和显存的通信方式，然后给GPU下绘制指令，它自然就会去显存里面找需要用什么数据（属性）以及什么shader
    //第一步：shader
    unsigned int vertexShader;//创建顶点着色器变量
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//将这个变量变成一个对象
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//指定这个着色器对象的shader代码
    glCompileShader(vertexShader);//编译这段shader代码

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);//对片元着色器进行同样操作

    unsigned int shaderProgram;//指定着色器程序：它是由多个着色器合并然后链接完成的东西，也是opengl最终绘制的时候用的
    shaderProgram = glCreateProgram();//初始化
    glAttachShader(shaderProgram, vertexShader);//绑定我们事先编译好的两个shader
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);//绑定完了要链接（就像你写完代码要先编译链接才能执行）
    
    //至此，GPU已经知道如何对顶点数据进行着色。着色方法存在shaderprogram对象中

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //new完对象之后我们要delete掉没用的对象，现在shader program已经完全替代了这俩
    //第二步：vertex attribute
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//先绑定VAO！！！这是之后画画时要用的唯二要用的自定义对象之一（另一个是shader program)
    //为什么画画不用VBO EBO这两个自定义对象？因为它们只是用来辅助修改GL_ARRAY_BUFFER和GL_ELEMENT_ARRAY_BUFFER这两个系统内部变量的
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//将VBO对象指定类型（VBO它是个array buffer类型的对象所以只能给它绑GL_ARRAY_BUFFER）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//将用户定义的数据复制到当前绑定的缓冲上

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //至此，这个顶点数据就已经存在显存里了，并且有一个叫VBO的对象管理着它，这个对象的数据类型是GL_ARRAY_BUFFER
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //第一个参数指定我们要配置的顶点属性（也就是位置），顶点着色器shader代码中已经规定了是0；
    //第二个参数指定顶点属性的大小，是一个vec3所以传入3
    //第三个参数是数据类型 vec*里默认都是float
    //第四个参数定义我们是否希望数据被标准化到-1,1之间
    //第五个参数步长告诉我们在连续的顶点属性组之间的间隔，第五个第六个参数略
    glEnableVertexAttribArray(0);//参数是顶点属性位置值，作用是启用上述配置好的顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, 0);//先解绑VBO再解绑VAO
    glBindVertexArray(0);//这两句是对VAO VBO解除绑定，因为当前它们的任务已经完成，它们内部已经存好了各项信息
    //注意不要在解绑VAO之前解绑EBO，否则VAO拿不到EBO的内容

    glViewport(0, 0, 800, 600);//通过调用glViewport函数来设置窗口的维度也就是定义视口
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册回调函数
    while (!glfwWindowShouldClose(window))//函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了
    {
        processInput(window);
        //下面两行是渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClear(GL_COLOR_BUFFER_BIT);//通过调用glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
        //
        glUseProgram(shaderProgram);//激活这个shader program对象
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        //对于drawarrays它使用已经激活的shader program、已经enable的顶点属性配置以及用VAO间接绑定的VBO顶点数据画出指定图元
        //第一个参数是图元类型，第二个是顶点数组的起始索引，第三个是要画多少个顶点
        //对于drawelements，它不再从vertexbuffer中拿数据进行绘制，而是从element array buffer中取数据（索引）
        glfwSwapBuffers(window);//函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        glfwPollEvents();
        //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);//释放资源
    /*
    双缓冲(Double Buffer)
    应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 
    这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。
    最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。
    为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；
    而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。*/
    glfwTerminate();//释放/删除之前的分配的所有资源
    return 0;
}