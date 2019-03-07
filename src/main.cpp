#include <GL/glew.h>

#include <GLFW/glfw3.h>
static GLFWwindow* window;
unsigned windowWidth  = 1000u;
unsigned windowHeight = 1000u;

#include <iostream>
#include <string>

#include "model.h"
#include "framebuffer.h"
#include "rasterizer.h"

void drawModel(ed::Framebuffer& framebuffer, ed::Model& model);

bool initGL();
void keyboard_input();
void setupQuad();
void drawQuad();
void setupTexture();
void updateTexture(std::vector<uint8_t>& data);

struct GLContext
{
    GLuint vaoId;
    GLuint vboId;
    GLuint texId;
    GLuint progId;
};

GLContext gContext;

int main(int argc, char* argv[])
{
    using namespace ed;

    std::string filepath = "assets/suzanne.obj";
    if (argc == 2) {
        filepath = argv[1];
    }

    Model model;
    model.loadObj(filepath.c_str());
    Framebuffer framebuffer(windowWidth, windowHeight);

    framebuffer.output().writePPM("out.ppm");

    std::vector<uint8_t> renderedData = std::vector<uint8_t>(framebuffer.output().width() *
                                                             framebuffer.output().height() * 4u);

    /********* GL *********/

    if (!initGL()) {
        return 0;
    }

    glClearColor(0.12f, 0.11f, 0.14f, 1.0f);

    setupQuad();
    setupTexture();

#ifdef __APPLE__ // Mojave is stupid
    keyboard_input();
    glfwSetWindowSize(window, windowWidth, windowHeight);
#endif

    double previousTime = glfwGetTime();
    uint32_t nrFrames   = 0u;
    while (glfwWindowShouldClose(window) == 0) {
        double currentTime = glfwGetTime();
        ++nrFrames;
        if (currentTime - previousTime >= 1.0) {
            std::string fpsString = std::to_string(nrFrames) + " FPS";
            glfwSetWindowTitle(window, fpsString.c_str());
            nrFrames = 0u;
            previousTime += 1.0;
        }

        keyboard_input();

        drawModel(framebuffer, model);
        framebuffer.output().writeData(renderedData);
        updateTexture(renderedData);

        glClear(GL_COLOR_BUFFER_BIT);

        drawQuad();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void drawModel(ed::Framebuffer& framebuffer, ed::Model& model)
{
    using namespace ed;

    Color red(255u, 120u, 0u);
    Color green(20u, 255u, 120u);
    Color gray(50u, 50u, 50u);

    framebuffer.output().fill(gray);

    const uint32_t halfWidth  = framebuffer.output().width()  * 0.5f;
    const uint32_t halfHeight = framebuffer.output().height() * 0.5f;

    for (uint32_t i = 0u; i < model.vertices().size(); i += 3u) {
        Vertex v0 = model.vertices()[i + 0u];
        Vertex v1 = model.vertices()[i + 1u];
        Vertex v2 = model.vertices()[i + 2u];

        Vector3 edge0 = normalize(v0.position - v1.position);
        Vector3 edge1 = normalize(v0.position - v2.position);
        Vector3 normal = cross(edge0, edge1);

        if (dot(normal, Vector3(0.0f, 0.0f, -1.0f)) < 0.0f)
            continue;

        float x0 = (v0.position.x() * 0.7f + 1.0f) * halfWidth;
        float y0 = (v0.position.y() * 0.7f + 1.0f) * halfHeight;
        float x1 = (v1.position.x() * 0.7f + 1.0f) * halfWidth;
        float y1 = (v1.position.y() * 0.7f + 1.0f) * halfHeight;
        float x2 = (v2.position.x() * 0.7f + 1.0f) * halfWidth;
        float y2 = (v2.position.y() * 0.7f + 1.0f) * halfHeight;

        drawLine(framebuffer.output(), Vector2(x0, y0), Vector2(x1, y1), green);
        drawLine(framebuffer.output(), Vector2(x0, y0), Vector2(x2, y2), green);
        drawLine(framebuffer.output(), Vector2(x1, y1), Vector2(x2, y2), green);
    }
}

bool initGL()
{
    // Initialise GLFW
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window
    window = glfwCreateWindow(windowWidth - 1u, windowHeight - 1u, "W1S1", nullptr, nullptr);
    if(window == nullptr) {
        std::cerr <<  "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    return true;
}

void keyboard_input()
{
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void setupQuad() {

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aUv;\n"
        "out vec2 vUv;\n"
        "void main()\n"
        "{\n"
        "   vUv = aUv;"
        "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "}\n\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "uniform sampler2D tex;"
        "in vec2 vUv;"
        "out vec3 outColor;\n"
        "void main()\n"
        "{\n"
        "   outColor = texture(tex, vUv).rgb;\n"
        "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    gContext.progId = glCreateProgram();
    glAttachShader(gContext.progId, vertexShader);
    glAttachShader(gContext.progId, fragmentShader);
    glLinkProgram(gContext.progId);

    glGetProgramiv(gContext.progId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(gContext.progId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    static float quadBufferData[] = {
       // P.x,   P.y, UV.s, UV.t
         1.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, 0.0f, 1.0f, // top-left

         1.0f, -1.0f, 1.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, 0.0f, 1.0f, // top-left
    };

    glGenVertexArrays(1, &gContext.vaoId);
    glBindVertexArray(gContext.vaoId);

        glGenBuffers(1, &gContext.vboId);
        glBindBuffer(GL_ARRAY_BUFFER, gContext.vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadBufferData), quadBufferData, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void drawQuad()
{
    glUseProgram(gContext.progId);

    glBindVertexArray(gContext.vaoId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gContext.texId);

    GLuint texLocation = glGetUniformLocation(gContext.progId, "tex");
    glUniform1i(texLocation, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void setupTexture()
{
    std::vector<unsigned char> data = std::vector<uint8_t>(1024*768*4);
    std::fill(data.begin(), data.end(), 255u);

    glGenTextures(1, &gContext.texId);

    glBindTexture(GL_TEXTURE_2D, gContext.texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void updateTexture(std::vector<uint8_t>& data)
{
    glBindTexture(GL_TEXTURE_2D, gContext.texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
}
