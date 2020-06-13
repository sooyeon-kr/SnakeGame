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

    bool IsGameOver();
    void Blocking();
    void NonBlocking();

    void CreateItem();
    void DestructItem(); //일정 시간에 의한 아이템 소멸 메소드
    void DestructItem(Position pos); //스네이크에 의한 아이템 소멸 메소드 오버로딩
    Position GetRandomPos();

    TileType CheckBuffer(int x, int y);

private:
    Stage mStage;
    Renderer renderer;
    Snake mSnake;
    Timer mGameTimer;
    std::list<Item> Items;
    int** scrBuffer;    //스크린용 버퍼
};
