// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <framework/gl.h>
#include <GLFW/glfw3.h>

#include <framework/text.h>
#include <framework/colorrect.h>
#include <framework/stage.h>

static const unsigned int WINDOW_HEIGHT = 768;
static const unsigned int WINDOW_WIDTH = 1024;

// Isolated render loop to aid porting
GLFWwindow* window = nullptr;
Font font;
std::shared_ptr<ColorRect> rect1, rect2, rect3;
void renderLoop(void) {

	// Clear the screen
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GetStage().Render();

	//rect1.Render();
	//rect2.Render();
	//rect3.Render();

	font.RenderText("layout_1 opengl demo.", 10, 20, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	font.RenderText("OGL Demo", 600.0f, 570.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	
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

	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	GetStage().setSize(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	GetStage().setColor(Color::White);
	GetStage().Initialize();

	if(!font.Load("assets/arial.ttf")) {
		printf("Failed to initialize font!\n");
	}

	rect1 = std::make_shared<ColorRect>(glm::vec2(50, 50), glm::vec2(200, 200), Color::Red);
	GetStage().addChild(rect1);
	rect2 = std::make_shared<ColorRect>(glm::vec2(50, 50), glm::vec2(100, 100), Color::Green);
	rect1->addChild(rect2);
	rect3 = std::make_shared<ColorRect>(glm::vec2(75, 75), glm::vec2(50, 50), Color::Blue);
	rect1->addChild(rect3);

	GetStage().Initialize();

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

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

