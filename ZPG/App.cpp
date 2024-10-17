#include "App.h"

App::App() {
    initGLFW();
    initGLEW();
    createShaders();
    createModels();
}

App::~App() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run() {
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
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

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        });
}

void App::initGLEW() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void App::createShaders() {
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;\n"
        "void main () { gl_Position = vec4 (vp, 1.0); }";

    const char* fragment_shader =
        "#version 330\n"
        "out vec4 frag_colour;\n"
        "void main () { frag_colour = vec4(0.5, 0.0, 0.5, 1.0); }";

    const char* fragment_shader2 =
        "#version 330\n"
        "out vec4 frag_colour;\n"
        "void main () { frag_colour = vec4(1.0, 1.0, 0.0, 1.0); }";

    shader1 = new Shader(vertex_shader, fragment_shader);
    shader2 = new Shader(vertex_shader, fragment_shader2);
}

void App::createModels() {
    float points1[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    float points2[] = {
        0.6f, 0.6f, 0.0f,
        0.9f, 0.6f, 0.0f,
        0.9f, 0.9f, 0.0f,
        0.9f, 0.9f, 0.0f,
        0.6f, 0.9f, 0.0f,
        0.6f, 0.6f, 0.0f
    };

    model1 = new Model(points1, sizeof(points1), 3);
    model2 = new Model(points2, sizeof(points2), 6);
}

void App::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader1->use();
    model1->draw();

    shader2->use();
    model2->draw();
}