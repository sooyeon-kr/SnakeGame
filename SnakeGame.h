#include "Renderer.h"


class SnakeGame{

public:
    SnakeGame() = default;
    ~SnakeGame() = default;

public:
    void Init();
    void Play();

private:
    Stage mStage;
    Renderer renderer;
    Snake mSnake;
};
