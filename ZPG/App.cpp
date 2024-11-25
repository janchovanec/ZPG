#include "App.h"
#include "ShaderProgram.h"

#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/plain.h"
#include "Models/sphere.h"
#include "Models/gift.h"

#include "BaseTransform.h"
#include "RotTransform.h"
#include "ScaleTransform.h"
#include "PosTransform.h"
#include "DRotTransform.h"

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
        scene->render();
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

void App::treeScene() {

	// Add lights
	scene->addLight(glm::vec3(0.1f, 0.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// Set camera position
	scene->setCameraPosition(glm::vec3(0.0f, 0.5f, 0.2f));

	// Add shaders
    scene->addShaderProgram("Shaders/vertex_phong.vert", "Shaders/fragment_phong.frag");
	scene->addShaderProgram("Shaders/vertex_shader_cam.vert", "Shaders/fragment_shader_brown.frag");
	scene->addShaderProgram("Shaders/vertex_shader_cam.vert", "Shaders/fragment_shader_ground.frag");
	scene->addShaderProgram("Shaders/vertex_lambert.vert", "Shaders/fragment_lambert.frag");

    std::vector<std::shared_ptr<BaseTransform>> scales;
	std::vector<std::shared_ptr<BaseTransform>> rotations;

    // Add random rotations
    for (int i = 0; i < 20; i++) {
		rotations.push_back(std::make_shared<RotTransform>(glm::vec3((8 - rand() % 17) / 100.0, (rand() % 100) / 100.0, (8 - rand() % 17) / 100.0)));
    }

    // Add random scales for trees
	for (int i = 0; i < 20; i++) {
        float scaleModifier = ((130 - (rand() % 61)) / 100.0);
		scales.push_back(std::make_shared<ScaleTransform>(glm::vec3(0.1f * scaleModifier, 0.1f * scaleModifier, 0.1f * scaleModifier)));
	}

    // Add random scales for bushes
    for (int i = 0; i < 20; i++) {
        float scaleModifier = ((120 - (rand() % 41)) / 100.0);
        scales.push_back(std::make_shared<ScaleTransform>(glm::vec3(0.1f * scaleModifier, 0.1f * scaleModifier, 0.1f * scaleModifier)));
    }

	// Dynamic rotation for trees
	std::shared_ptr<DRotTransform> drot = std::make_shared<DRotTransform>(glm::vec3(0.0f, 0.0001f, 0.0f));

    // Add trees
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            scene->addObject(DrawableObject(scene->getShader(0), Model(tree, sizeof(tree), 92814), glm::vec3(0.0,(rand()%10)/10.0, 0.0)), "tree_" + std::to_string(i) + ":" + std::to_string(j));
            auto& obj = scene->getObject("tree_" + std::to_string(i) + ":" + std::to_string(j));
            // Translation
            obj.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(i * ((120 - (rand() % 41)) / 100.0), 0.0f, j * ((120 - (rand() % 41)) / 100.0))));
			// Rotation
			obj.getModelMatrix().addTransform(rotations[rand()%20]);
			// Scale
			obj.getModelMatrix().addTransform(scales[rand() % 20]);

			// Add dynamic rotation
			obj.getModelMatrix().addTransform(drot);
        }
    }
    
    // Add bushes
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            scene->addObject(DrawableObject(Model(bushes, sizeof(bushes), 8730)), "bush_" + std::to_string(i) + ":" + std::to_string(j));
            scene->getObject("bush_" + std::to_string(i) + ":" + std::to_string(j)).setShader(scene->getShader(1));

			auto& obj = scene->getObject("bush_" + std::to_string(i) + ":" + std::to_string(j));

			// Translation
			obj.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(i * ((200 - (rand() % 101)) / 100.0), 0.0f, j * ((200 - (rand() % 101)) / 100.0))));
			// Rotation
			obj.getModelMatrix().addTransform(rotations[rand() % 20]);
			// Scale
			obj.getModelMatrix().addTransform(scales[20 + rand() % 20]);
        }
    }

	// Add gifts
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			scene->addObject(DrawableObject(scene->getShader(3), Model(gift, sizeof(gift), 66624), glm::vec3(1.0f, 0.0f, 0.0f)), "gift_" + std::to_string(i) + ":" + std::to_string(j));

			auto& obj = scene->getObject("gift_" + std::to_string(i) + ":" + std::to_string(j));

			// Translation
			obj.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(i * ((200 - (rand() % 101)) / 100.0), 0.0f, j * ((200 - (rand() % 101)) / 100.0))));
			// Rotation
			obj.getModelMatrix().addTransform(rotations[rand() % 20]);
			// Scale
			obj.getModelMatrix().addTransform(scales[20 + rand() % 20]);
		}
	}

    // Add plain
	scene->addObject(DrawableObject(Model(plain, sizeof(plain), 6)), "plain");

	auto& plain = scene->getObject("plain");
	plain.setShader(scene->getShader(2));
	plain.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(0.0f, 0.0f, 0.0f)));
	plain.getModelMatrix().addTransform(std::make_shared<ScaleTransform>(glm::vec3(100.0f, 0.1f, 100.0f)));
    
}

void App::ballScene() {
	scene->addLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	scene->setCameraPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	scene->addShaderProgram("Shaders/vertex_phong.vert", "Shaders/fragment_phong.frag", glm::vec3(1.0, 1.0, 1.0));

	shared_ptr<ScaleTransform> scale = make_shared<ScaleTransform>(glm::vec3(0.1f, 0.1f, 0.1f));

	scene->addObject(DrawableObject(Model(sphere, sizeof(sphere), 2880)), "sphere");
	scene->addObject(DrawableObject(Model(sphere, sizeof(sphere), 2880)), "sphere2");
	scene->addObject(DrawableObject(Model(sphere, sizeof(sphere), 2880)), "sphere3");
	scene->addObject(DrawableObject(Model(sphere, sizeof(sphere), 2880)), "sphere4");


	auto& sphere = scene->getObject("sphere");
	sphere.setShader(scene->getShader(0));
	sphere.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(0.5f, 0.0f, 0.0f)));
	sphere.getModelMatrix().addTransform(scale);

	sphere = scene->getObject("sphere2");
	sphere.setShader(scene->getShader(0));
	sphere.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(-0.5f, 0.0f, 0.0f)));
	sphere.getModelMatrix().addTransform(scale);

	sphere = scene->getObject("sphere3");
	sphere.setShader(scene->getShader(0));
	sphere.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(0.0f, 0.0f, 0.5f)));
	sphere.getModelMatrix().addTransform(scale);

	sphere = scene->getObject("sphere4");
	sphere.setShader(scene->getShader(0));
	sphere.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(0.0f, 0.0f, -0.5f)));
	sphere.getModelMatrix().addTransform(scale);
}

void App::triangleScene()
{
	scene->addLight(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	scene->setCameraPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	scene->addShaderProgram("Shaders/vertex_phong.vert", "Shaders/fragment_phong.frag", glm::vec3(1.0, 1.0, 1.0));

	shared_ptr<PosTransform> pos = make_shared<PosTransform>(glm::vec3(0.0f, 0.0f, 0.0f));
	shared_ptr<ScaleTransform> scale = make_shared<ScaleTransform>(glm::vec3(0.5f, 0.5f, 0.5f));
	shared_ptr<RotTransform> rot = make_shared<RotTransform>(glm::vec3(0.0f, 0.0f, 0.0f));

	scene->addObject(DrawableObject(scene->getShader(0), Model(plain, sizeof(plain)/2, 6), glm::vec3(1.0, 1.0, 0.0)), "triangle");
	auto& mat = scene->getObject("triangle").getModelMatrix();
	mat.addTransform(pos);
	mat.addTransform(scale);
	mat.addTransform(rot);

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
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS) {
			scene->updateShaders();
		}
		break;
    }
}