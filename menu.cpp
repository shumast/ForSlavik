#include "ui.h"

void showMenu() {
    sf::RenderWindow menuWindow(sf::VideoMode(400, 420), "Menu", sf::Style::Close);
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text title("Settings", font, 30);
    title.setFillColor(sf::Color::Black);
    title.setPosition(100, 20);

    sf::Text boardWidthText("DeskWidth:", font, 20);
    boardWidthText.setFillColor(sf::Color::Black);
    boardWidthText.setPosition(50, 80);

    sf::Text gridWidthText(std::to_string(N_GRID_SIZE), font, 20);
    gridWidthText.setFillColor(sf::Color::Black);
    gridWidthText.setPosition(250, 80);

    sf::Text boardHeightText("DeskHeight:", font, 20);
    boardHeightText.setFillColor(sf::Color::Black);
    boardHeightText.setPosition(50, 120);

    sf::Text gridHeightText(std::to_string(M_GRID_SIZE), font, 20);
    gridHeightText.setFillColor(sf::Color::Black);
    gridHeightText.setPosition(250, 120);

    sf::Text winBlackConditionText("Black win condition: ", font, 20);
    winBlackConditionText.setFillColor(sf::Color::Black);
    winBlackConditionText.setPosition(50, 160);

    sf::Text conditionBlackText(std::to_string(NEED_FIRST), font, 20);
    conditionBlackText.setFillColor(sf::Color::Black);
    conditionBlackText.setPosition(250, 160);

    sf::Text winWhiteConditionText("White win condition: ", font, 20);
    winWhiteConditionText.setFillColor(sf::Color::Black);
    winWhiteConditionText.setPosition(50, 200);

    sf::Text conditionWhiteText(std::to_string(NEED_SECOND), font, 20);
    conditionWhiteText.setFillColor(sf::Color::Black);
    conditionWhiteText.setPosition(250, 200);

    std::string firstPlayerText1 = "First human";
    if (FIRST_PLAYER == 1) {
        firstPlayerText1 = "First easy bot";
    }
    else if (FIRST_PLAYER == 2) {
        firstPlayerText1 = "First medium bot";
    }
    else if (FIRST_PLAYER == 3) {
        firstPlayerText1 = "First hard bot";
    }
    sf::Text turnText1(firstPlayerText1, font, 20);
    turnText1.setFillColor(sf::Color::Black);
    turnText1.setPosition(50, 240);

    std::string firstPlayerText2 = "Second human";
    if (SECOND_PLAYER == 1) {
        firstPlayerText2 = "Second easy bot";
    }
    else if (SECOND_PLAYER == 2) {
        firstPlayerText2 = "Second medium bot";
    }
    else if (SECOND_PLAYER == 3) {
        firstPlayerText2 = "Second hard bot";
    }
    sf::Text turnText2(firstPlayerText2, font, 20);
    turnText2.setFillColor(sf::Color::Black);
    turnText2.setPosition(50, 285);

    std::string firstPlayerText3 = "Ordinary queue"; // (WB)
    if (QUEUE == 1) {
        firstPlayerText3 = "Marcel queue"; // W(BBWW)
    }
    else if (QUEUE == 2) {
        firstPlayerText3 = "(2, 2) queue"; // (WWBB)
    }
    else if (QUEUE == 3) {
        firstPlayerText3 = "Progressive queue"; // WBBWWWBBBB...
    }
    else if (QUEUE == 4) {
        firstPlayerText3 = "Random queue"; // with probability p
    }
    sf::Text turnText3(firstPlayerText3, font, 20);
    turnText3.setFillColor(sf::Color::Black);
    turnText3.setPosition(50, 330);

    std::string firstPlayerText4 = "Probability queue";
    sf::Text turnText4(firstPlayerText4, font, 20);
    turnText4.setFillColor(sf::Color::Black);
    turnText4.setPosition(50, 375);

    sf::RectangleShape inputBox(sf::Vector2f(100, 30));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(250, 375);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setFillColor(sf::Color::Black);
    inputText.setCharacterSize(24);
    inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 1);
    std::string currentInput = "0.5";
    inputText.setString(currentInput);

    sf::RectangleShape boardWidthButtonUp(sf::Vector2f(30, 30));
    boardWidthButtonUp.setPosition(320, 80);
    boardWidthButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape boardWidthButtonDown(sf::Vector2f(30, 30));
    boardWidthButtonDown.setPosition(285, 80);
    boardWidthButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape boardHeightButtonUp(sf::Vector2f(30, 30));
    boardHeightButtonUp.setPosition(320, 120);
    boardHeightButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape boardHeightButtonDown(sf::Vector2f(30, 30));
    boardHeightButtonDown.setPosition(285, 120);
    boardHeightButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape conditionBlackButtonUp(sf::Vector2f(30, 30));
    conditionBlackButtonUp.setPosition(320, 160);
    conditionBlackButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape conditionBlackButtonDown(sf::Vector2f(30, 30));
    conditionBlackButtonDown.setPosition(285, 160);
    conditionBlackButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape conditionWhiteButtonUp(sf::Vector2f(30, 30));
    conditionWhiteButtonUp.setPosition(320, 200);
    conditionWhiteButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape conditionWhiteButtonDown(sf::Vector2f(30, 30));
    conditionWhiteButtonDown.setPosition(285, 200);
    conditionWhiteButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape toggleTurnButton1(sf::Vector2f(100, 30));
    toggleTurnButton1.setPosition(250, 240);
    toggleTurnButton1.setFillColor(sf::Color::Black);

    sf::RectangleShape toggleTurnButton2(sf::Vector2f(100, 30));
    toggleTurnButton2.setPosition(250, 285);
    toggleTurnButton2.setFillColor(sf::Color::Red);

    sf::RectangleShape toggleTurnButton3(sf::Vector2f(100, 30));
    toggleTurnButton3.setPosition(250, 330);
    toggleTurnButton3.setFillColor(sf::Color::Blue);

    while (menuWindow.isOpen()) {
        sf::Event event;
        while (menuWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                PROBABILITY = std::stof(currentInput);
                if (QUEUE == 0) { // (WB)
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        sophisticated_queue.push_back(i & 1);
                    }
                }
                else if (QUEUE == 1) { // W(BBWW)
                    sophisticated_queue.push_back(0);
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE - 1; i++) {
                        if (i % 4 < 2) {
                            sophisticated_queue.push_back(1);
                        }
                        else {
                            sophisticated_queue.push_back(0);
                        }
                    }
                }
                else if (QUEUE == 2) { // (WWBB)
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        if (i % 4 < 2) {
                            sophisticated_queue.push_back(0);
                        }
                        else {
                            sophisticated_queue.push_back(1);
                        }
                    }
                }
                else if (QUEUE == 3) { // WBBWWWBBBB...
                    int turn = 0, cap = 1;
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        if (cap > 0) {
                            sophisticated_queue.push_back(turn & 1);
                            cap--;
                        }
                        else {
                            turn++;
                            cap = turn;
                            sophisticated_queue.push_back(turn & 1);
                        }
                    }
                }
                else if (QUEUE == 4) { // with probability p
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        if ((double)gen() / RAND_MAX / RAND_MAX / 4 < PROBABILITY) {
                            sophisticated_queue.push_back(0);
                        }
                        else {
                            sophisticated_queue.push_back(1);
                        }
                    }
                }
                sophisticated_queue.push_back(1 - sophisticated_queue.back());
                menuWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // increasing sizes
                    if (boardWidthButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        N_GRID_SIZE++;
                        N_WINDOW_SIZE = N_GRID_SIZE * CELL_SIZE;
                        gridWidthText.setString(std::to_string(N_GRID_SIZE));
                    }
                    else if (boardHeightButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        M_GRID_SIZE++;
                        M_WINDOW_SIZE = M_GRID_SIZE * CELL_SIZE;
                        gridHeightText.setString(std::to_string(M_GRID_SIZE));
                    }
                    // decreasing sizes
                    else if (boardWidthButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        N_GRID_SIZE--;
                        N_WINDOW_SIZE = N_GRID_SIZE * CELL_SIZE;
                        gridWidthText.setString(std::to_string(N_GRID_SIZE));
                    }
                    else if (boardHeightButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        M_GRID_SIZE--;
                        M_WINDOW_SIZE = M_GRID_SIZE * CELL_SIZE;
                        gridHeightText.setString(std::to_string(M_GRID_SIZE));
                    }
                    // increasing win condition
                    else if (conditionBlackButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_FIRST++;
                        conditionBlackText.setString(std::to_string(NEED_FIRST));
                    }
                    else if (conditionWhiteButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_SECOND++;
                        conditionWhiteText.setString(std::to_string(NEED_SECOND));
                    }
                    // decreasing win condition
                    else if (conditionBlackButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_FIRST--;
                        conditionBlackText.setString(std::to_string(NEED_FIRST));
                    }
                    else if (conditionWhiteButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_SECOND--;
                        conditionWhiteText.setString(std::to_string(NEED_SECOND));
                    }
                    // choose first player or bot
                    else if (toggleTurnButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        FIRST_PLAYER = (FIRST_PLAYER + 1) % 4;
                        if (FIRST_PLAYER == 1) {
                            firstPlayerText1 = "First easy bot";
                        }
                        else if (FIRST_PLAYER == 2) {
                            firstPlayerText1 = "First medium bot";
                        }
                        else if (FIRST_PLAYER == 3) {
                            firstPlayerText1 = "First hard bot";
                        }
                        else {
                            firstPlayerText1 = "First human";
                        }
                        turnText1.setString(firstPlayerText1);
                    }
                    else if (toggleTurnButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        SECOND_PLAYER = (SECOND_PLAYER + 1) % 4;
                        if (SECOND_PLAYER == 1) {
                            firstPlayerText2 = "Second easy bot";
                        }
                        else if (SECOND_PLAYER == 2) {
                            firstPlayerText2 = "Second medium bot";
                        }
                        else if (SECOND_PLAYER == 3) {
                            firstPlayerText2 = "Second hard bot";
                        }
                        else {
                            firstPlayerText2 = "Second human";
                        }
                        turnText2.setString(firstPlayerText2);
                    }
                    else if (toggleTurnButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        QUEUE = (QUEUE + 1) % 5;
                        if (QUEUE == 1) {
                            firstPlayerText3 = "Marcel queue"; // W(BBWW)
                        }
                        else if (QUEUE == 2) {
                            firstPlayerText3 = "(2, 2) queue"; // (WWBB)
                        }
                        else if (QUEUE == 3) {
                            firstPlayerText3 = "Progressive queue"; // WBBWWWBBBB...
                        }
                        else if (QUEUE == 4) {
                            firstPlayerText3 = "Random queue"; // with probability p
                        }
                        else {
                            firstPlayerText3 = "Ordinary queue"; // (WB)
                        }
                        turnText3.setString(firstPlayerText3);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                // Обработка клавиши Backspace (ASCII 8)
                if (event.text.unicode == 8) {
                    if (!currentInput.empty())
                        currentInput.pop_back();
                }
                // Обработка вводимых символов (ASCII < 128)
                else if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    // Разрешаем ввод только цифр, точки и знака минус
                    if ((enteredChar >= '0' && enteredChar <= '9') || enteredChar == '.') {
                        // Разрешаем только одну точку
                        if (enteredChar == '.' && currentInput.find('.') != std::string::npos)
                            continue;
                        currentInput += enteredChar;
                    }
                }
                // Обновляем отображаемый текст
                inputText.setString(currentInput);
            }
        }

        menuWindow.clear(sf::Color::White);
        menuWindow.draw(title);
        menuWindow.draw(boardWidthText);
        menuWindow.draw(gridWidthText);
        menuWindow.draw(boardHeightText);
        menuWindow.draw(gridHeightText);
        menuWindow.draw(winBlackConditionText);
        menuWindow.draw(conditionBlackText);
        menuWindow.draw(winWhiteConditionText);
        menuWindow.draw(conditionWhiteText);
        menuWindow.draw(turnText1);
        menuWindow.draw(turnText2);
        menuWindow.draw(turnText3);
        menuWindow.draw(turnText4);
        menuWindow.draw(boardWidthButtonUp);
        menuWindow.draw(boardWidthButtonDown);
        menuWindow.draw(boardHeightButtonUp);
        menuWindow.draw(boardHeightButtonDown);
        menuWindow.draw(conditionBlackButtonUp);
        menuWindow.draw(conditionBlackButtonDown);
        menuWindow.draw(conditionWhiteButtonUp);
        menuWindow.draw(conditionWhiteButtonDown);
        menuWindow.draw(toggleTurnButton1);
        menuWindow.draw(toggleTurnButton2);
        menuWindow.draw(toggleTurnButton3);
        menuWindow.draw(inputBox);
        menuWindow.draw(inputText);
        menuWindow.display();
    }
}
