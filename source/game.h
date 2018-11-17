#include <stdio.h>
#define CHR_SNAKE '@'
#define CHR_FOOD '*'
#define CHR_FRAME '#'
#define clear() printf("\033[H\033[J")

enum Direction { UP, RIGHT, DOWN, LEFT };
struct Snake {
	int index_i;
	int index_j;
};

class Game {
private:
    int height, width, max_size;
    int snake_size, food_x, food_y, snake_x, snake_y, score, speed;
    Direction direction;
    Snake *snake_body;
public:
    bool status, win;
    Game(int wHeight, int wWidth);
    ~Game();
    bool InBoundsOf(int i, int j);
    int getScore();
    void DrawTable();
    void Process();
    void Move();
    void PutFood();
    void setSpeed(int s);
    void setDirection(Direction dir);
}