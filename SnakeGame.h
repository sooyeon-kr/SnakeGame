#include "Renderer.h"
#include "Timer.h"
#include <list>

struct Item{
    TileType ItemType;
    Position Pos;
    Timer timer;
    float lifeTime = 10;
};

struct Score{
    int SnakeLength;
    int GrowthItemNum;
    int PoisonItemNum;
    int UsedGateNum;
    float time;
};

struct Gate{
    Position gPos1, gPos2;
    int lifeTurn = -1;
};

class SnakeGame{

public:
    SnakeGame() = default;
    ~SnakeGame() = default;

public:
    void Init();
    bool Play();
    void Exit();

private:
    void RestartGame();

    void ScreenClear();
    void WriteStageToScreen(Stage& stage);
    void WriteSnakeToScreen(Snake& snake);
    void WriteItemToScreen();
    void WriteGateToScreen();

    bool IsClear();
    bool IsGameOver();
    void Blocking();
    void NonBlocking();

    //
    //아이템관련 메서드
    //
    void CreateItem();
    void DestructItem();
    void DestructItem(Position pos);
    Position GetRandomPos();

    //
    //게이트관련 메서드
    //
    void CreateGate();  //게이트 생성
    DPosition IndicatePassedDPos(DPosition headPos);    //게이트 통과 시에 다음 좌표와 방향 리턴
    bool CanMovePos(int x, int y);
    
    bool GameClear();

    TileType CheckBuffer(int x, int y);
    


private:
  int CLEAR=0;

    Score score, mission;
    Stage mStage;
    Renderer renderer;
    Snake mSnake;
    Timer mGameTimer;

    std::list<Item> Items;
    Gate* gate = nullptr;
    float gateStartTime = 0.0f;

    int** scrBuffer;    //스크린용 버퍼

    const char* stages[4] = {"data/stage/stage1.txt",
                            "data/stage/stage2.txt",
                            "data/stage/stage3.txt",
                            "data/stage/stage4.txt"};
    int curStage = 0;
};
