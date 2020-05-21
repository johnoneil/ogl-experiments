// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <framework/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <framework/shaders.h>
#include <framework/text.h>
#include <framework/stage.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static const unsigned int WINDOW_HEIGHT = 768;
static const unsigned int WINDOW_WIDTH = 1024;
static const unsigned int TEXT_SIZE = 48;


#ifdef __EMSCRIPTEN__
const char *vertexShaderSource = "#version 300 es\n"
#else
const char *vertexShaderSource = "#version 330 core\n"
#endif
"precision mediump float;\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"layout (location = 2) in vec3 aNormal;\n"
"out vec2 TexCoord;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    TexCoord = aTexCoord;\n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    mat3 normalMatrix = transpose(inverse(mat3(model)));\n"
"    Normal = normalize(normalMatrix * aNormal);\n"
"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

#ifdef __EMSCRIPTEN__
const char *fragmentShaderSource = "#version 300 es\n"
#else
const char *fragmentShaderSource = "#version 330 core\n"
#endif
"precision mediump float;\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"in vec3 Normal;  \n"
"in vec3 FragPos;  \n"
"uniform vec3 lightPos; \n"
"uniform vec3 lightColor;\n"
"uniform vec3 objectColor;\n"
"uniform sampler2D uTexture;\n"
"void main()\n"
"{\n"
"    // ambient\n"
"    float ambientStrength = 0.1;\n"
"    vec3 ambient = ambientStrength * lightColor;\n"
"    // Texture\n"
"    vec3 modulatedColor = (texture(uTexture, TexCoord)).xyz;\n"
"    // diffuse \n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPos - FragPos);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * lightColor;     \n"
"    vec3 result = (ambient + diffuse) * modulatedColor;\n"
"    FragColor = vec4(result, 1.0);\n"
"}\n";

// Isolated render loop to aid porting
GLFWwindow* window = nullptr;
GLuint programID = 0;
GLuint VBO = 0, VAO = 0;
float angle_deg = 0.0f;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 MVP;
glm::mat4 model;
GLuint vertexbuffer = 0;
GLuint colorbuffer = 0;
GLuint MatrixID = 0;
GLuint modelUniformLoc = 0;
GLuint viewUniformLoc = 0;
GLuint projUniformLoc = 0;
GLuint lightPosUniformLoc = 0;
GLuint lightColorUniformLoc = 0;
GLuint objectColorUniformLoc = 0;
Font font;
void renderLoop(void) {
	angle_deg += 0.33f;

	#if 0
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#else
	GetStage2D().Render(glm::mat4(1.0f));
	#endif

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
	window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Text_1 Demo", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.1 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	#if 1
	GetStage2D().SetSize(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	GetStage2D().SetColor(Color::Gray);
	GetStage2D().Initialize();
	#else
	// Dark blue background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	#endif

	auto font = std::make_shared<Font>();
	if(!font->Load("assets/arial.ttf", TEXT_SIZE)) {
		printf("Failed to initialize font!\n");
	}

	auto text1 = std::make_shared<Text>("OpenGL Text_1 demo.", glm::vec2(0.f, 0.0f), Color::White, font);
	auto text2Size = font->GetRect("OpenGL Demo.");
	auto text2 = std::make_shared<Text>("OpenGL Demo.", glm::vec2(0.0f,static_cast<float>(WINDOW_HEIGHT-text2Size.y)), Color::Yellow, font);
	GetStage2D().addChild(text1);
	GetStage2D().addChild(text2);

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

