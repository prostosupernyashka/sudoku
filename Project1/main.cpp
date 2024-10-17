#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

const int CELL_SIZE = 50;   // Размер каждой клетки
const int GRID_SIZE = 9;    // Размер сетки (9x9)
const int WINDOW_SIZE = CELL_SIZE * GRID_SIZE + 10;  // Размер окна

// Функция для инициализации судоку
std::vector<std::vector<int>> initializeSudoku() {
    // Пример начальной конфигурации судоку с пустыми клетками
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

// Функция для отображения времени в формате "минуты:секунды"
std::string formatTime(int seconds) {
    int minutes = seconds / 60;
    int sec = seconds % 60;
    return std::to_string(minutes) + ":" + (sec < 10 ? "0" : "") + std::to_string(sec);
}

// Проверка на допустимый ввод числа
bool isValidNumber(int num, int row, int col, const std::vector<std::vector<int>>& grid) {
    if (num < 1 || num > 9) return false;

    // Проверка по строкам и столбцам
    for (int i = 0; i < GRID_SIZE; i++) {
        if (grid[row][i] == num || grid[i][col] == num)
            return false;
    }

    // Проверка в 3x3 блоке
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

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE + 50), "Sudoku");

    // Шрифт для отображения текста
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!" << std::endl;
        return -1;
    }

    // Текст для отображения времени
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(10, WINDOW_SIZE);

    // Инициализация судоку
    std::vector<std::vector<int>> grid = initializeSudoku();

    // Прямоугольник для клеток судоку
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    cellShape.setOutlineThickness(2);
    cellShape.setOutlineColor(sf::Color::Black);

    int selectedRow = 0, selectedCol = 0;  // Координаты выбранной клетки
    int inputNumber = 0;  // Текущее введенное число

    // Время начала игры
    sf::Clock gameClock;

    // Основной цикл игры
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Ввод клавиш для перемещения курсора
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

                // Ввод цифр от 1 до 9
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                    inputNumber = event.key.code - sf::Keyboard::Num0;

                    // Проверка, можно ли вставить число
                    if (grid[selectedRow][selectedCol] == 0 && isValidNumber(inputNumber, selectedRow, selectedCol, grid)) {
                        grid[selectedRow][selectedCol] = inputNumber;
                    }
                }
            }
        }

        // Очистка окна
        window.clear(sf::Color::White);

        // Рисуем сетку судоку
        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                cellShape.setPosition(col * CELL_SIZE + 5, row * CELL_SIZE + 5);

                if (row == selectedRow && col == selectedCol) {
                    cellShape.setFillColor(sf::Color(200, 200, 200));  // Выделенная клетка
                }
                else {
                    cellShape.setFillColor(sf::Color::White);  // Обычные клетки
                }

                window.draw(cellShape);

                // Если значение не ноль, рисуем текст
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

        // Обновляем и отображаем таймер
        int elapsedTime = gameClock.getElapsedTime().asSeconds();
        timerText.setString("Time: " + formatTime(elapsedTime));
        window.draw(timerText);

        // Отображаем изменения
        window.display();
    }

    return 0;
}
