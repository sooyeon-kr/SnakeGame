#ifndef __RENDERER__
#define __RENDERER__

#include <ncurses.h> //ncurses 라이브러리를 사용하는 프로그램들의 컴파일을 위해 헤더파일 포함
#include "Stage.h"
#include "Snake.h"

class Renderer{
    
public:
    //생성자
    Renderer() = default;
    ~Renderer() = default;


public:
    //초기화 함수
    void Init();
    //스테이지와 뱀을 그리는 함수
    void Draw(Stage& stage, Snake& snake);
    WINDOW *win2;


private:
    void DrawMap(Stage& stage);
    void DrawSnake(Snake& snake);



private:



};


#endif
