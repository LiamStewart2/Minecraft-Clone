#include "Application.h"

#include <GLFW/glfw3.h>

void cursorFunction(GLFWwindow* window, double xposIn, double yposIn);
void scrollFunction(GLFWwindow* window, double xoffset, double yoffset);

Application app = Application();

int main(void)
{
    app.setFunctions(cursorFunction, scrollFunction);
    app.mainloop();
    return 0;
}

void cursorFunction(GLFWwindow* window, double xposIn, double yposIn)
{
    app.mouse_callback(window, xposIn, yposIn);
}
void scrollFunction(GLFWwindow* window, double xoffset, double yoffset)
{
    app.scroll_callback(window, xoffset, yoffset);
}