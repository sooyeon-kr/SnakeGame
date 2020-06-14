#include "Renderer.h"
#include "Timer.h"
#include <list>

struct Item{
    TileType ItemType;
    Position Pos;
    Timer timer;
    float lifeTime = 10;
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
    void WriteGate(int gatex,int gatey, int gatex2, int gatey2);

    bool IsGameOver();
    void Blocking();
    void NonBlocking();

    void CreateItem();
    void CreateGate(int &x, int &y, int &x2, int &y2);
    void DestructItem();
    void DestructItem(Position pos);
    Position GetRandomPos();
    bool GameClear();

    TileType CheckBuffer(int x, int y);

private:
  int gatenum=0;  //한쌍의 게이트만 만들어지도록 하는 변수
  int through=0;  //한쌍의 게이트만 만들어지도록 하는 변수
  float gateT = 0.0f;
  int CLEAR=0;

    Stage mStage;
    Renderer renderer;
    Snake mSnake;
    Timer mGameTimer;
    std::list<Item> Items;
    int** scrBuffer;    //스크린용 버퍼
};
