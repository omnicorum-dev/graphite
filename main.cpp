#include "graphite.h"
#include <fstream>

using namespace omni;

constexpr u32 WIDTH = 800;
constexpr u32 HEIGHT = 600;


int main() {
    println("Hello, World!");

    Graphite::Canvas canvas(WIDTH, HEIGHT);

    const Graphite::Color BACKGROUND = {0xff202020};
    const Graphite::Color RED = {0xff3030ff};
    const Graphite::Color GREEN = {0xff50ff50};
    const Graphite::Color BLUE = {0xffff3030};

    const Graphite::Color HALF_RED = {0x770000ff};
    const Graphite::Color HALF_GREEN = {0xbb00ff00};
    const Graphite::Color HALF_BLUE = {0x44ff0000};

    canvas.fill(BACKGROUND);

    canvas.fillCircle(canvas.getWidth()/2 - 100, canvas.getHeight()/2 - 75, 150, RED);
    canvas.fillCircle(canvas.getWidth()/2 + 100, canvas.getHeight()/2 - 75, 150, HALF_GREEN);
    canvas.fillCircle(canvas.getWidth()/2 + 0, canvas.getHeight()/2 + 75, 150, HALF_BLUE);

    const std::string filename = "../output.ppm";
    if (!canvas.saveToPPM(filename)) {
        LOG_ERROR("Could not save output file {}", filename);
    }
    return 0;
}