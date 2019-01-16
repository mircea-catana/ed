#include <GL/glew.h>

#include <GLFW/glfw3.h>
static GLFWwindow* window;

#include "vector.h"
#include "framebuffer.h"
#include "model.h"
#include "rasterizer.h"

bool initGL();
void keyboard_input();
void drawQuad();

int main(int argc, char* argv[])
{
    using namespace ed;

    if (argc != 2) {
        std::cerr << "Usage ./ed <obj file>" << std::endl;
        return 0;
    }

    Color red(255u, 120u, 0u);
    Color green(20u, 255u, 120u);
    Color gray(50u, 50u, 50u);

    Model model(argv[1]);

    Framebuffer framebuffer(1000u, 1000u);
    framebuffer.output().fill(gray);

    const uint32_t halfWidth  = framebuffer.output().width()  * 0.5f;
    const uint32_t halfHeight = framebuffer.output().height() * 0.5f;

    for (uint32_t i = 0u; i < model.numFaces(); ++i) {
        const std::vector<uint32_t>& face = model.face(i);

        Vector3 v0 = model.vertex(face[0]);
        Vector3 v1 = model.vertex(face[1]);
        Vector3 v2 = model.vertex(face[2]);

        Vector3 edge0 = normalize(v0 - v1);
        Vector3 edge1 = normalize(v0 - v2);
        Vector3 normal = cross(edge0, edge1);

        if (dot(normal, Vector3(0.0f, 0.0f, -1.0f)) < 0.0f)
            continue;

        float x0 = (v0.x() * 0.7f + 1.0f) * halfWidth;
        float y0 = (v0.y() * 0.7f + 1.0f) * halfHeight;
        float x1 = (v1.x() * 0.7f + 1.0f) * halfWidth;
        float y1 = (v1.y() * 0.7f + 1.0f) * halfHeight;
        float x2 = (v2.x() * 0.7f + 1.0f) * halfWidth;
        float y2 = (v2.y() * 0.7f + 1.0f) * halfHeight;

        drawLine(framebuffer.output(), Vector2(x0, y0), Vector2(x1, y1), green);
        drawLine(framebuffer.output(), Vector2(x0, y0), Vector2(x2, y2), green);
        drawLine(framebuffer.output(), Vector2(x1, y1), Vector2(x2, y2), green);
    }


    framebuffer.output().writePPM("out.ppm");

    /********* GL *********/

    if (!initGL()) {
        return 0;
    }

    glClearColor(0.12f, 0.11f, 0.14f, 0.0f);

    while (glfwWindowShouldClose(window) == 0) {
        keyboard_input();

        glClear(GL_COLOR_BUFFER_BIT);
        drawQuad();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
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
    window = glfwCreateWindow(1024, 768, "W1S1", nullptr, nullptr);
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

void drawQuad() {

}
