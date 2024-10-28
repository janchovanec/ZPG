#include "App.h"
#include "ShaderProgram.h"

#include "models.h"

App* App::instance = nullptr;

App::App() {
    initGLFW();
	initCallback();
    initGLEW();
	instance = this;
	scene = new Scene();
}

App::~App() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run() {
    glEnable(GL_DEPTH_TEST);
	auto lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
		auto newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;
        scene->render(deltaTime);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void App::initCallback() {
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double mouseXPos, double mouseYPos) ->
        void {App::getInstance().cursor_pos_callback(window, mouseXPos, mouseYPos); });

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) ->
		void {App::getInstance().key_callback(window, key, scancode, action, mods); });
}


void App::initGLFW() {
    glfwSetErrorCallback([](int error, const char* description) { fputs(description, stderr); });
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
}

void App::initGLEW() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void App::initAll() {

    const char* vertex_shader_cam =
        "#version 410\n"
        "layout(location = 0) in vec3 localPosition;"
		"layout(location = 1) in vec3 vn;\n"
        "uniform mat4 modelMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 projectionMatrix;"
		"out vec3 color;"
		"void main(void) { gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(localPosition, 1.0); color = vn; }";

    const char* fragment_shader =
        "#version 410\n"
        "out vec4 frag_colour;\n"
		"in vec3 color;\n"
		"void main () { frag_colour = vec4(color, 1.0); }";

    scene->addShader(new ShaderProgram(vertex_shader_cam, fragment_shader), "position");

    scene->addObject(DrawableObject(Model(tree, sizeof(tree), 92814)), "sphere");

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            scene->addObject(DrawableObject(Model(tree, sizeof(tree), 92814)), "sphere_" + (i * 10 + j));
            scene->getObject("sphere_" + (i*10+j)).setShader(scene->getShader("position"));
			scene->getObject("sphere_" + (i * 10 + j)).getModelMatrix().
				setPosition(glm::vec3(i, 0.0f, j)).setScale(glm::vec3(0.1f, 0.1f, 0.1f));
        }
    }

	scene->getObject("sphere").setShader(scene->getShader("position"));
	scene->getObject("sphere").getModelMatrix().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

}


void App::cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY) {
	scene->moveCamera(mouseX, mouseY);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        scene->movePosition(key, deltaTime);
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}