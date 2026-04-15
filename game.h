//
// Created by Nico Russo on 4/15/26.
//

#ifndef GAME_H
#define GAME_H

#include "graphite.h"
#include "consoleOpenGL.h"

using namespace omni;

class Game {
public:
    u32 CONSOLE_WIDTH;
    u32 CONSOLE_HEIGHT;
    u32 GAME_WIDTH;
    u32 GAME_HEIGHT;

    ConsoleOpenGL console;
    Graphite::Canvas gameCanvas;

    Game(const u32 consoleWidth, const u32 consoleHeight, const u32 gameWidth, const u32 gameHeight) {
        CONSOLE_WIDTH = consoleWidth;
        CONSOLE_HEIGHT = consoleHeight;
        GAME_WIDTH = gameWidth;
        GAME_HEIGHT = gameHeight;
        console.consoleInit(CONSOLE_WIDTH, CONSOLE_HEIGHT);
        gameCanvas.newCanvas(GAME_WIDTH, GAME_HEIGHT);
    }
};

#endif //GAME_H
