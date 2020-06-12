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

    //화면 버퍼 초기화
    scrBuffer = new int*[MAXROW];
    for(int i = 0; i < MAXROW; i++)
        scrBuffer[i] = new int[MAXCOL];
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
            WriteStageToScreen(mStage);
            WriteSnakeToScreen(mSnake);
            renderer.Draw(scrBuffer);

            char str[256] = {0,};
            sprintf(str, "totalDt = %0.2f", totalDt);
            renderer.PrintScoreMessage(str);

            renderer.Refresh();
        }
    }

}

void SnakeGame::Exit(){
    renderer.End();

    //화면버퍼 없애기
    for(int i = 0; i < MAXROW; i++)
        delete[] scrBuffer[i];

    delete[] scrBuffer;
}

void SnakeGame::RestartGame(){
    mGameTimer.ResetTimer();

    renderer.Init();
    mStage.loadStage("data/stage/stage1.txt");
    mSnake.Init();

    NonBlocking();
}

void SnakeGame::WriteStageToScreen(Stage& stage){
    //스테이지에 대한 정보를 받아옴
    auto map = stage.GetMap();
    int row = stage.GetRow();
    int col = stage.GetColumn();

    //벽 출력
    for(int i=0; i<row; i++)
        for(int k=0; k<col; k++)
            scrBuffer[i][k] = map[i][k];
}

void SnakeGame::WriteSnakeToScreen(Snake& snake){
    //헤드 그리기
    scrBuffer[snake.head.Pos.y][snake.head.Pos.x] = (int)TileType::Snake_Head;

    //바디 그리기
    int cnt = 0;
    for(auto it = snake.body.begin(); it != snake.body.end(); it++){
        cnt++;
        if(cnt == snake.body.size()){
            scrBuffer[it->y][it->x] = (int)TileType::Snake_Tail;
            break;
        }

        scrBuffer[it->y][it->x] = (int)TileType::Snake_Body;
    }
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
