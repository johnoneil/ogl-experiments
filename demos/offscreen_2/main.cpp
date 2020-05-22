// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <framework/gl.h>
#include "framework/GL/texture.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <framework/text.h>
#include <framework/colorrect.h>
#include <framework/stage.h>
#include <framework/tween.h>
#include <glm/gtc/constants.hpp>

#include <framework/shaders.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static unsigned int WINDOW_WIDTH = 1024;
static unsigned int WINDOW_HEIGHT = 768;
static unsigned int OFFSCREEN_SURFACE_WIDTH = 512;
static unsigned int OFFSCREEN_SURFACE_HEIGHT = 512;


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
"layout(location = 0) out vec4 color;"
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
"    color = vec4(result, 1.0);\n"
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
TextureResource texture;
GLuint modelUniformLoc = 0;
GLuint viewUniformLoc = 0;
GLuint projUniformLoc = 0;
GLuint lightPosUniformLoc = 0;
GLuint lightColorUniformLoc = 0;
GLuint objectColorUniformLoc = 0;
GLuint FramebufferName = 0;
GLuint depthrenderbuffer = 0;
GLuint renderedTexture = 0;
int viewportWidth = 0, viewportHeight = 0;
double lastTimeElapsed = 0.0;
void renderLoop(void) {
	angle_deg += 0.33f;

	double timeElapsed = glfwGetTime();
	double dt = timeElapsed - lastTimeElapsed;
	lastTimeElapsed = timeElapsed;

	// Render to our framebuffer managed offscreen texture
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	// the offscreen texture we're rendering to should match the window width and height
	glViewport(0,0,OFFSCREEN_SURFACE_WIDTH, OFFSCREEN_SURFACE_HEIGHT);

	TweenSystem::Get().Update(dt);

	GetStage2D().Render();

	glBindVertexArray(0);
	glUseProgram(0);

	#if 1
	{
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// now render all over again to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// The window framebuffer might be differently sized, so we use the entire viewport
		glViewport(0,0, viewportWidth, viewportHeight);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glBindVertexArray(VAO);
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		model = glm::mat4(1.0f);

		model = glm::translate(model,glm::vec3(0,0,0)); //position = 0,0,0
		model = glm::rotate(model,glm::radians(angle_deg),glm::vec3(1,0,0));//rotation x = 0.0 degrees
		model = glm::rotate(model,glm::radians(angle_deg),glm::vec3(0,1,0));//rotation y = 0.0 degrees
		model = glm::rotate(model,glm::radians(0.0f),glm::vec3(0,0,1));//rotation z = 0.0 degrees
		model = glm::scale(model,glm::vec3(2, 2, 2));//scale = 2,2,2, because mesh is 0.5 based geom.

		// Uniforms:
		glUniformMatrix4fv(modelUniformLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projUniformLoc, 1, GL_FALSE, &Projection[0][0]);
		glm::vec3 lightPos(4, 3, -3);
		glUniform3fv(lightPosUniformLoc, 1, &lightPos[0]);
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glUniform3fv(lightColorUniformLoc, 1, &lightColor[0]);
		glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
		glUniform3fv(objectColorUniformLoc, 1, &objectColor[0]);

		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glBindVertexArray(0);
		glUseProgram(0);
	}
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
	window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Offscreen render", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaderFromSource( vertexShaderSource, fragmentShaderSource);

	// Get a handle for our "MVP" uniform
	modelUniformLoc = glGetUniformLocation(programID, "model");
	viewUniformLoc = glGetUniformLocation(programID, "view");
	projUniformLoc = glGetUniformLocation(programID, "projection");
	lightPosUniformLoc = glGetUniformLocation(programID, "lightPos");
	lightColorUniformLoc = glGetUniformLocation(programID, "lightColor");
	objectColorUniformLoc = glGetUniformLocation(programID, "objectColor");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	View       = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 MVP        = Projection * View * model; // Remember, matrix multiplication is the other way around

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		// Vertex pos         // Tex coord  // normal
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  1.0f,  0.0f,
	};

	glGenVertexArrays(1, &VAO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8*sizeof(float),    // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8*sizeof(float),    // stride
		(void*)(3 * sizeof(float))// array buffer offset
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8*sizeof(float),    // stride
		(void*)(5 * sizeof(float))// array buffer offset
	);

	glBindVertexArray(0);

    auto w = 0, h = 0, c = 0;
    stbi_set_flip_vertically_on_load(true);
    auto image = stbi_load("assets/brick.jpg",
                                    &w,
                                    &h,
                                    &c,
                                    STBI_rgb_alpha);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

	// glfw can provide a frame buffer of different dimentions from the actual window sie
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);

	#if 1
	// The framebuffer, which groups 0 or more textures, and 0 or 1 depth buffer.
	
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, OFFSCREEN_SURFACE_WIDTH, OFFSCREEN_SURFACE_HEIGHT, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set "renderedTexture" as our colour attachement #0
	#if 0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	#else
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	#endif

	#if 1
	GLuint depthRenderbuffer = 0;
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, OFFSCREEN_SURFACE_WIDTH, OFFSCREEN_SURFACE_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	#endif

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer status not yet complete...!\n");
		//return 1;
	}

	#endif

	#if 1
	GetStage2D().SetSize(glm::vec2(OFFSCREEN_SURFACE_WIDTH, OFFSCREEN_SURFACE_HEIGHT));
	GetStage2D().SetColor(Color::Gray);
	GetStage2D().Initialize();

	// Initialize subsystems
	TweenSystem::Get().Startup();

	auto font = std::make_shared<Font>();
	if(!font->Load("assets/arial.ttf")) {
		printf("Failed to initialize font!\n");
	}

	auto rect1 = std::make_shared<ColorRect>(glm::vec2(128, 128), glm::vec2(256, 256), Color::Red);
	GetStage2D().addChild(rect1, "parent");
	//rect1->SetRotation(glm::quarter_pi<float>());
	rect1->SetCenter(glm::vec2(0.5f, 0.5f));
	auto rect2 = std::make_shared<ColorRect>(glm::vec2(0, 0), glm::vec2(100, 100), Color::Green);
	rect2->SetCenter(glm::vec2(0.5f, 0.5f));
	rect1->addChild(rect2);
	auto rect3 = std::make_shared<ColorRect>(glm::vec2(50, 0), glm::vec2(50, 50), Color::Blue);
	rect3->SetCenter(glm::vec2(0.5f, 0.5f));
	rect1->addChild(rect3);

	auto text1 = std::make_shared<Text>("OPenGL Offscreen Render Demo", glm::vec2(0,0), Color::White, font);
	auto text2 = std::make_shared<Text>("OpenGL Demo.", glm::vec2(0,0), Color(1.0f, 1.0f, 0.0f, 1.0f), font);
	GetStage2D().addChild(text1);
	rect1->addChild(text2);

	GetStage2D().Initialize();

	auto parent = GetStage2D().GetByName("parent");

	#if 1
	Pause(3.0f)->Then(
	//TweenPos(rect1, glm::vec2(400.0f, 400.0f), 2.0f, TweenSystem::Easing::ELASTIC_IN))->Then(
		TweenRotation(parent, glm::half_pi<float>(), 2.0f, TweenSystem::Easing::LINEAR))->Then(
		//Pause(3.0f))->Then(
		//TweenPos(parent, glm::vec2(300.0f, 300.0f), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenPos(rect3, glm::vec2(100.0f, -120.0f), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenRotation(rect3, glm::half_pi<float>(), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenPos(rect2, glm::vec2(200.0f, 0.0f), 2.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
		TweenRotation(rect2, glm::half_pi<float>(), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenPos(rect3, glm::vec2(50.0f, 0.0f), 2.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
		TweenRotation(rect2, glm::three_over_two_pi<float>(), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		//Pause(3.0f))->Then(
		TweenColor(rect1, Color::Olive, 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		//Pause(3.0f))->Then(
		TweenColor(text2, Color::Orange, 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		//Pause(3.0f))->Then(
		TweenColor(text1, Color(1.0f, 1.0f, 1.0f, 0.5f), 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		TweenPos(rect3, glm::vec2(0.0f, 0.0f), 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenPos(rect2, glm::vec2(50.0f, 50.0f), 2.0f, TweenSystem::Easing::BOUNCE_OUT))->Then(
		TweenRotation(rect2, 0.0f, 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenRotation(rect3, 0.0f, 2.0f, TweenSystem::Easing::BACK_INOUT))->Then(
		TweenScale(parent, glm::vec2(2.0f, 2.0f), 2.0f, TweenSystem::Easing::LINEAR))->Then(
		TweenRotation(parent, glm::quarter_pi<float>(), 2.0f, TweenSystem::Easing::SIN_INOUT))->Then(
		TweenColor(parent, Color(1.0f, 0.0f, 0.0f, 1.0f), 2.0f, TweenSystem::Easing::SIN_IN))->Then(
		Pause(3.0f))->Start();
	#endif
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

	// Cleanup VBO and shader
	texture.Free();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

