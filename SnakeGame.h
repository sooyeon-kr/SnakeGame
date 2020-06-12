#include "Renderer.h"
#include "Timer.h"

struct Item{
    TileType ItemType;
    Position Pos;
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

    void WriteStageToScreen(Stage& stage);
    void WriteSnakeToScreen(Snake& snake);

    bool IsGameOver();
    void Blocking();
    void NonBlocking();

private:
    Stage mStage;
    Renderer renderer;
    Snake mSnake;
    Timer mGameTimer;
    int** scrBuffer;    //스크린용 버퍼

};
