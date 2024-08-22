#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

const int N = 4;
int maze[N][N] = {
    {1, 0, 0, 0},
    {1, 1, 0, 1},
    {0, 1, 0, 0},
    {1, 1, 1, 1}
};
int solution[N][N];

bool isSafe(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1);
}

bool solveMazeUtil(int x, int y, vector<pair<int, int>>& path) {
    if (x == N - 1 && y == N - 1) {
        solution[x][y] = 1;
        path.push_back({x, y});
        return true;
    }

    if (isSafe(x, y)) {
        solution[x][y] = 1;
        path.push_back({x, y});

        if (solveMazeUtil(x + 1, y, path)) return true;
        if (solveMazeUtil(x, y + 1, path)) return true;

        solution[x][y] = 0; // Backtracking
        path.pop_back();
        return false;
    }

    return false;
}

int main() {
    vector<pair<int, int>> path;

    // Solve the maze and find the path
    solveMazeUtil(0, 0, path);

    // SFML window setup
    sf::RenderWindow window(sf::VideoMode(400, 400), "Rat in a Maze");
    int cellSize = 100;

    // Game loop
    int step = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw the maze
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(j * cellSize, i * cellSize);
                cell.setOutlineThickness(2);
                cell.setOutlineColor(sf::Color::Black);

                if (maze[i][j] == 1)
                    cell.setFillColor(sf::Color::White); // Open path
                else
                    cell.setFillColor(sf::Color::Black); // Blocked path

                window.draw(cell);
            }
        }

        // Draw the rat
        if (step < path.size() && clock.getElapsedTime().asSeconds() > 0.5) {
            clock.restart();
            sf::CircleShape rat(cellSize / 4);
            rat.setFillColor(sf::Color::Red);
            rat.setPosition(path[step].second * cellSize + cellSize / 4,
                            path[step].first * cellSize + cellSize / 4);
            window.draw(rat);
            step++;
        } else if (step >= path.size()) {
            sf::CircleShape rat(cellSize / 4);
            rat.setFillColor(sf::Color::Red);
            rat.setPosition(path.back().second * cellSize + cellSize / 4,
                            path.back().first * cellSize + cellSize / 4);
            window.draw(rat);
        }

        window.display();
    }

    return 0;
}