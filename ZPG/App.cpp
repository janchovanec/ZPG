#include "App.h"
#include "ShaderProgram.h"

#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/plain.h"
#include "Models/sphere.h"
#include "Models/gift.h"
#include "Models/tex_triangle.h"

#include "BaseTransform.h"
#include "RotTransform.h"
#include "ScaleTransform.h"
#include "PosTransform.h"
#include "DRotTransform.h"
#include "LightPosTransform.h"
#include "LightMovement.h"

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
	glfwSetFramebufferSizeCallback(window, win_size_callback);
}

void App::initGLEW() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void App::treeScene() {

	// Add dir light simulating moonlight

	scene->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.45f), glm::vec3(0.2f, 0.2f, 0.2f));

	/*
	// Add point light
	//scene->addPointLight(glm::vec3(0.3f, 0.5f, 0.6f), glm::vec3(0.8f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);

	// Add spot light
	scene->addSpotLight(glm::vec3(0.3f, 0.5f, 0.6f), // position
		glm::vec3(0.0f, -1.0f, 0.0f), // direction
		glm::vec3(0.1f, 0.1f, 0.1f), // ambient
		glm::vec3(0.8f, 0.8f, 0.8f), // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // specular
		1.0f, // constant
		0.09f, // linear
		0.032f, // quadratic
		glm::cos(glm::radians(12.5f))); // cutOff
	*/

	// Set camera position
	scene->setCameraPosition(glm::vec3(0.0f, 0.5f, 0.2f));

	// Add shaders
    scene->addShaderProgram("Shaders/vertex_phong.vert", "Shaders/fragment_lights.frag");
	scene->addShaderProgram("Shaders/vertex_phong.vert", "Shaders/fragment_lights.frag");
	scene->addShaderProgram("Shaders/vertex_texture_light.vert", "Shaders/fragment_texture_light.frag");

    std::vector<std::shared_ptr<BaseTransform>> scales;
	std::vector<std::shared_ptr<BaseTransform>> rotations;

	// Add flashlight
	scene->addFlashLight(glm::vec3(0.3f, 0.5f, 0.6f), // position
		glm::vec3(0.0f, -1.0f, 0.0f), // direction
		glm::vec3(0.1f, 0.1f, 0.1f), // ambient
		glm::vec3(0.8f, 0.8f, 0.8f), // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // specular
		1.0f, // constant
		0.09f, // linear
		0.032f, // quadratic
		glm::cos(glm::radians(24.5f))); // cutOff

	
	// add fireflies
	for (int i = 0; i < 10; i++) {
		glm::vec3 pos = glm::vec3((rand() % 100) / 10.0, 0.1 + (rand() % 100) / 1000.0, (rand() % 100) / 10.0);
		scene->addPointLight(pos,
							 glm::vec3(0.05f, 0.05f, 0.0f),
							 glm::vec3(0.0f, 0.0f, 0.0f),
							 glm::vec3(0.2f, 0.2f, 0.1f),
							 1.0f, 0.7f, 1.8f);
		scene->lights.back()->setMovementMethod(LightMovement::oscillate);

		scene->addObject(DrawableObject(scene->getShader(0), new Model(sphere, sizeof(sphere), 2880), glm::vec3(0.1f, 0.5f, 0.0f)), "firefly_" + std::to_string(i));
		auto& obj = scene->getObject("firefly_" + std::to_string(i));
		obj.getModelMatrix().addTransform(std::make_shared<LightPosTransform>(scene->lights.back(), glm::vec3(0, 0.1, 0)));
		obj.getModelMatrix().addTransform(std::make_shared<ScaleTransform>(glm::vec3(0.02f, 0.02f, 0.02f)));
	}

	// add fireflies flying in circle
	for (int i = 0; i < 10; i++) {
		glm::vec3 pos = glm::vec3((rand() % 100) / 1000.0, 0.2, (rand() % 100) / 1000.0);
		scene->addPointLight(pos,
			glm::vec3(0.02f, 0.09f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.01f, 0.01f, 0.01f),
			0.01f, 0.01f, 10.0f);
		scene->lights.back()->setMovementMethod(LightMovement::circularMotion);

		scene->addObject(DrawableObject(scene->getShader(0), new Model(sphere, sizeof(sphere), 2880), glm::vec3(0.1f, 0.5f, 0.0f)), "firefly_circle_" + std::to_string(i));
		auto& obj = scene->getObject("firefly_circle_" + std::to_string(i));
		obj.getModelMatrix().addTransform(std::make_shared<LightPosTransform>(scene->lights.back(), glm::vec3(0, -0.1, 0)));
		obj.getModelMatrix().addTransform(std::make_shared<ScaleTransform>(glm::vec3(0.005f, 0.005f, 0.005f)));
	}




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
            scene->addObject(DrawableObject(scene->getShader(0), new Model(tree, sizeof(tree), 92814), glm::vec3(0.01,0.1+(rand()%100)/100.0, 0.01)), "tree_" + std::to_string(i) + ":" + std::to_string(j));
            auto& obj = scene->getObject("tree_" + std::to_string(i) + ":" + std::to_string(j));
            // Translation
            obj.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(i * ((120 - (rand() % 41)) / 100.0), 0.0f, j * ((120 - (rand() % 41)) / 100.0))));
			// Rotation
			obj.getModelMatrix().addTransform(rotations[rand()%20]);
			// Scale
			obj.getModelMatrix().addTransform(scales[rand() % 20]);

			// Add dynamic rotation to some trees
			if (rand()%5 == 0) obj.getModelMatrix().addTransform(drot);
        }
    }
    
    // Add bushes
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            scene->addObject(DrawableObject(new Model(bushes, sizeof(bushes), 8730)), "bush_" + std::to_string(i) + ":" + std::to_string(j));
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
			scene->addObject(DrawableObject(scene->getShader(0), new Model(gift, sizeof(gift), 66624), glm::vec3(1.0f, 0.0f, 0.0f)), "gift_" + std::to_string(i) + ":" + std::to_string(j));

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
	scene->addObject(DrawableObject(scene->getShader(2), new TexturedModel(tex_plain, sizeof(tex_plain), 6, "Shaders/Textures/grass.png")), "plain");

	auto& plain = scene->getObject("plain");
	plain.getModelMatrix().addTransform(std::make_shared<PosTransform>(glm::vec3(0.0f, 0.0f, 0.0f)));
	plain.getModelMatrix().addTransform(std::make_shared<ScaleTransform>(glm::vec3(100.0f, 0.1f, 100.0f)));
    
}

void App::ballScene() {
	scene->addSpotLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)));
	scene->setCameraPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	scene->addShaderProgram("Shaders/vertex_phong.vert", "Shaders/fragment_phong.frag", glm::vec3(1.0, 1.0, 1.0));

	shared_ptr<ScaleTransform> scale = make_shared<ScaleTransform>(glm::vec3(0.1f, 0.1f, 0.1f));

	scene->addObject(DrawableObject(new Model(sphere, sizeof(sphere), 2880)), "sphere");
	scene->addObject(DrawableObject(new Model(sphere, sizeof(sphere), 2880)), "sphere2");
	scene->addObject(DrawableObject(new Model(sphere, sizeof(sphere), 2880)), "sphere3");
	scene->addObject(DrawableObject(new Model(sphere, sizeof(sphere), 2880)), "sphere4");


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
	scene->addFlashLight(glm::vec3(0.3f, 0.5f, 0.6f), // position
		glm::vec3(0.0f, -1.0f, 0.0f), // direction
		glm::vec3(0.1f, 0.1f, 0.1f), // ambient
		glm::vec3(0.8f, 0.8f, 0.8f), // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // specular
		1.0f, // constant
		0.09f, // linear
		0.032f, // quadratic
		glm::cos(glm::radians(24.5f))); // cutOff

	scene->addDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
	scene->setCameraPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	scene->addShaderProgram("Shaders/vertex_texture_light.vert", "Shaders/fragment_texture_light.frag", glm::vec3(1.0, 1.0, 1.0));

	shared_ptr<PosTransform> pos = make_shared<PosTransform>(glm::vec3(0.0f, 0.0f, 0.0f));
	shared_ptr<ScaleTransform> scale = make_shared<ScaleTransform>(glm::vec3(1.0f, 1.0f, 1.0f));
	shared_ptr<RotTransform> rot = make_shared<RotTransform>(glm::vec3(0.0f, 0.0f, 0.0f));

	scene->addObject(DrawableObject(scene->getShader(0), new TexturedModel(tex_triangle, sizeof(tex_triangle), 12, "Shaders/Textures/wooden_fence.png")), "triangle");
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
	// debug cutoff for flashlight
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS) {
			scene->flashlight->setCutOff(scene->flashlight->getCutOff() + 0.01f);
		}
		break;
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS) {
			scene->flashlight->setCutOff(scene->flashlight->getCutOff() - 0.01f);
		}
		break;
    }
}

void App::updateProjectionMatrix(float width, float height)
{
	float aspect = width / height;
	scene->setProjectionMatrix(glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f));
}

void App::win_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	App::getInstance().updateProjectionMatrix(width, height);
}