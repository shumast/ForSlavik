#pragma once
#include "xyk.h"
#include "Graphics.hpp"
#include <thread>
#include <windows.h>

void drawBoard(sf::RenderWindow& window, const XYK& game);

void showMenu();

void showWinnerWindow(sf::RenderWindow& window, int winner);

void newWindow();
