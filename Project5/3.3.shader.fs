#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
uniform float someColor;
void main()
{
    FragColor = vec4(ourColor, someColor);
}