#include <stdio.h>
#define CHR_SNAKE '@'
#define CHR_FOOD '*'
#define CHR_FRAME '#'
#define CHR_EMPTY ' '
#define clear() printf("\033[H\033[J")

/////////////////////////////////////////////
// Port of github.com/lowlevelftw/somecode //
/////////////////////////////////////////////

enum GameState { PLAYING, WON, LOST };
enum Direction { UP, RIGHT, DOWN, LEFT, NOP };
struct Snake {
	int index_i;
	int index_j;
};

class Game {
private:
    int height, width, max_size;
    bool status, win;
    int snake_size, food_x, food_y, snake_x, snake_y, score, speed;
    Direction direction;
    Snake *snake_body;
    bool inBoundsOf(int i, int j);
public:
    Game(int vHeight, int vWidth);
    ~Game();
    GameState getState();
    int getScore();
    void DrawTable();
    void Process();
    void Move();
    void PutFood();
    void setSpeed(int s);
    void setDirection(Direction dir);
};