#include "ui.h"

void newWindow() {
    sf::RenderWindow window(sf::VideoMode(N_WINDOW_SIZE + CELL_SIZE, M_WINDOW_SIZE + CELL_SIZE), "XYK");
    XYK game;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (sophisticated_queue[currentTurn] == 0) {
                if (FIRST_PLAYER == 0) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            int x = event.mouseButton.x / CELL_SIZE;
                            int y = event.mouseButton.y / CELL_SIZE;
                            game.makeMove(y, x);
                        }
                    }
                }
                else if (FIRST_PLAYER == 1) {
                    std::future<void> result;
                    //result = std::async(std::launch::async, &XYK::makeRandomCloseMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithAlphaBetaMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepthMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepthAndTimeMove, &game);
                    result = std::async(std::launch::async, &XYK::makeGreedySearchMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeRadialSearchMove, &game);
                    result.get();
                }
                else if (FIRST_PLAYER == 2) {
                    std::future<void> result;
                    result = std::async(std::launch::async, &XYK::makeGreedySearch2Move, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepth2Move, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepth3Move, &game);
                    result.get();
                }
                else if (FIRST_PLAYER == 3) {
                    std::future<void> result;
                    result = std::async(std::launch::async, &XYK::makeMCTSmove, &game);
                    result.get();
                }
            }
            else if (sophisticated_queue[currentTurn] == 1) {
                if (SECOND_PLAYER == 0) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            int x = event.mouseButton.x / CELL_SIZE;
                            int y = event.mouseButton.y / CELL_SIZE;
                            game.makeMove(y, x);
                        }
                    }
                }
                else if (SECOND_PLAYER == 1) {
                    std::future<void> result;
                    //result = std::async(std::launch::async, &XYK::makeRandomCloseMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithAlphaBetaMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepthMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepthAndTimeMove, &game);
                    result = std::async(std::launch::async, &XYK::makeGreedySearchMove, &game);
                    //result = std::async(std::launch::async, &XYK::makeRadialSearchMove, &game);
                    result.get();
                }
                else if (SECOND_PLAYER == 2) {
                    std::future<void> result;
                    result = std::async(std::launch::async, &XYK::makeGreedySearch2Move, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepth2Move, &game);
                    //result = std::async(std::launch::async, &XYK::makeMinimaxWithDepth3Move, &game);
                    result.get();
                }
                else if (SECOND_PLAYER == 3) {
                    std::future<void> result;
                    result = std::async(std::launch::async, &XYK::makeMCTSmove, &game);
                    result.get();
                }
            }
            drawBoard(window, game);
            if (!game.canWin()) {
                showWinnerWindow(window, 2);
                window.clear();
                game.reset();
            }
            if (game.getWinner() != EMPTY) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                showWinnerWindow(window, game.getWinner());
                window.clear();
                game.reset();
            }
            if (window.getSize().x != N_WINDOW_SIZE + CELL_SIZE || window.getSize().y != M_WINDOW_SIZE + CELL_SIZE) {
                window.close();
                newWindow();
            }
        }
    }
}
