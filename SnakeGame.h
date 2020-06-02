#include "Renderer.h"


class SnakeGame{

public:
    SnakeGame() = default;
    ~SnakeGame() = default;

public:
    void Init();
    void Play();
    bool CheckWall(int x, int y);


private:
    Stage mStage;
    Renderer renderer;
    Snake mSnake;
};
