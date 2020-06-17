#include "SnakeGame.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <vector>
#include <string.h>

void SnakeGame::Init(){

    renderer.Init();

    if(CLEAR==0){
    mStage.loadStage("data/stage/stage1.txt");
  }
  else if(CLEAR==1){
    mStage.loadStage("data/stage/stage2.txt");
  }
  else if(CLEAR==2){
    mStage.loadStage("data/stage/stage3.txt");
  }
  else if(CLEAR==3){
    mStage.loadStage("data/stage/stage4.txt");
  }
    mSnake.Init();

    //Items 초기화
    Items.clear();
    // Gates.clear();

    srand(time(NULL));

    mGameTimer.Init();
    NonBlocking();

    //화면 버퍼 초기화
    scrBuffer = new int*[MAXROW];
    for(int i = 0; i < MAXROW; i++){
        scrBuffer[i] = new int[MAXCOL];
        memset(scrBuffer[i], 0x00, sizeof(int)*MAXCOL);
    }

    //스코어 및 미션 내용 초기화
    score.SnakeLength = mSnake.GetSnakeLength();
    score.GrowthItemNum = 0;
    score.PoisonItemNum = 0;
    score.UsedGateNum = 0;
    score.time = 0.0f;   

    mission.SnakeLength = 5;
    mission.GrowthItemNum = 5;
    mission.PoisonItemNum = 3;
    mission.UsedGateNum = 5;
    mission.time = 60.0f;
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
    float gateT = 0.0f;

    float rTime = 5.0f;
    float itemDT = 0.0f;

    while(1)
    {
        mGameTimer.UpdateTime();
        updateDT += mGameTimer.GetDeltaTime();

        renderer.DrawUI();


        //입력받은 키를 b에 저장

        if(updateDT >= 0.15f){
            totalDt += updateDT;
            itemDT += updateDT;
            gateT += updateDT;
            score.time +=updateDT;
            updateDT = 0.0f;
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
              //몸이 최대길이보다 작을경우에만 몸길이 늘리기
              if(mSnake.GetSnakeLength() < mSnake.GetSnakeMaxLength()){
                mSnake.body.push_front(mSnake.head.Pos);
                mSnake.head = nextHeadPos;
                score.SnakeLength = mSnake.GetSnakeLength(); // 스네이크의 몸의 길이를 스코어에 반영해주어야 하므로
                score.GrowthItemNum++;
              }

              DestructItem(mSnake.head.Pos);

            }else if(t == TileType::Item_Poison){
                mSnake.body.pop_back();
                mSnake.UpdateSnakePos(nextHeadPos);
                score.PoisonItemNum++;
                score.SnakeLength = mSnake.GetSnakeLength();
                
                DestructItem(mSnake.head.Pos);
                
                // 스네이크의 몸의 길이가 3보다 작아지거나 PoisonItem을 Mission에서 제한한 갯수보다 많이 먹었을 경우 죽음
                if(mSnake.GetSnakeLength() < 3 || score.PoisonItemNum > mission.PoisonItemNum) 
                    mSnake.Die();
            }
            else if(t == TileType::Blank){
                //충돌 아닌 경우 뱀의 좌표 업데이트
                mSnake.UpdateSnakePos(nextHeadPos);
            }
            else if(t == TileType::Wall || t == TileType::Snake_Body || t == TileType::Snake_Tail){
                mSnake.Die();
            }
            //게이트
            else if(t== TileType::Gate || t == TileType::Gate2){ 
              score.UsedGateNum++; //스코어의 게이트 사용 횟수 증가
              nextHeadPos = IndicatePassedDPos(nextHeadPos);  //게이트 통과 후 다음 위치 계산   
              mSnake.UpdateSnakePos(nextHeadPos); //스네이크의 위치 업데이트
              gate->lifeTurn = mSnake.GetSnakeLength(); //게이트 소멸을 위해서 lifeTurn에 스네이크의 길이를 넣어줌
            }

            //게이트가 있을경우 게이트의 생존턴 계산
            if(gate != nullptr){ // 게이트가 만들어졌을경우
              if(gate->lifeTurn > 0){ // 현재 게이트 통과 중인 것(한 틱 마다 한 번씩 이동하므로)
                gate->lifeTurn--; 
              }else if(gate->lifeTurn == 0){ // 스네이크의 길이만큼 모두 통과를 했을 경우
                //삭제
                delete gate;
                gate = nullptr; // 다시 create해주기 위하여 nullptr로 초기화
              }
            }
            
            if(!mSnake.IsAlive()){ // 스네이크가 죽었을 경우
                if(IsGameOver()){ //게임이 끝났을 경우인데 사용자가 n입력
                    return false;
                }
                else //y입력
                {
                    //1단계 다시 시작을 위한 초기화
                    totalDt = 0.0f;
                    updateDT = 0.0f;
                    gateT=0.0f;

                    RestartGame(); //재시작

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


        if(gateT>7.0f){ //게임 첫 시작 시, 7초 후에 생성
          if(gate == nullptr){ //gate가 nullptr일 경우
            CreateGate();
            gateT = 0.0f;
          }
        }

        WriteGateToScreen(); //gate값을 scrBuffer에 저장

      //스코어 및 미션 표시
      {
        char str[256] = {0,};
        sprintf(str, "Score");
        renderer.PrintScoreMessage(str);

        sprintf(str, "Total GameTime: %0.0f sec", totalDt);
        renderer.PrintScoreMessageXY(1, 2, str);

        sprintf(str, "Length: %d / %d", score.SnakeLength, mSnake.GetSnakeMaxLength());
        renderer.PrintScoreMessageXY(1, 4, str);

        sprintf(str, "G-Item: %d",score.GrowthItemNum);
        renderer.PrintScoreMessageXY(1, 6, str);

        sprintf(str, "P-Item: %d",score.PoisonItemNum);
        renderer.PrintScoreMessageXY(1, 8, str);

        sprintf(str, "Gate Usage: %d",score.UsedGateNum);
        renderer.PrintScoreMessageXY(1, 10, str);
      }

      { //이렇게 할 경우 str이 생성되었다가 {}이후에
        char str[256]= {0,};
        sprintf(str, "Mission");
        renderer.PrintMissionMessage(str);

        sprintf(str, "Mission Length: %d / %d ( )",score.SnakeLength, mission.SnakeLength);
        renderer.PrintMissionMessageXY(1, 3, str);

        sprintf(str, "Mission G-Item: %d / %d ( )",score.GrowthItemNum, mission.GrowthItemNum);
        renderer.PrintMissionMessageXY(1, 5, str);

        sprintf(str, "Mission P-Item: %d <= %d ( )",score.PoisonItemNum, mission.PoisonItemNum);
        renderer.PrintMissionMessageXY(1, 7, str);

        sprintf(str, "Mission Gate Usage: %d / %d ( )",score.UsedGateNum, mission.UsedGateNum);
        renderer.PrintMissionMessageXY(1, 9, str);

        sprintf(str, "Mission Time: %0.0f > %0.0f sec ( )", score.time, mission.time);
        renderer.PrintMissionMessageXY(1, 11, str);
      }

      //게임클리어 조건 검사
      if(IsClear()){
        CLEAR++;
        if(GameClear()){
          totalDt = 0.0f;
          updateDT = 0.0f;
          gateT=0.0f;

          RestartGame();
          continue;
        }
        else{
          return false;
        }

      }

        renderer.Draw(scrBuffer);

        renderer.Refresh();
    }

}

//끝냈을 때 사용하려고 만든 메서드
void SnakeGame::Exit(){
    renderer.End();

    //화면버퍼 없애기
    for(int i = 0; i < MAXROW; i++)
        delete[] scrBuffer[i];

    delete[] scrBuffer;
}


void SnakeGame::RestartGame(){
    mGameTimer.ResetTimer();

    //renderer.Init();

    if(CLEAR==0){
    mStage.loadStage("data/stage/stage1.txt");
    }
    else if(CLEAR==1){
      mStage.loadStage("data/stage/stage2.txt");
    }
    else if(CLEAR==2){
      mStage.loadStage("data/stage/stage3.txt");
    }
    else if(CLEAR==3){
      mStage.loadStage("data/stage/stage4.txt");
    }

    mSnake.Init();

    Items.clear();

    NonBlocking();
}

//scrBuffer의 값을 비워주기 위해 모두 0으로 채워준다.
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


//모두 scrBuffer에 각각의 TileType 저장
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
void SnakeGame::WriteGateToScreen(){
  if(gate != nullptr){
    scrBuffer[gate->gPos1.y][gate->gPos1.x] = (int)TileType::Gate;
    scrBuffer[gate->gPos2.y][gate->gPos2.x] = (int)TileType::Gate2;
  }
}

//미션을 성공했을 경우, 호출하여 다음 스테이지로 실행 여부 출력 및 키입력받는 메소드
bool SnakeGame::GameClear(){
    renderer.PrintSystemMessage("Clear!\nNext Stage? Y/N");
    Blocking();
    while(1){
        int key = getch();
        if(key == 'y' || (key + 32) == 'y'){
            return true;
        }else if(key == 'n' | (key + 32) == 'n'){
            return false;
        }
    }
}

//미션 성공 여부를 score와 mission의 멤버변수로 비교하여 리턴해주는 메소드
bool SnakeGame::IsClear(){
  return score.SnakeLength >= mission.SnakeLength && score.GrowthItemNum >= mission.GrowthItemNum && 
        score.PoisonItemNum <= mission.PoisonItemNum && score.UsedGateNum >= mission.UsedGateNum && score.time > mission.time;
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

//아이템을 생성하는 메소드
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

//게이트 생성하는 메소드
void SnakeGame::CreateGate(){
  if(gate == nullptr){ //게이트가 없으면
    std::vector<Position> candidate; //Position형을 갖는 vector를 선언

    for(int i=0; i<MAXROW; i++)//scrBuffer에서 TileType이 Wall인 좌표 값을 벡터에 모두 저장
        for(int j=0; j<MAXCOL; j++)
            if(scrBuffer[i][j]==(int)TileType::Wall)
                candidate.emplace_back(j, i);
    int randIdx1, randIdx2;

    while(true){ //랜덤으로 돌린 인덱스가 같지 않을 때까지 while문을 돌아 두개의 인덱스를 구함
      randIdx1 = rand() % candidate.size();
      randIdx2 = rand() % candidate.size();
      if(randIdx1 != randIdx2){
        break;
      }
    }

    gate = new Gate(); //한 쌍의 게이트 정보를 갖을 수 있는 gate 생성하여 각각의 gate에 wall의 위치값을 주어 생성될 gate의 위치를 정한다.
    gate->gPos1 = candidate[randIdx1]; 
    gate->gPos2 = candidate[randIdx2];
  }
}

//게이트 통과 시 출력 될 head의 좌표를 구하는 메소드(바디는 연쇄적으로 처리 가능)
DPosition SnakeGame::IndicatePassedDPos(DPosition headPos){
  Position outPos; // 나갈 게이트의 위치값을 받을 Position선언. 
  if(gate->gPos1 == headPos.Pos){ 
    outPos = gate->gPos2;
  }else{
    outPos = gate->gPos1;
  }

  Direction curDir = headPos.Dir; //현재 head의 진행 방향을 curDir에 저장함
  Position nextPos = outPos; //나갈 게이트의 위치는 nextPos에 저장함
  
  for(int i = 0; i < (int)Direction::SIZE; i++){ //나갈방향의 수(4번 0~3)만큼 비교함
    //현재 방향을 기준으로 다음 좌표 계산
    switch(curDir){
    case Direction::UP:
      nextPos.y--;
      break;
    case Direction::DOWN:
      nextPos.y++;
      break;
    case Direction::RIGHT:
      nextPos.x++;
      break;
    case Direction::LEFT:
      nextPos.x--;
      break;
    }

    //그 좌표가 BLANK가 맞는지 체크
    if(CanMovePos(nextPos.x, nextPos.y)){
      break; //맞다면 다른 검사할 필요 없이 break로 빠져나가 이후 nextPos로 Head값을 옮기면 된다.
    }
    
    //(2번째 검사 시 필요, 원래 방향으로 바꿔서 다시 검사해야 하므로)
    curDir = headPos.Dir;

    //안맞으면 방향 순서에 따라 변경 
    {
      switch(i){
        case 0: //첫 시도 실패 따라서 시계방향 계산
          curDir = (Direction)(((int)curDir + 1) % (int)Direction::SIZE);
          break;

        case 1: //두번째 시도 실패. 역시계방향 계산
          curDir = (Direction)((curDir - 1) < 0 ? Direction::SIZE - 1 : curDir - 1);
          break;
          
        case 2: //세번째 실패. 역방향 계산
          if((int)curDir >= 2){
            curDir = Direction((int)curDir - 2);
          }else{
            curDir = Direction((int)curDir + 2);
          }
          break;
      }
      nextPos = outPos; //원래 좌표로 바꿔서 다시 검사해야 하므로
    }
  }

    DPosition dp;
    dp.Pos = nextPos;
    dp.Dir = curDir;

    return dp;
}

//벽인지 아닌지 체크하여 벽일 경우 false를 바로 리턴해주고 blank일 경우 true를 리턴해주는 메서드
bool SnakeGame::CanMovePos(int x, int y){
  if(x < 0 || x >= MAXCOL || y < 0 || y >= MAXROW)
    return false;

  return scrBuffer[y][x] == (int)TileType::Blank ? true : false;
}

//시간의 흐름에 의한 Item삭제 메소드
void SnakeGame::DestructItem(){
    Items.pop_front();
}

//스네이크에 의한 Item삭제 메소드
void SnakeGame::DestructItem(Position pos){
    for(auto it=Items.begin(); it!=Items.end();)
    {
      if(it->Pos.x == pos.x && it->Pos.y == pos.y)
        Items.erase(it++);
      else
        it++;
    }
}

//Item의 생성 위치 결정 메소드
Position SnakeGame::GetRandomPos(){
    std::vector<Position> candidate;

    for(int i=0; i<MAXROW; i++)
        for(int j=0; j<MAXCOL; j++)
            if(scrBuffer[i][j]==(int)TileType::Blank)
                candidate.emplace_back(j, i);

    int randIdx = rand() % candidate.size();

    return candidate[randIdx];
}

//x, y값을 받아 그 좌표에 해당하는 TileType이 무엇인지 return하는 메소드
TileType SnakeGame::CheckBuffer(int x, int y){
    return (TileType)scrBuffer[y][x];
}
