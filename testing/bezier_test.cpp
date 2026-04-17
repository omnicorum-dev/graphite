//
// Created by Nico Russo on 4/16/26.
//
#include <graphite.h>
#include <base.h>
#include <random>

using namespace omni;

Graphite::Canvas mainCanvas(800, 600);

int main() {
    mainCanvas.fillFast(0xff202020);

    const Vec2<float> p1 = {100, 400};
    const Vec2<float> p2 = {400, 100};
    const Vec2<float> p3 = {700, 500};

    mainCanvas.drawPoint(p1.x, p1.y, 10, Graphite::Colors::Red);
    mainCanvas.drawPoint(p2.x, p2.y, 10, Graphite::Colors::Red);
    mainCanvas.drawPoint(p3.x, p3.y, 10, Graphite::Colors::Red);

    for (float t = 0; t <= 1.01; t += 0.04) {
        Vec2<float> p = bezierQuadratic(p1, p2, p3, t);
        mainCanvas.drawPoint(p.x, p.y, 5, Graphite::Colors::Brown);
    }

    if (!mainCanvas.saveToJPG("../bezier.jpg")) {
        LOG_ERROR("Failed to save bezier image");
        return 1;
    }

    return 0;
}