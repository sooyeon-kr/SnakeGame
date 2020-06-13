#include "SnakeGame.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <vector>
#include <string.h>

void SnakeGame::Init(){
    renderer.Init();
    mStage.loadStage("data/stage/stage1.txt");
    mSnake.Init();
    
    //Items 초기화
    Items.clear();

    srand(time(NULL));

    mGameTimer.Init();
    NonBlocking();

    //화면 버퍼 초기화
    scrBuffer = new int*[MAXROW];
    for(int i = 0; i < MAXROW; i++){
        scrBuffer[i] = new int[MAXCOL];
        memset(scrBuffer[i], 0x00, sizeof(int)*MAXCOL);
    }
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
    float updateDT = 0;
    
    float rTime = 5.0f;
    float itemDT = 0.0f;

    while(1){
        renderer.DrawUI();
        mGameTimer.UpdateTime();
        updateDT += mGameTimer.GetDeltaTime();
   
        if(updateDT >= 0.2f){
            totalDt += updateDT;
            itemDT += updateDT;
            updateDT = 0.0f;
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

            //다음방향 좌표를 이용해서 충돌 계산하거나 타일정보에 따라 처리
            TileType t = CheckBuffer(nextHeadPos.Pos.x, nextHeadPos.Pos.y);
            if(t == TileType::Item_Growth){
                mSnake.body.push_front(mSnake.head.Pos);
                mSnake.head.Pos = nextHeadPos.Pos;
                //아이템 없애기
                DestructItem(mSnake.head.Pos);
            }else if(t == TileType::Item_Poison){
                mSnake.body.pop_back();
                mSnake.UpdateSnakePos(nextHeadPos);
                //아이템 없애기
                DestructItem(mSnake.head.Pos);
                if(mSnake.GetSnakeLength() < 3)
                    mSnake.Die();
            }else if(t == TileType::Blank){
                //충돌 아닌 경우 뱀의 좌표 업데이트
                mSnake.UpdateSnakePos(nextHeadPos);
            }else if(t == TileType::Wall || t == TileType::Snake_Body || t == TileType::Snake_Tail){
                mSnake.Die();
            }

            if(!mSnake.IsAlive()){
                if(IsGameOver()){
                    return false;
                }else{
                    //1단계 다시 시작을 위한 초기화
                    totalDt = 0.0f;
                    updateDT = 0.0f;
                    RestartGame();

                    continue;
                }
            }
        }
        //화면 초기화
        ScreenClear();

        //그리기
        WriteStageToScreen(mStage);
        WriteSnakeToScreen(mSnake);

        //아이템 제거
        for(auto& e : Items){
            float flowT = e.timer.GetFlowTime();
            if(e.lifeTime <= flowT){
                DestructItem();
                break;
            }
        }

        //아이템 생성
        if(itemDT >= rTime){
            CreateItem();
            rTime = rand()%5 + 3;
            itemDT = 0;
        }
        WriteItemToScreen();

        char str[256] = {0,};
        sprintf(str, "totalDt = %0.2f", totalDt);
        renderer.PrintScoreMessage(str);

        renderer.Draw(scrBuffer);
  
        renderer.Refresh();
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

    Items.clear();

    NonBlocking();
}

void SnakeGame::ScreenClear(){
    for(int i = 0; i < MAXROW; i++)
        for(int j = 0; j < MAXCOL; j++)
            scrBuffer[i][j] = 0;
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
void SnakeGame::WriteItemToScreen(){
    for(auto e : Items){
        scrBuffer[e.Pos.y][e.Pos.x] = (int)e.ItemType;
    }
}

bool SnakeGame::IsGameOver(){
    //렌더러를 이용해서 시스템 문자열 출력
    renderer.PrintSystemMessage("Game Over\nRetry Y/N");

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

void SnakeGame::CreateItem(){
    if(Items.size() < 3){
        Item newItem;

        int itemType = (rand() % 100 +1)%2; //0또는 1로 나오는 나머지 값으로 아이템의 종류를 정함.
        if(itemType)
            newItem.ItemType = TileType::Item_Growth;
        else
            newItem.ItemType = TileType::Item_Poison;

        newItem.Pos = GetRandomPos();

        newItem.timer.Init();
        Items.push_back(newItem);
    }else{
        return;
    }
}

void SnakeGame::DestructItem(){
    Items.pop_front();
    /*
    for(auto it=Items.begin(); it!=Items.end() ; it++)
    {
        if(it->isAlive==false)
        {
            Items.erase(it++);
            break;
        }
    }
    */
}
void SnakeGame::DestructItem(Position pos){
    for(auto it=Items.begin(); it!=Items.end();)
    {
        if(it->Pos.x == pos.x && it->Pos.y == pos.y)
            Items.erase(it++);
        else
            it++;
    }
}

Position SnakeGame::GetRandomPos(){
    std::vector<Position> candidate;
    
    for(int i=0; i<MAXROW; i++)
        for(int j=0; j<MAXCOL; j++)
            if(scrBuffer[i][j]==(int)TileType::Blank)
                candidate.emplace_back(j, i);

    int randIdx = rand() % candidate.size();

    return candidate[randIdx];
}

TileType SnakeGame::CheckBuffer(int x, int y){
    return (TileType)scrBuffer[y][x];
}