#include <iostream>
#include <string>
#include "game.h"
#include <time.h>
#include <switch.h>

#define WITH_TIMER true

using namespace std;

Game::Game(const short vHeight, const short vWidth, PrintConsole* con) : height(vHeight), width(vWidth), console(con) {
    defeated = false;
    win = false;
    score_msg = "Your current score is: ";
    snake_size = 1;
    score = 0;
    scorePos_x = height;
	scorePos_y = score_msg.length();
    snake_x = height / 2;
    snake_y = width / 2;
    max_size = (vHeight - 2) * (vWidth - 2);
    snake_body = new Snake[max_size];
    snake_body[0].index_i = snake_x;
    snake_body[0].index_j = snake_y;
    
    srand(time(NULL));
	food_x = rand() % (height - 3) + 3;
	food_y = rand() % (width - 3) + 3;
	while (food_x == snake_x && food_y == snake_y) {
		food_x = rand() % (height - 3) + 3;
		food_y = rand() % (width - 3) + 3;
	}
}

Game::~Game() {
    delete[] snake_body;
}

// Polyfill over Windows Console API
void Game::SetConsoleCursorPosition(short x, short y) {
    // TODO: implement
    console->cursorX = x;
    console->cursorY = y;
}

void Game::Draw() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!i || i == height - 1 || !j || j == width - 1) {
				cout << FRAME;
			}
			else if (i == food_x && j == food_y) {
				cout << FOOD;
			}
			else if (i == snake_x && j == snake_y) {
				cout << SNAKE;
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
	cout << score_msg;
	SetConsoleCursorPosition(scorePos_y, scorePos_x);
	cout << score << endl;
    consoleUpdate(NULL);
}

void Game::RefreshPosition() {
	for (int i = 0; i < snake_size; i++) {
		SetConsoleCursorPosition(snake_body[i].index_j, snake_body[i].index_i);
		cout << SNAKE;
	}
	SetConsoleCursorPosition(scorePos_y, scorePos_x);
	cout << score << endl;
    consoleUpdate(NULL);
}

void Game::Process() {
	switch (eDirection) {
	case UP:
		snake_x--;
		break;
	case LEFT:
		snake_y--;
		break;
	case DOWN:
		snake_x++;
		break;
	case RIGHT:
		snake_y++;
		break;
    default:
        break;
	}
	Move();
}

void Game::Move() {
	/*for (int i = 0; i < snake_size; i++) {   tail collision logic (if you try to reverse your move, you die). Optional.
		if (snake_body[i].index_i == snake_x && snake_body[i].index_j == snake_y) {
			defeated = true;
			return;
		}
	}*/
	snake_body[snake_size].index_i = snake_x;
	snake_body[snake_size].index_j = snake_y;
	if (!snake_x || snake_x == height - 1 || !snake_y || snake_y == width - 1) { // wall collision logic
		defeated = true;
	}
	else if (snake_x == food_x && snake_y == food_y) { // grow
		snake_size++;
		score++;
		if (snake_size == max_size) {
			win = true;
		}
		else {
			SetFoodPosition();
		}
	}
	else { // simple move
		SetConsoleCursorPosition(snake_y, snake_x);
		cout << SNAKE;
		SetConsoleCursorPosition(snake_body[0].index_j, snake_body[0].index_i);
		cout << " ";
		for (int i = 0; i < snake_size; i++) {
			snake_body[i].index_i = snake_body[i + 1].index_i;
			snake_body[i].index_j = snake_body[i + 1].index_j;
		}
	}

    consoleUpdate(NULL);
#ifdef WITH_TIMER
    // Input control blocked by sleep.
    // Consider using async calls
    svcSleepThread(speed);
#endif
}

void Game::SetFoodPosition() {
	srand(time(NULL));
	food_x = rand() % (height - 3) + 3;
	food_y = rand() % (width - 3) + 3;
	int i = 0;
	while (i < snake_size) {
		if ((food_x == snake_body[i].index_i && food_y == snake_body[i].index_j) || (!food_x || food_x == height - 1 || !food_y || food_y == width - 1)) {
			food_x = rand() % (height - 3) + 3;
			food_y = rand() % (width - 3) + 3;
			i = 0;
		}
		else {
			i++;
		}
	}
	SetConsoleCursorPosition(food_y, food_x);
	cout << FOOD;
}

short Game::getScore() {
	return score;
}

void Game::setSpeed(u64 s) {
	speed = s;
}

Direction Game::getDirection() {
	return eDirection;
}

void Game::setDirection(Direction dir) {
    eDirection = dir;
}

GameState Game::getState() {
    if (defeated) {
        return LOST;
    }

    if (win) {
        return WON;
    }

    return PLAYING;
}