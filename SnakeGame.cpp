#include "SnakeGame.h"
#include "linux_kbhit.h"
#include <unistd.h>


void SnakeGame::Init(){
    renderer.Init();
    mStage.loadStage("data/stage/stage1.txt");
    mSnake.Init();

}

void SnakeGame::Play(){
    /*틱마다 갱신 후 출력*/

    while(1){
        //입력받은 키를 b에 저장
        int key = getch();

        Direction nextDir = mSnake.GetCurDirection();

        //키 입력이 올 경우
        if(key != -1){
            switch(key){
            case KEY_LEFT:            //왼쪽 방향키를 눌렀을때
                nextDir = Direction::LEFT;
                break;
            case KEY_RIGHT:             //오른쪽 방향키
                nextDir = Direction::RIGHT;
                break;
            case KEY_UP:                //위쪽 방향키
                nextDir = Direction::UP;
                break;
            case KEY_DOWN:              //아래쪽 방향키
                nextDir = Direction::DOWN;
                break;
            }
        }

        //뱀의 다음 방향 계산
        DPosition nextPos = mSnake.NextSnakePos(nextDir);

        //다음방향 좌표를 이용해서 충돌 계산하거나 뱀이 살아있는 상태인지 체크
        if(!(mStage.CheckWall(nextPos.Pos.x, nextPos.Pos.y))){
          wmove(renderer.win2,15,15);
          wprintw(renderer.win2,"game over");
            break;
        }

        //충돌 아닌 경우 뱀의 현재위치 업데이트
        mSnake.UpdateSnakePos(nextPos);

        //그리기
        renderer.Draw(mStage, mSnake);
        usleep(50);
    }
}
