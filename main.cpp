#include "graphite.h"
#include <fstream>

using namespace omni;

constexpr u32 WIDTH = 800;
constexpr u32 HEIGHT = 600;


int main() {
    println("Hello, World!");

    Graphite::Canvas canvas(WIDTH, HEIGHT);

    Graphite::Color BACKGROUND = {0xff202020};
    Graphite::Color FOREGROUND = {0xff3030ff};

    canvas.fill(BACKGROUND);

    canvas.fillCircle(canvas.getWidth()/2, canvas.getHeight()/2, 100, FOREGROUND);
    canvas.drawLine(0, 0, canvas.getWidth(), canvas.getHeight(), FOREGROUND);

    const std::string filename = "../output.ppm";
    if (!canvas.saveToPPM(filename)) {
        LOG_ERROR("Could not save output file {}", filename);
    }
    return 0;
}