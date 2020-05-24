// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "framework/gl.h"
#include "framework/shaders.h"
#include <GLFW/glfw3.h>

#if 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#define DEBUG_GUI

#if defined(DEBUG_GUI)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

#include <rlottie.h>

#if 1
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#if defined(DEBUG_GUI)
bool my_tool_active = true;
float my_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
void doMenu() {
	#if 0
	// render your GUI
    ImGui::Begin("Demo window");
    ImGui::Button("Hello!");
    ImGui::End();

	#else
	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("My First Tool", &my_tool_active, 0);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", my_color);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
	#endif
}
#endif

// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;

#ifdef __EMSCRIPTEN__
const char *vertexShaderSource = "#version 300 es\n"
#else
const char *vertexShaderSource = "#version 330 core\n"
#endif
    "precision mediump float;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";

#ifdef __EMSCRIPTEN__
const char *fragmentShaderSource = "#version 300 es\n"
#else
const char *fragmentShaderSource = "#version 330 core\n"
#endif
    "precision mediump float;\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord).zyxw;\n"
    "}\n\0";

GLFWwindow* window = nullptr;
GLuint VBO = 0;
GLuint VAO = 0;
GLuint texture = 0;
int _w = 200, _h = 200;
std::unique_ptr<uint32_t[]> buffer;
size_t frameCount = 0;
std::unique_ptr<rlottie::Animation> player;
int frame = 0;
GLuint shaderProgram = 0;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 MVP;
glm::mat4 model;
float angle_deg = 0.0f;
GLuint modelUniformLoc = 0;
GLuint viewUniformLoc = 0;
GLuint projUniformLoc = 0;
void renderLoop(void) {

	#if defined(DEBUG_GUI)
	// feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
	#endif

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(frame > frameCount)
        frame = 0;
    frame++;

    glUseProgram(shaderProgram);

    #if 0
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    #endif
    
    #if 1
    rlottie::Surface surface(buffer.get(), _w, _h, _w * 4);
	player->renderSync(frame, surface);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)buffer.get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(VAO);
	glUseProgram(shaderProgram);

    Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	View       = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

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

	// render the triangle
    //glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    #else
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    #endif

	#if defined(DEBUG_GUI)
	doMenu();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
	window = glfwCreateWindow( 1024, 768, "Lottie animatimation demo 1", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.1 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	#if defined(DEBUG_GUI)
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 100";
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	#endif

    #if 0
	// build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    #else
    shaderProgram = LoadShaderFromSource( vertexShaderSource, fragmentShaderSource);
    modelUniformLoc = glGetUniformLocation(shaderProgram, "model");
	viewUniformLoc = glGetUniformLocation(shaderProgram, "view");
	projUniformLoc = glGetUniformLocation(shaderProgram, "projection");
    #endif

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
		// positions         // colors          // tex coords
        -1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // bottom left
        1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 0.0f,// top
        1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,// top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);

    #if 0
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
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
    #else

	player = rlottie::Animation::loadFromFile("assets/alien.lottie.json");
	//player = rlottie::Animation::loadFromFile("assets/origami.lottie.json");
	//player = rlottie::Animation::loadFromFile("assets/swinging.lottie.json");
	//player = rlottie::Animation::loadFromFile("assets/checkmark.lottie.json");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

	buffer = std::unique_ptr<uint32_t[]>(new uint32_t[_w * _h]);

	frameCount = player->totalFrame();

    #if 1
	rlottie::Surface surface(buffer.get(), _w, _h, _w * 4);
	player->renderSync(100, surface);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)buffer.get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    #endif
    #endif

	// Dark blue background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

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

