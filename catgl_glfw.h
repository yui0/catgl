//---------------------------------------------------------
//	catgl
//
//		©2015 Yuichiro Nakada
//---------------------------------------------------------

#include <GLFW/glfw3.h>

extern void caInit(int width, int height);
extern void caRender();
extern void caEnd();

int main()
{
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(640, 480, "Catgl example", /*nullptr*/0, /*nullptr*/0);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	caInit(640, 480);
	while (!glfwWindowShouldClose(window)) {
		caRender();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	caEnd();
	glfwTerminate();
}
