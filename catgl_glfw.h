//---------------------------------------------------------
//	catgl
//
//		©2015,2018 Yuichiro Nakada
//---------------------------------------------------------

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#ifndef SCREEN_WIDTH
#define SCREEN_TITLE	"Catgl"
#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720
#endif

extern void caInit(int width, int height);
extern void caRender();
extern void caEnd();

#include <string.h>
char *caGetPath(char *path)
{
	static char s[BUFSIZ];
	strncpy(s, "assets/", BUFSIZ);
	strncat(s, path, BUFSIZ);
	return s;
}

int _button/*, _action*/;
void mouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods)
{
//	int xpos, ypos;
//	glfwGetMousePos(&xpos, &ypos);
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (caMouseEvent) {
		_button = button;
//		_action = action;
		caMouseEvent(button, action, xpos, ypos);
	}
}
void mouseMoveCallback(GLFWwindow* window, const double xpos, const double ypos)
{
	if (caMouseEvent) caMouseEvent(_button, /*_action*/2, xpos, ypos);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (caKeyEvent) {
		caKeyEvent(key, action);
	} //else {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
//	}
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	//glfwWindowHint(GLFW_DECORATED, /*false*/0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);*/
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, 0, 0);
	if (!window) {
		//printf("Error at glfwCreateWindow!\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetKeyCallback(window, keyCallback);
	caInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (!glfwWindowShouldClose(window)) {
		caRender();

		glfwSwapBuffers(window);
		glfwPollEvents();

		/*if (glfwGetKey(window, GLFW_KEY_P)) caMode = CATGL_MODE_POINT;
		if (glfwGetKey(window, GLFW_KEY_W)) caMode = CATGL_MODE_LINE;
		if (glfwGetKey(window, GLFW_KEY_T)) caMode = CATGL_MODE_TRIANGLES;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) break;*/
	}
	caEnd();
	glfwTerminate();
}
