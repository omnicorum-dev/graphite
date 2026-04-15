//
// Created by Nico Russo on 4/15/26.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include "base.h"
#include "graphite.h"
using namespace omni::basic;

class Console {
public:
    u32* backBuffer;

    u32 CONSOLE_WIDTH;
    u32 CONSOLE_HEIGHT;

    Graphite::Canvas consoleCanvas;

    virtual ~Console() = default;

    void consoleInit(const u32 consoleWidth, const u32 consoleHeight) {
        CONSOLE_WIDTH = consoleWidth;
        CONSOLE_HEIGHT = consoleHeight;
    }

    virtual u32 consoleStartup() = 0;
    virtual void consoleRun(std::function<Graphite::Canvas&(f32)> gameUpdate) = 0;
    virtual u32 consoleShutdown() = 0;
};

#endif //CONSOLE_H
