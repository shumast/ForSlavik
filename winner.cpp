#include "ui.h"

void showWinnerWindow(sf::RenderWindow& window, int winner) {
    sf::RenderWindow winnerWindow(sf::VideoMode(300, 200), "Won");

    sf::Font font;
    font.loadFromFile("arial.ttf");
    std::string message = "Draw!";
    if (winner == 0) {
        message = "Black won!";
    }
    if (winner == 1) {
        message = "Red won!";
    }
    sf::Text text(message, font, 30);
    text.setFillColor(sf::Color::Black);
    text.setPosition(50, 50);

    sf::RectangleShape restartButton(sf::Vector2f(100, 50));
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setPosition(40, 120);

    sf::Text buttonText("Restart", font, 20);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(50, 135);

    sf::RectangleShape menuButton(sf::Vector2f(100, 50));
    menuButton.setFillColor(sf::Color::Yellow);
    menuButton.setPosition(140, 120);

    sf::Text buttonMenuText("Menu", font, 20);
    buttonMenuText.setFillColor(sf::Color::Black);
    buttonMenuText.setPosition(160, 135);

    while (winnerWindow.isOpen()) {
        sf::Event event;
        while (winnerWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                winnerWindow.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::FloatRect buttonBounds = restartButton.getGlobalBounds();
                    if (buttonBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                        winnerWindow.close();
                    }
                    sf::FloatRect buttonMenuBounds = menuButton.getGlobalBounds();
                    if (buttonMenuBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                        showMenu();
                    }
                }
            }
        }

        winnerWindow.clear(sf::Color::White);
        winnerWindow.draw(text);
        winnerWindow.draw(restartButton);
        winnerWindow.draw(buttonText);
        winnerWindow.draw(menuButton);
        winnerWindow.draw(buttonMenuText);
        winnerWindow.display();
    }
}
