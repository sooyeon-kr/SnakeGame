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








