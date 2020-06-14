#ifndef __RENDERER__
#define __RENDERER__

#include "Stage.h"
#include "Snake.h"
#include <ncurses.h> //ncurses 라이브러리를 사용하는 프로그램들의 컴파일을 위해 헤더파일 포함
#include <string>
#include <vector>
#include <unordered_map>
#include <list>

#define MAXROW 32
#define MAXCOL 32
enum class WindowType{
    SCORE,
    MISSION,
    SIZE
};




class Renderer{

public:
    //생성자
    Renderer() = default;
    ~Renderer() = default;


public:
    //초기화 함수
    void Init();

    void DrawUI();
    //스테이지와 뱀을 그리는 함수
    void Draw(int** scrBuffer);
    void Refresh();

    void End();

    void PrintSystemMessage(const char* str);
    void PrintSystemMessage(std::string str);

    void PrintScoreMessage(const char* str);

    void PrintMissionMessage(const char* str);

private:
    void DrawBox(WINDOW* win);

private:
    WINDOW* windows[4]; //서브 윈도우 배열
    int subWinProperty[(int)WindowType::SIZE][4];
};


#endif
