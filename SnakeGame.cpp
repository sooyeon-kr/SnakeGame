#include "SnakeGame.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

void SnakeGame::Init(){
    renderer.Init();
    mStage.loadStage("data/stage/stage1.txt");
    mSnake.Init();

    mGameTimer.Init();
    NonBlocking();
}

void SnakeGame::Blocking(){
    int value = fcntl(0, F_GETFL, 0);
        value &= (~O_NONBLOCK);
        fcntl(0, F_SETFL, value);
}

void SnakeGame::NonBlocking(){
    int value = fcntl(0, F_GETFL, 0);
        value |= O_NONBLOCK;
        fcntl(0, F_SETFL, value);
}

bool SnakeGame::Play(){
    /*틱마다 갱신 후 출력*/
    float totalDt = 0.0f;
    float tickDt = 0;

    while(1){
        mGameTimer.UpdateTime();
        tickDt += mGameTimer.GetDeltaTime();

        if(tickDt >= 0.2f){
            totalDt += tickDt;
            tickDt = 0.0f;
             //입력받은 키를 b에 저장
            int key = getch(); 


            if(errno != EAGAIN){
                renderer.PrintSystemMessage("Error Not EAGAIN");
                return false;
            }



            Direction nextDir = mSnake.GetCurDirection();

            //키 입력이 올 경우
            if(key != -1){
                switch(key){
                case KEY_LEFT:            //왼쪽 방향키를 눌렀을때
                    if(nextDir == Direction::RIGHT)
                        mSnake.Die();
                    nextDir = Direction::LEFT;
                    break;
                case KEY_RIGHT:             //오른쪽 방향키
                    if(nextDir == Direction::LEFT)
                        mSnake.Die();
                    nextDir = Direction::RIGHT;
                    break;
                case KEY_UP:                //위쪽 방향키
                    if(nextDir == Direction::DOWN)
                        mSnake.Die();
                    nextDir = Direction::UP;
                    break;
                case KEY_DOWN:              //아래쪽 방향키
                    if(nextDir == Direction::UP)
                        mSnake.Die();
                    nextDir = Direction::DOWN;
                    break;
                }
            }

            //뱀 머리의 다음 좌표 계산
            DPosition nextHeadPos = mSnake.NextSnakeHeadPos(nextDir);

            //다음방향 좌표를 이용해서 충돌 계산하거나 뱀이 살아있는 상태인지 체크
            if(!(mStage.CheckWall(nextHeadPos.Pos.x, nextHeadPos.Pos.y)) || !mSnake.IsAlive()){
                renderer.PrintSystemMessage("Game Over\nRetry Y/N");

                if(IsGameOver()){

                    return false;
                }else{
                    //1단계 다시 시작을 위한 초기화
                    totalDt = 0.0f;
                    tickDt = 0.0f;
                    RestartGame();

                    continue;
                }
            }

            //충돌 아닌 경우 뱀의 좌표 업데이트
            mSnake.UpdateSnakePos(nextHeadPos);

            //그리기
            renderer.Draw(mStage, mSnake);

            char str[256] = {0,};
            sprintf(str, "totalDt = %0.2f", totalDt);
            renderer.PrintScoreMessage(str);

            renderer.Refresh();
        }
    }

}

void SnakeGame::Exit(){
    renderer.End();
}

void SnakeGame::RestartGame(){
    mGameTimer.ResetTimer();

    renderer.Init();
    mStage.loadStage("data/stage/stage1.txt");
    mSnake.Init();

    NonBlocking();
}

bool SnakeGame::IsGameOver(){
    //렌더러를 이용해서 시스템 문자열 출력

    //리트라이 입력을 받아서 bool값 리턴
    Blocking();
    while(1){
        int key = getch();
        if(key == 'y' || (key + 32) == 'y'){
            return false;
        }else if(key == 'n' | (key + 32) == 'n'){
            return true;
        }
    }
}
