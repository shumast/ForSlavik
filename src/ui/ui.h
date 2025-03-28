#pragma once
#include "../xyk/xyk.h"
#include "../../lib/Graphics.hpp"
#include <future>
#include <thread>
#include <windows.h>

void drawBoard(sf::RenderWindow& window, const XYK& game);

void showMenu();

void showWinnerWindow(sf::RenderWindow& window, int winner);

void newWindow();
