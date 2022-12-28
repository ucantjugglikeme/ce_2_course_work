#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <stack>
#include <cstring>

enum LabyrinthItem {
    CELL = 0,
    WALL = 1,
    CELL_VISITED = 2
};

std::pair<int, int> getWallPoint(std::pair<int, int>, std::pair<int, int>);
int getRandomIndex(int, int);
std::string generateLabyrinth(int, int);

#ifdef __cplusplus
extern "C" {
#endif

char * wrappedGenerateLabyrinth(int, int);

#ifdef __cplusplus
}
#endif