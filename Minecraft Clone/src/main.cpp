#include "Application.h"

#include <GLFW/glfw3.h>

void cursorFunction(GLFWwindow* window, double xposIn, double yposIn);

Application app = Application();

int main(void)
{
    app.setFunctions(cursorFunction);
    app.mainloop();
    return 0;
}

void cursorFunction(GLFWwindow* window, double xposIn, double yposIn)
{
    app.mouse_callback(window, xposIn, yposIn);
}