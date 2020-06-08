#include <fstream>
#include <iostream>
#include <unistd.h>

#include "SnakeGame.h"

int main()
{
    setlocale(LC_ALL, ""); //로케일 설정, 유니코드 출력을 위해. 안 하면 cmd에 #표시가 됨.

	SnakeGame game;
	game.Init();

    while(1)
    {
        //TODO: 게임 오버 시, 재시작 여부 물어보고, 입력 값에 따라  continue break
        if(!game.Play()){
			break;
		}
    }

	game.Exit();

    return 0;
}

/*

#include "SnakeGame.h"

// Unreal standards
using int32 = int;

int32 maxheight, maxwidth;
void PlayGame();
int32 IsUserReady();
int32 AskUserToPlayAgain();
void ClearCentre();
int32 UserInput();

int32 main ()
{
	if (IsUserReady() == 'y') // wait for confirmation of the user
	do {
		{
			SnakeGame NewSnake;
			NewSnake.PlayGame();
		}
	}
	while (AskUserToPlayAgain() == 'y');
	return 0;
}

// clear the screen and centre the cursor
void ClearCentre(float x, float y)
{
	clear(); // clear the screen if the game is played for the 2nd time
	initscr();
	noecho();
	getmaxyx(stdscr, maxheight, maxwidth);
	move((maxheight/y), (maxwidth/x));
}

// receive user confirmation
int32 UserInput()
{
	int32 UserInput = getch();
	refresh();
	endwin();
	clear();

	return UserInput;
}

// print start menu
int32 IsUserReady()
{
	ClearCentre(3, 2.5);
	printw("Welcome to the Snake Game. Are you ready? (y/n)");
	return UserInput();
}

// print end of the game menu and ask user to play again
int32 AskUserToPlayAgain()
{
	ClearCentre(2.5, 2.5);
	printw("Do you want to play again? (y/n)");
	return UserInput();
}

*/







