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
                    Sleep(100);

                    //game.makeRandomCloseMove();
                    //game.makeMinimaxMove();
                    //game.makeMinimaxWithAlphaBetaMove();
                    //game.makeMinimaxWithDepthMove();
                    //game.makeMinimaxWithDepthAndTimeMove();
                    game.makeGreedySearchMove();
                    //game.makeRadialSearchMove();
                }
                else if (FIRST_PLAYER == 2) {
                    Sleep(100);

                    game.makeGreedySearch2Move();
                    //game.makeMinimaxWithDepth2Move();
                }
                else if (FIRST_PLAYER == 3) {
                    Sleep(100);

                    game.makeMCTSmove();
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
                    Sleep(100);

                    //game.makeRandomCloseMove();
                    //game.makeMinimaxMove();
                    //game.makeMinimaxWithAlphaBetaMove();
                    //game.makeMinimaxWithDepthMove();
                    //game.makeMinimaxWithDepthAndTimeMove();
                    game.makeGreedySearchMove();
                    //game.makeRadialSearchMove();
                }
                else if (SECOND_PLAYER == 2) {
                    Sleep(100);

                    game.makeGreedySearch2Move();
                    //game.makeMinimaxWithDepth2Move();
                }
                else if (SECOND_PLAYER == 3) {
                    Sleep(100);

                    game.makeMCTSmove();
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
