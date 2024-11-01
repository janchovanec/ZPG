#include "App.h"
#include "ShaderProgram.h"

#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/plain.h"

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

    window = glfwCreateWindow(1200, 800, "ZPG", NULL, NULL);
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

void App::initScene() {

    const char* vertex_shader_cam =
        "#version 410\n"
        "layout(location = 0) in vec3 localPosition;"
		"layout(location = 1) in vec3 vn;\n"
        "uniform mat4 modelMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 projectionMatrix;"
		"out vec3 color;"
		"void main(void) { gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(localPosition, 1.0); color = vn; }";

	const char* fragment_shader_green =
        "#version 410\n"
        "out vec4 frag_colour;\n"
		"in vec3 color;\n"
		"void main () { frag_colour = vec4(color * vec3(0.1, 0.5, 0.1), 1.0); }";

	const char* fragment_shader_brown =
		"#version 410\n"
		"out vec4 frag_colour;\n"
		"in vec3 color;\n"
		"void main () { frag_colour = vec4(color * vec3(0.5, 0.3, 0.1), 1.0); }";

	const char* fragment_shader_ground =
		"#version 410\n"
		"out vec4 frag_colour;\n"
		"in vec3 color;\n"
		"void main () { frag_colour = vec4(vec3(0.1333, 0.545, 0.1333), 1.0); }";

    scene->addShader(new ShaderProgram(vertex_shader_cam, fragment_shader_green));
	scene->addShader(new ShaderProgram(vertex_shader_cam, fragment_shader_brown));
	scene->addShader(new ShaderProgram(vertex_shader_cam, fragment_shader_ground));

    // Add trees
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            scene->addObject(DrawableObject(Model(tree, sizeof(tree), 92814)), "tree_" + std::to_string(i) + ":" + std::to_string(j));
            scene->getObject("tree_" + std::to_string(i) + ":" + std::to_string(j)).setShader(scene->getShader(0));

            float scaleModifier = ((130 - (rand() % 61)) / 100.0);
            scene->getObject("tree_" + std::to_string(i) + ":" + std::to_string(j)).getModelMatrix()
                .setPosition(glm::vec3(i * ((120 - (rand() % 41)) / 100.0), 0.0f, j * ((120 - (rand() % 41)) / 100.0)))
                .setScale(glm::vec3(0.1f * scaleModifier, 0.1f * scaleModifier, 0.1f * scaleModifier))
				.setRotation(glm::vec3((8-rand()%17)/100.0, (rand() % 100) / 100.0, (8 - rand() % 17) / 100.0));
        }
    }
    
    // Add bushes
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            scene->addObject(DrawableObject(Model(bushes, sizeof(bushes), 8730)), "bush_" + std::to_string(i) + ":" + std::to_string(j));
            scene->getObject("bush_" + std::to_string(i) + ":" + std::to_string(j)).setShader(scene->getShader(1));

            float scaleModifier = ((120 - (rand() % 41)) / 100.0);
            scene->getObject("bush_" + std::to_string(i) + ":" + std::to_string(j)).getModelMatrix()
                .setPosition(glm::vec3(i * ((200 - (rand() % 101)) / 100.0), 0.0f, j * ((200 - (rand() % 101)) / 100.0)))
                .setScale(glm::vec3(0.1f * scaleModifier, 0.1f * scaleModifier, 0.1f * scaleModifier))
                .setRotation(glm::vec3((5 - rand() % 11) / 100.0, (rand() % 100) / 100.0, (5 - rand() % 11) / 100.0));
        }
    }

    // Add plain
	scene->addObject(DrawableObject(Model(plain, sizeof(plain), 6)), "plain");
	scene->getObject("plain").setShader(scene->getShader(2));
	scene->getObject("plain").getModelMatrix().setPosition(glm::vec3(0.0f, 0.0f, 0.0f)).setScale(glm::vec3(100.0f, 0.1f, 100.0f));
    
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