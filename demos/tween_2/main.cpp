// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <framework/gl.h>
#include <GLFW/glfw3.h>

#include <framework/text.h>
#include <framework/colorrect.h>
#include <framework/stage.h>
#include <framework/tween.h>
#include <framework/image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static const unsigned int WINDOW_HEIGHT = 768;
static const unsigned int WINDOW_WIDTH = 1024;

// Isolated render loop to aid porting
GLFWwindow* window = nullptr;
double lastTimeElapsed = 0.0;
void renderLoop(void) {

	double timeElapsed = glfwGetTime();
	double dt = timeElapsed - lastTimeElapsed;
	lastTimeElapsed = timeElapsed;

	TweenSystem::Get().Update(dt);

	GetStage2D().Render();
	
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Layout_1 Demo", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GetStage2D().SetSize(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	GetStage2D().SetColor(Color(0.2f, 0.3f, 0.3f, 1.0f));
	GetStage2D().Initialize();

	// Initialize subsystems
	TweenSystem::Get().Startup();

	auto font = std::make_shared<Font>();
	if(!font->Load("assets/arial.ttf")) {
		printf("Failed to initialize font!\n");
	}

	#if 0
	auto rect1 = std::make_shared<ColorRect>(glm::vec2(50, 50), glm::vec2(200, 200), Color::Red);
	GetStage2D().addChild(rect1);
	auto rect2 = std::make_shared<ColorRect>(glm::vec2(0, 0), glm::vec2(100, 100), Color::Green);
	rect1->addChild(rect2);
	auto rect3 = std::make_shared<ColorRect>(glm::vec2(50, 0), glm::vec2(50, 50), Color::Blue);
	rect1->addChild(rect3);

	//text1 = std::make_shared<Text>(const std::string& str, const glm::vec2& pos, const float scale, const Color& color, std::shared_ptr<Font> font)
	auto text1 = std::make_shared<Text>("A.g,p-C123%@", glm::vec2(0,0), 1.0f, Color::White, font);
	auto text2 = std::make_shared<Text>("OpenGL Demo.", glm::vec2(0,0), 1.0f, Color(1.0f, 1.0f, 0.0f, 0.0f), font);
	GetStage2D().addChild(text1);
	rect1->addChild(text2);
	#endif

	GetStage2D().addChild(std::make_shared<Image>("assets/brick.jpg", glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f)));
	//GetStage2D().addChild(image1);
	GetStage2D().addChild(std::make_shared<Image>("assets/grid.png", glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f)));
	auto img3 = std::make_shared<Image>("assets/testpattern.jpg", glm::vec2(WINDOW_WIDTH/2, WINDOW_WIDTH/2), glm::vec2(100.0f, 100.0f));
	img3->SetCenter(glm::vec2(0.5f, 0.5f));
	//img3->SetScale(glm::vec2(0.5f, 0.5f));
	GetStage2D().addChild(img3);

	GetStage2D().Initialize();

	#if 1
	Pause(1.0f)->Then(
		TweenScale(img3, glm::vec2(2.0f, 2.0f), 4.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
		Pause(1.0f))->Then(
		TweenScale(img3, glm::vec2(0.5f, 3.0f), 4.0f, TweenSystem::Easing::EXPONENTIAL_INOUT))->Then(
		Pause(1.0f))->Then(
		TweenScale(img3, glm::vec2(4.0f, 1.0f), 4.0f, TweenSystem::Easing::BOUNCE_OUT))->Start();
	#endif

    // render loop
    // -----------
    #if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(renderLoop, 0, 1 /*simulate infinite loop */);
    #else
    while (!glfwWindowShouldClose(window))
    {
        renderLoop();
        //sleep(1);
    }
    #endif

	// Shut down subsystems
	TweenSystem::Get().Shutdown();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

