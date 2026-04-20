//
// Created by Nico Russo on 4/19/26.
//

#include <graphite.h>

int main() {
    const Graphite::Canvas canvas(400, 300);

    canvas.fill(Graphite::Colors::Black);

    canvas.writeStringBaseline("This is a test",
        {10, 50}, 16, Graphite::Colors::White);

    if (!canvas.saveToJPG("../testing/new_output.jpg")) {
        omni::LOG_ERROR("Failed to save to JPEG");
        return 1;
    }

    return 0;
}