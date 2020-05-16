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
std::shared_ptr<Font> font;
std::shared_ptr<Text> text1;
std::shared_ptr<Text> text2;
std::shared_ptr<ColorRect> rect1, rect2, rect3;
std::shared_ptr<iTween> tween, tween2;
float tweenfloat = 0.0f;
glm::vec3 tweenVec3;
void renderLoop(void) {

	if(tween && !tween->isComplete())
		tween->Update(0.1f);
	if(tween2 && !tween2->isComplete())
		tween2->Update(0.1f);

	GetStage().Render();
	
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

	GetStage().setSize(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	GetStage().setColor(Color::Gray);
	GetStage().Initialize();

	font = std::make_shared<Font>();
	if(!font->Load("assets/arial.ttf")) {
		printf("Failed to initialize font!\n");
	}

	rect1 = std::make_shared<ColorRect>(glm::vec2(50, 50), glm::vec2(200, 200), Color::Red);
	GetStage().addChild(rect1);
	rect2 = std::make_shared<ColorRect>(glm::vec2(0, 0), glm::vec2(100, 100), Color::Green);
	rect1->addChild(rect2);
	rect3 = std::make_shared<ColorRect>(glm::vec2(50, 0), glm::vec2(50, 50), Color::Blue);
	rect1->addChild(rect3);

	//text1 = std::make_shared<Text>(const std::string& str, const glm::vec2& pos, const float scale, const Color& color, std::shared_ptr<Font> font)
	text1 = std::make_shared<Text>("A.g,p-C123%@", glm::vec2(0,0), 1.0f, Color::White, font);
	text2 = std::make_shared<Text>("OpenGL Demo.", glm::vec2(0,0), 1.0f, Color::Yellow, font);
	GetStage().addChild(text1);
	rect1->addChild(text2);

	GetStage().Initialize();

	tween = Tween::Create(10.0f, TweenSystem::Easing::LINEAR,
        nullptr, // onStart
        [&](float dt, Tween& tween)->bool{ // onUpdate
			tweenfloat += dt;
			if(tweenfloat >= 100.0f) return true;
			printf("Tween update dt: %f value: %f\n", dt, tweenfloat);
			return false;
		},
        [&](float dt, Tween& tween)->bool { // onComplete
			printf("Tween complete\n");
			return tween.isComplete();
		},
        nullptr); // onCancel

	glm::vec3 finalPos(100.0f, 100.0f, 0.0f);
	glm::vec3 initialPos = tweenVec3;
	tween2 = Tween::Create(10.0f, TweenSystem::Easing::LINEAR,
        nullptr, // onStart
        [&](float dt, Tween& tween)->bool{ // onUpdate
			//tweenfloat += dt;
			float a = tween.getAlpha();
			float b = 1.0f - a;
			tweenVec3 = (a*finalPos) + (b*initialPos);
			//if(tweenfloat >= 100.0f) return true;
			if(a >= 1.0f) {
				tweenVec3 = finalPos;
				return true;
			}
			printf("Tween update d: %f V:%f,%f,%f", dt, tweenVec3.x, tweenVec3.y, tweenVec3.z);
			return false;
		},
        [&](float dt, Tween& tween)->bool { // onComplete
			printf("Tween complete\n");
			return tween.isComplete();
		},
        nullptr); // onCancel

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

