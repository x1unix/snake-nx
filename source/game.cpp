#include <iostream>
#include <string>
#include "game.h"

using namespace std;

Game::Game(int wHeight, int wWidth) {
    height = wHeight;
    width = wWidth;
    snake_x = wHeight / 2;
    snake_y = wWidth / 2;
    max_size = (wHeight - 2) * (wWidth - 2);
    snake_body = new Snake[max_size];
    snake_body[0].index_i = snake_x;
    snake_body[0].index_j = snake_y;
    PutFood();
}

Game::~Game() {
    delete[] snake_body;
}

void Game::DrawTable() {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!i || i == height - 1 || !j || j == width - 1) {
                cout << frame;
            }
            else if (i == food_x && j == food_y) {
                cout << food;
            }
            else if (Check(i, j)) {
                cout << snake;
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << "Your current score is: " << score;
}

void Game::setDirection(Direction dir) {
    direction = dir;
}

void Game::Process() {
    switch (direction) {
    case UP:
        snake_x--;
        Move();
        break;
    case LEFT:
        snake_y--;
        Move();
        break;
    case DOWN:
        snake_x++;
        Move();
        break;
    case RIGHT:
        snake_y++;
        Move();
        break;
    }
}

void Game::Move() {
    snake_body[snake_size].index_i = snake_x;
    snake_body[snake_size].index_j = snake_y;
    if (!snake_x || snake_x == height - 1 || !snake_y || snake_y == width - 1) { // collision logic
        status = true;
    }
    else if (snake_x == food_x && snake_y == food_y) {
        snake_size++;
        score++;
        if (snake_size == max_size) {
            win = true;
            return;
        }
        PutFood();
    }
    else {
        for (int index = 0; index < snake_size; index++) {
            snake_body[index].index_i = snake_body[index + 1].index_i;
            snake_body[index].index_j = snake_body[index + 1].index_j;
        }
        snake_body[snake_size].index_i = 0;
        snake_body[snake_size].index_j = 0;
    }
    Sleep(speed);
}

void Game::PutFood() {
    srand(time(NULL));
    food_x = rand() % (height - 2) + 2;
    food_y = rand() % (width - 2) + 2;
}

bool Game::InBoundsOf(int i, int j) {
    for (int k = 0; k < snake_size; k++) {
        if (i == snake_body[k].index_i && j == snake_body[k].index_j) {
            return true;
        }
    }
    return false;
}

int Game::getScore() {
    return score;
}

int Game::setSpeed(int s) {
    speed = s;
}

