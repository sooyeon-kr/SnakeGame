#include "Renderer.h"
#include "Timer.h"

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

    bool IsGameOver();
    void Blocking();
    void NonBlocking();

private:
    Stage mStage;
    Renderer renderer;
    Snake mSnake;
    Timer mGameTimer;
};
