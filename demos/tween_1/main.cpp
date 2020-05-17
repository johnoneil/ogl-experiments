// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <framework/gl.h>
#include <GLFW/glfw3.h>

#include <framework/text.h>
#include <framework/colorrect.h>
#include <framework/stage.h>
#include <framework/tween.h>

static const unsigned int WINDOW_HEIGHT = 768;
static const unsigned int WINDOW_WIDTH = 1024;

// Isolated render loop to aid porting
GLFWwindow* window = nullptr;
void renderLoop(void) {

	TweenSystem::Get().Update(0.1f);

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

	GetStage2D().setSize(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	GetStage2D().setColor(Color::Gray);
	GetStage2D().Initialize();

	// Initialize subsystems
	TweenSystem::Get().Startup();

	auto font = std::make_shared<Font>();
	if(!font->Load("assets/arial.ttf")) {
		printf("Failed to initialize font!\n");
	}

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

	GetStage2D().Initialize();

	TweenPos(text2, glm::vec2(100.0f, 0.0f), 10.0f, TweenSystem::Easing::ELASTIC_IN)->Then(
		TweenPos(rect1, glm::vec2(300.0f, 300.0f), 10.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenPos(rect3, glm::vec2(100.0f, -120.0f), 10.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenPos(rect2, glm::vec2(200.0f, 0.0f), 10.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
			[](){printf("Tween completed...\n"); })->Then(
		TweenColor(rect1, Color::Olive, 15.0f, TweenSystem::Easing::SIN_IN))->Then(
			[](){printf("Another Tween completed...\n"); })->Then(
		TweenColor(text2, Color(0.0f, 0.0f, 1.0f, 1.0f), 15.0f, TweenSystem::Easing::SIN_IN))->Then(
			[](){printf("Yet another tween completed...\n");})->Then(
		TweenColor(text1, Color(1.0f, 1.0f, 1.0f, 0.0f), 15.0f, TweenSystem::Easing::SIN_IN))->Start();

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

