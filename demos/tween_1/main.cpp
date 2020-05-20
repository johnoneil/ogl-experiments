// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <framework/gl.h>
#include <GLFW/glfw3.h>

#include <framework/text.h>
#include <framework/colorrect.h>
#include <framework/stage.h>
#include <framework/tween.h>
#include <glm/gtc/constants.hpp>

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
	GetStage2D().SetColor(Color::Gray);
	GetStage2D().Initialize();

	// Initialize subsystems
	TweenSystem::Get().Startup();

	auto font = std::make_shared<Font>();
	if(!font->Load("assets/arial.ttf")) {
		printf("Failed to initialize font!\n");
	}

	auto rect1 = std::make_shared<ColorRect>(glm::vec2(400, 400), glm::vec2(200, 200), Color::Red);
	GetStage2D().addChild(rect1);
	//rect1->SetRotation(glm::quarter_pi<float>());
	rect1->SetCenter(glm::vec2(0.5f, 0.5f));
	//auto rect2 = std::make_shared<ColorRect>(glm::vec2(0, 0), glm::vec2(100, 100), Color::Green);
	//rect1->addChild(rect2);
	//auto rect3 = std::make_shared<ColorRect>(glm::vec2(50, 0), glm::vec2(50, 50), Color::Blue);
	//rect1->addChild(rect3);

	auto text1 = std::make_shared<Text>("A.g,p-C123%@", glm::vec2(0,0), Color::White, font);
	auto text2 = std::make_shared<Text>("OpenGL Demo.", glm::vec2(0,0), Color(1.0f, 1.0f, 0.0f, 1.0f), font);
	GetStage2D().addChild(text1);
	rect1->addChild(text2);

	GetStage2D().Initialize();

	#if 1
	Pause(3.0f)->Then(
	//TweenPos(rect1, glm::vec2(400.0f, 400.0f), 2.0f, TweenSystem::Easing::ELASTIC_IN))->Then(
		TweenRotation(rect1, glm::half_pi<float>(), 2.0f, TweenSystem::Easing::LINEAR))->Then(
		//Pause(3.0f))->Then(
		//TweenPos(rect1, glm::vec2(300.0f, 300.0f), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		//TweenPos(rect3, glm::vec2(100.0f, -120.0f), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		//TweenPos(rect2, glm::vec2(200.0f, 0.0f), 2.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
		//Pause(3.0f))->Then(
		//TweenColor(rect1, Color::Olive, 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		//Pause(3.0f))->Then(
		//TweenColor(text2, Color::Orange, 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		//Pause(3.0f))->Then(
		//TweenColor(text1, Color(1.0f, 1.0f, 1.0f, 0.5f), 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		//TweenPos(rect3, glm::vec2(0.0f, 0.0f), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		//TweenSize(rect3,glm::vec2(50.0f, 50.0f), 2.0f, TweenSystem::Easing::QUADRATIC_INOUT))->Then(
		//TweenPos(rect2, glm::vec2(50.0f, 50.0f), 2.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
		TweenScale(rect1, glm::vec2(2.0f, 2.0f), 2.0f, TweenSystem::Easing::LINEAR))->Then(
		TweenRotation(rect1, glm::quarter_pi<float>(), 2.0f, TweenSystem::Easing::SIN_INOUT))->Then(
		Pause(3.0f))->Start();
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

