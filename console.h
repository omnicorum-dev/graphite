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

    u32 SCREEN_WIDTH;
    u32 SCREEN_HEIGHT;
    u32 GAME_WIDTH;
    u32 GAME_HEIGHT;

    Graphite::Canvas canvas;

    virtual ~Console() = default;

    void consoleInit(const u32 consoleWidth, const u32 consoleHeight, const u32 gameWidth, const u32 gameHeight) {
        SCREEN_WIDTH = consoleWidth;
        SCREEN_HEIGHT = consoleHeight;
        GAME_WIDTH = gameWidth;
        GAME_HEIGHT = gameHeight;
    }

    virtual u32 consoleStartup() = 0;
    virtual void consoleRun(std::function<Graphite::Canvas&(f32)> gameUpdate) = 0;
    virtual u32 consoleShutdown() = 0;
};

#endif //CONSOLE_H
