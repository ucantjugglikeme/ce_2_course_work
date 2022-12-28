#include "wrapped_lab_gen.h"

std::pair<int, int> getWallPoint(std::pair<int, int> curCell, std::pair<int, int> neighbourCell) {
    int x;
    int dx = (curCell.first - neighbourCell.first);
    if (dx > 0) {
        x = curCell.first - 1;
    }
    else if (dx < 0) {
        x = curCell.first + 1;
    }
    else {
        x = curCell.first;
    }

    int y;
    int dy = (curCell.second - neighbourCell.second);
    if (dy > 0) {
        y = curCell.second - 1;
    }
    else if (dy < 0) {
        y = curCell.second + 1;
    }
    else {
        y = curCell.second;
    }

    return {x, y};
}

int getRandomIndex(int startInd, int endInd) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(startInd, endInd);
    return distrib(gen);
}

std::string generateLabyrinth(int width = 29, int height = 13) {
    if (2 > width || width > 79 || 2 > height || height > 59) {
        return "111\n"
               "101\n"
               "111\n";
    }

    const int WIDTH = width, HEIGHT = height;

    int map[HEIGHT][WIDTH];

    // initial begin

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if ((i % 2 != 0 && j % 2 != 0) && (i < HEIGHT - 1 && j < WIDTH - 1)) {
                map[i][j] = LabyrinthItem::CELL;
            }
            else {
                map[i][j] = LabyrinthItem::WALL;
            }
        }
    }

    // end

    int unvisitedCellsCount = (HEIGHT - (HEIGHT / 2 + 1)) * (WIDTH - (WIDTH / 2 + 1));
    const int DISTANCE = 2;
    std::pair<int, int> startCell = std::make_pair(1, 1);
    std::pair<int, int> curCell = startCell;
    std::stack<std::pair<int, int>> neighboursPoints = {};

    //unvisitedCellsCount--;

    // always @(posedge clk) begin

    do {
        std::pair<int, int> dirUp = {curCell.first, curCell.second - DISTANCE};
        std::pair<int, int> dirRight = {curCell.first + DISTANCE, curCell.second};
        std::pair<int, int> dirDown = {curCell.first, curCell.second + DISTANCE};
        std::pair<int, int> dirLeft = {curCell.first - DISTANCE, curCell.second};

        int neighbourCell;
        std::vector<std::pair<int, int>> neighbours;
        std::vector<std::pair<int, int>> directions;
        directions = {dirUp, dirRight, dirDown, dirLeft};
        for (auto &dir: directions) {
            if (dir.first > 0 && dir.first < WIDTH && dir.second > 0 && dir.second < HEIGHT) {
                neighbourCell = map[dir.second][dir.first];
                if (
                        neighbourCell != LabyrinthItem::WALL &&
                        neighbourCell != LabyrinthItem::CELL_VISITED
                        ) {
                    neighbours.emplace_back(dir);
                }
            }
        }

        if (!neighbours.empty()) {
            int randNum = getRandomIndex(0, int(neighbours.size() - 1));
            std::pair<int, int> neighbourPoint = neighbours[randNum];
            neighboursPoints.push(neighbourPoint);

            auto wallPoint = getWallPoint(curCell, neighbourPoint);
            map[wallPoint.second][wallPoint.first] = LabyrinthItem::CELL_VISITED;
            curCell = neighbourPoint;

            map[neighbourPoint.second][neighbourPoint.first] = LabyrinthItem::CELL_VISITED;
            unvisitedCellsCount--;
        }
        else if (!neighboursPoints.empty()) {
            curCell = neighboursPoints.top();
            neighboursPoints.pop();
        }
        else {
            int randUnvisitedX;
            int randUnvisitedY;
            for (int i = 1; i < HEIGHT - 1; ++i) {
                for (int j = 1; j < WIDTH - 1; ++j) {
                    if (map[i][j] == LabyrinthItem::CELL) {
                        randUnvisitedX = j;
                        randUnvisitedY = i;
                        map[i][j] = LabyrinthItem::CELL_VISITED;
                    }
                }
            }

            curCell = {randUnvisitedX, randUnvisitedY};
            unvisitedCellsCount--;
        }
    } while (unvisitedCellsCount > 0);

    // end

    // c out

    for (auto &row: map) {
        for (auto &cell: row) {
            if (cell == LabyrinthItem::CELL_VISITED) {
                cell = LabyrinthItem::CELL;
            }
        }
    }

    std::string str_maze;
    for (const auto &row: map) {
        for (const auto &item: row) {
            str_maze.append(std::to_string(item));
        }
        str_maze.append("\n");
    }

    return str_maze;
}

char * wrappedGenerateLabyrinth(int width, int height) {
    std::string maze = generateLabyrinth(width, height);

    char * wrappedMaze = new char[maze.length() + 1];
    strcpy(wrappedMaze, maze.c_str());

    return wrappedMaze;
}
