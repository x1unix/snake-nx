#include <stdio.h>
#include <switch.h>
#include <string>

#define SNAKE '@'
#define FOOD '*'
#define FRAME '#'

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
    const short height, width;
    PrintConsole* console;
    bool defeated, win;
    Direction eDirection;
    short max_size;
    std::string score_msg;
    short snake_size, food_x, food_y, score, scorePos_x, scorePos_y;
    short snake_x, snake_y;
    u64 speed;
    Snake *snake_body;
    void SetConsoleCursorPosition(short x, short y);
public:
    Game(const short vHeight, const short vWidth, PrintConsole* con);
    ~Game();
    void Draw();
	void RefreshPosition();
    void Process();
	void Move();
	void SetFoodPosition();
	short getScore();
    void setSpeed(u64 s);
    void setDirection(Direction dir);
    Direction getDirection();
    GameState getState();
};