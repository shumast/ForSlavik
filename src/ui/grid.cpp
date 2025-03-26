#include "ui.h"

void drawBoard(sf::RenderWindow& window, const XYK& game) {
    window.clear(sf::Color::White);
    for (int i = 0; i < M_GRID_SIZE; i++) {
        for (int j = 0; j < N_GRID_SIZE; j++) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            cell.setOutlineThickness(BORDER);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor(sf::Color::Transparent);
            window.draw(cell);

            if (game.getBoard()[i][j] == 0) {
                sf::CircleShape circle(CELL_SIZE / 2 - 2 * BORDER);
                circle.setPosition(j * CELL_SIZE + 2 * BORDER, i * CELL_SIZE + 2 * BORDER);
                circle.setOutlineThickness(BORDER);
                circle.setOutlineColor(sf::Color::Black);
                circle.setFillColor(sf::Color::Transparent);
                window.draw(circle);
            }
            else if (game.getBoard()[i][j] == 1) {
                sf::CircleShape circle(CELL_SIZE / 2 - 2 * BORDER);
                circle.setPosition(j * CELL_SIZE + 2 * BORDER, i * CELL_SIZE + 2 * BORDER);
                circle.setOutlineThickness(BORDER);
                circle.setOutlineColor(sf::Color::Red);
                circle.setFillColor(sf::Color::Transparent);
                window.draw(circle);
            }
        }
    }
    sf::RectangleShape boardBorder(sf::Vector2f(N_GRID_SIZE * CELL_SIZE - 2 * BORDER, M_GRID_SIZE * CELL_SIZE - 2 * BORDER));
    boardBorder.setPosition(BORDER, BORDER);
    boardBorder.setOutlineThickness(BORDER);
    boardBorder.setOutlineColor(sf::Color::Black);
    boardBorder.setFillColor(sf::Color::Transparent);
    window.draw(boardBorder);
    static sf::Font font;
    font.loadFromFile("arial.ttf");
    std::vector<std::pair<int, int>> moves = game.getMoves();
    reverse(moves.begin(), moves.end());
    float movesStartX = N_GRID_SIZE * CELL_SIZE + 10;
    float movesStartY = 40;
    sf::Text moveText;
    moveText.setFont(font);
    moveText.setString("Moves:");
    moveText.setCharacterSize(13);
    moveText.setFillColor(sf::Color::Black);
    moveText.setPosition(N_GRID_SIZE * CELL_SIZE + 7, 10);
    window.draw(moveText);
    int moveIndex = 0;
    for (const auto& move : moves) {
        if (movesStartY + moveIndex * 30 >= M_WINDOW_SIZE) {
            break;
        }
        sf::Text moveText;
        moveText.setFont(font);
        moveText.setString(std::to_string(move.first + 1) + " " + std::to_string(move.second + 1));
        moveText.setCharacterSize(15);
        moveText.setFillColor(sf::Color::Black);
        moveText.setPosition(movesStartX, movesStartY + moveIndex * 30);
        window.draw(moveText);
        moveIndex++;
    }
    const std::vector<int>& queue = sophisticated_queue;
    float queueStartX = 10;
    float queueStartY = M_GRID_SIZE * CELL_SIZE + 10;
    int queueIndex = 0;
    for (int i = currentTurn; i < sophisticated_queue.size(); i++) {
        if (queueStartX + queueIndex * 20 >= N_WINDOW_SIZE) {
            break;
        }
        sf::Text queueText;
        queueText.setFont(font);
        std::string str = "B";
        if (sophisticated_queue[i] == 1) {
            str = "R";
        }
        queueText.setString(str);
        queueText.setCharacterSize(20);
        if (str == "B") {
            queueText.setFillColor(sf::Color::Black);
        }
        else {
            queueText.setFillColor(sf::Color::Red);
        }
        queueText.setPosition(queueStartX + queueIndex * 20, queueStartY);
        window.draw(queueText);
        queueIndex++;
    }

    window.display();
}
