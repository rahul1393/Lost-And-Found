#include "Window.h"

Window::Window()
{
	width = 800;					//Height and Width of Window
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;					//Initialising keys to false as none is pressed 
	}

	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;					//Initialising keys to false as none is pressed 
	}

	xChange = 0.0f;
	yChange = 0.0f;
}

int Window::Initialise()
{
	//Initialization of GLFW

	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Windows Properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	//Get Buffer size information 
	//we want the dimensions of the area of the middle of the window which is the buffer 
	//as this part holds the opengl data as it is being parsed to the window and 
	//thats what we will be using as the size of the OpenGL viewport the actual bit which is drawn\
	//Below function returns the size of the buffer
	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set the context for GLEW to use 
	//Let GLEW know that the Window we created is the one on which the OpenGL context should be tied to 
	//so everything which is drawn is drawn to this window
	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//Locks the cursor to the screen and cursor disappears 

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	//Initialize GLEW 
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);				//Sets the active user and window for key callbacks
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);				//Setting callback for key press, basically it means if the key is pressed on main window call handleKeys function
	glfwSetCursorPosCallback(mainWindow, handleMouse);		// Same as above for mouse movement
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));			//

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
