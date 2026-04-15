#include "graphite.h"
#include "consoleOpenGL.h"
#include <fstream>

using namespace omni;

constexpr u32 CONSOLE_WIDTH = 800;
constexpr u32 CONSOLE_HEIGHT = 600;
constexpr u32 WIDTH = CONSOLE_WIDTH / 2;
constexpr u32 HEIGHT = CONSOLE_HEIGHT / 2;

constexpr f32 GRAVITY = 750;

struct Ball {
    Vec2<float> position = {};
    Vec2<float> velocity = {};
    Graphite::Color color = 0xff000000;
};


ConsoleOpenGL console(CONSOLE_WIDTH, CONSOLE_HEIGHT);
Graphite::Canvas gameCanvas(WIDTH, HEIGHT);
Ball ball = {{100, 75}, {200, -300}, 0xff3030ff};

void game_update(const f32 dt) {
    gameCanvas.fill(0xff000000);

    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;
    ball.velocity.y += GRAVITY * dt;

    if (ball.position.y > HEIGHT) {
        ball.position.y = HEIGHT;
        ball.velocity.y = -ball.velocity.y * 0.95f;
    }

    if (ball.position.x > WIDTH) {
        ball.position.x = WIDTH;
        ball.velocity.x = -ball.velocity.x * 0.95f;
    }
    if (ball.position.x < 0) {
        ball.position.x = 0;
        ball.velocity.x = -ball.velocity.x * 0.95f;
    }

    gameCanvas.fillCircle(ball.position.x, ball.position.y, 3, ball.color);

    console.consoleCanvas.drawCanvas(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, gameCanvas);
}

int main() {

    println("Hello, world");

    console.setWindowTitle("THE GAME");
    if (console.consoleInit() != 0) {
        LOG_ERROR("Failed to initialize console");
        return 1;
    }

    console.consoleRun(game_update);

    if (console.consoleShutdown() != 0) {
        LOG_ERROR("Failed to shutdown console");
        return 1;
    }


    return 0;
/*
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Graphite", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    init_buffers(WIDTH, HEIGHT);
    init_texture(WIDTH, HEIGHT);

    const char* vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentSrc = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;

        uniform sampler2D screenTexture;

        void main() {
            FragColor = texture(screenTexture, TexCoord);
        }
    )";

    GLuint shader = createShaderProgram(vertexSrc, fragmentSrc);
    GLuint vao = createFullscreenQuadVAO();

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0, 0, 0, 1);

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0, 0, 0, 1);

    run(window, shader, vao);

    delete[] front_buffer;
    delete[] back_buffer;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;






    println("Hello, World!");

    Graphite::Canvas canvas(WIDTH, HEIGHT);
    Graphite::Canvas sprite(80, 80);
    Graphite::Canvas tmbg("../john_henry_album.jpg");

    const Graphite::Color BACKGROUND = {0xff202020};
    const Graphite::Color RED = {0xff3030ff};
    const Graphite::Color GREEN = {0xff50ff50};
    const Graphite::Color BLUE = {0xffff3030};

    const Graphite::Color HALF_RED = {0x770000ff};
    const Graphite::Color HALF_GREEN = {0xbb00ff00};
    const Graphite::Color HALF_BLUE = {0x44ff0000};

    sprite.fillCircle(40, 40, 40, BLUE);

    canvas.fill(BACKGROUND);
    canvas.fillEllipse(canvas.getWidth()/2, canvas.getHeight()/2, 300, 100, RED);
    canvas.drawCanvas(100, 100, tmbg.getWidth(), tmbg.getHeight(), tmbg);

    const std::string filename = "../output.ppm";
    if (!canvas.saveToPPM(filename)) {
        LOG_ERROR("Could not save output file {}", filename);
    }
    return 0;
    */
}