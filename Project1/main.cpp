#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

const int CELL_SIZE = 50; 
const int GRID_SIZE = 9;   
const int WINDOW_SIZE = CELL_SIZE * GRID_SIZE + 50;

std::vector<std::vector<int>> initializeSudoku() {
    std::vector<std::vector<int>> grid = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    return grid;
}

std::string formatTime(int seconds) {
    int minutes = seconds / 60;
    int sec = seconds % 60;
    return std::to_string(minutes) + ":" + (sec < 10 ? "0" : "") + std::to_string(sec);
}

bool isValidNumber(int num, int row, int col, const std::vector<std::vector<int>>& grid) {
    if (num < 1 || num > 9) return false;

    for (int i = 0; i < GRID_SIZE; i++) {
        if (grid[row][i] == num || grid[i][col] == num)
            return false;
    }

    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[blockRow + i][blockCol + j] == num)
                return false;
        }
    }

    return true;
}

bool solveSudoku(std::vector<std::vector<int>>& grid) {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            if (grid[row][col] == 0) { 
                for (int num = 1; num <= 9; ++num) {
                    if (isValidNumber(num, row, col, grid)) {
                        grid[row][col] = num;

                        if (solveSudoku(grid)) {
                            return true;
                        }

                        grid[row][col] = 0;
                    }
                }
                return false; 
            }
        }
    }
    return true;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE + 50), "Sudoku");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!" << std::endl;
        return -1;
    }

    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(10, WINDOW_SIZE);


    std::vector<std::vector<int>> grid = initializeSudoku();

    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    cellShape.setOutlineThickness(2);
    cellShape.setOutlineColor(sf::Color::Black);

    int selectedRow = 0, selectedCol = 0; 
    int inputNumber = 0; 

    sf::Clock gameClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedRow = (selectedRow - 1 + GRID_SIZE) % GRID_SIZE;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    selectedRow = (selectedRow + 1) % GRID_SIZE;
                }
                if (event.key.code == sf::Keyboard::Left) {
                    selectedCol = (selectedCol - 1 + GRID_SIZE) % GRID_SIZE;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    selectedCol = (selectedCol + 1) % GRID_SIZE;
                }

                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                    inputNumber = event.key.code - sf::Keyboard::Num0;
                    if (grid[selectedRow][selectedCol] == 0 && isValidNumber(inputNumber, selectedRow, selectedCol, grid)) {
                        grid[selectedRow][selectedCol] = inputNumber;
                    }
                }

                if (event.key.code == sf::Keyboard::S) {
                    if (solveSudoku(grid)) {
                        std::cout << "Судоку решено!" << std::endl;
                    }
                    else {
                        std::cout << "Судоку не имеет решения!" << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                cellShape.setPosition(col * CELL_SIZE + 5, row * CELL_SIZE + 5);

                if (row == selectedRow && col == selectedCol) {
                    cellShape.setFillColor(sf::Color(200, 200, 200));
                }
                else {
                    cellShape.setFillColor(sf::Color::White);
                }

                window.draw(cellShape);

                if (grid[row][col] != 0) {
                    sf::Text numberText;
                    numberText.setFont(font);
                    numberText.setCharacterSize(24);
                    numberText.setFillColor(sf::Color::Black);
                    numberText.setString(std::to_string(grid[row][col]));
                    numberText.setPosition(col * CELL_SIZE + 15, row * CELL_SIZE + 5);
                    window.draw(numberText);
                }
            }
        }

        int elapsedTime = gameClock.getElapsedTime().asSeconds();
        timerText.setString("Time: " + formatTime(elapsedTime));
        window.draw(timerText);

        window.display();
    }

    return 0;
}
