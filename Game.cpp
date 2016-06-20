// Kurs.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>

using namespace std;

const int EMPTY = 0;
const int CAR = 1;

const int MAP_HEIGHT = 10;
const int MAP_WIDTH = 20;

int personX;
int personY;

const int REFRESH_RATE = 100;

int map[MAP_HEIGHT][MAP_WIDTH];

int delays[MAP_HEIGHT];

int timers[MAP_HEIGHT];

bool finish = false;

bool crash = false;

void createWorld() 
{
    crash = false;
	finish = false;
	personX = MAP_WIDTH / 2;
	personY = 0;

	int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = EMPTY;
        }
    }

    for (y = 0; y < MAP_HEIGHT; y++) {
		if ( y == 2 || y == 6 ) {
			delays[y] = 0;
		} else {
			delays[y] = rand() % 1000 + 200;
		}
		timers[y] = 0;
    }
}

void checks() 
{
	if ( map[personY][personX] == CAR ) {
		crash = true;
	}

	if ( personY == MAP_HEIGHT - 1 ) {
		finish = true;
	}
}

void movePerson()
{
	int moveX = 0, moveY = 0;

	if ( GetAsyncKeyState(VK_LEFT) ) {
		moveX = -1;
	} else if ( GetAsyncKeyState(VK_RIGHT) ) {
		moveX = 1;
	} else if ( GetAsyncKeyState(VK_DOWN) ) {
		moveY = -1;
	} else if ( GetAsyncKeyState(VK_UP) ) {
		moveY = 1;
	}

	int nextX = personX + moveX;
	int nextY = personY + moveY;

	if ( nextX < 0 || nextX > MAP_WIDTH - 1 ) {
		return;
	} else if ( nextY < 0 || nextY > MAP_HEIGHT - 1 ) {
		return;
	}

	personX = nextX;
	personY = nextY;

	checks();
}

void draw() 
{
	int x, y, p;

    for (y = MAP_HEIGHT - 1; y > -1; y--) {
        for (x = 0; x < MAP_WIDTH; x++) {
            p = map[y][x];
            if ( personY == y && personX == x ) {
				cout << "O";
            } else if ( p == CAR ) {
				cout << "=";
            } else {
				cout << " ";
            }
        }
		cout << endl;
    }
}

void clearscreen()
{
	system("cls");
}

void createCars() 
{
    int x, y;

    for (y = 1; y < MAP_HEIGHT - 1; y++) {
        timers[y] -= REFRESH_RATE;

        if ( timers[y] > 0 ) {
            continue;
        }

		for (x = MAP_WIDTH - 1; x > 0; x--) {
			map[y][x] = map[y][x - 1];
		}

		if ( rand() % 100 > 75 ) {
			map[y][0] = CAR;
		} else {
			map[y][0] = EMPTY;
		}

        timers[y] = delays[y];
    }
}

int _tmain(int argc, _TCHAR* argv[]) 
{
	createWorld();

	while (true) {
		clearscreen();
		
		checks();
		
		if ( ! crash && ! finish ) {			
			movePerson();
			createCars();
			draw();
		} else {
			if ( crash ) {
				cout << "CRASH!";
			}
			if ( finish ) {
				cout << "FINISH!";
			}

			cout << "\n\n";
			cout << "PRESS ESC TO EXIT\nOR SPACE TO START AGAIN";

			if ( GetAsyncKeyState(VK_SPACE) ) {
				createWorld();
			}
		}

		if ( GetAsyncKeyState(VK_ESCAPE) ) {
			return 0;
		}

		Sleep(REFRESH_RATE);
	}

	return 0;
}