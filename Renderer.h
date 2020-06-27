#ifndef __RENDERER__
#define __RENDERER__

#include "Stage.h"
#include "Snake.h"
#include <ncurses.h> //ncurses 라이브러리를 사용하는 프로그램들의 컴파일을 위해 헤더파일 포함
#include <string>
#include <vector>
#include <unordered_map>
#include <list>

// 서브 윈도우의 유형을 저장하는 enum class형
enum class WindowType
{
    SCORE,
    MISSION,
    SIZE
};

/*화면 출력을 담당하는 클래스*/
class Renderer
{

public:
    Renderer() = default; // default 생성자
    ~Renderer() = default; // default 소멸자

public:
    // 화면을 초기화 하는 메소드
    void Init();
    // 서브윈도우에 박스를 그리는 메소드
    void DrawUI();
    // 스테이지와 뱀을 그리는 함수
    void Draw(int **scrBuffer);
    // 서브윈도우를 refresh하는 메소드
    void Refresh();
    // 화면을 끝내는 메소드
    void End();
    // 서브윈도우를 공백으로 채우는 메소드
    void ClearWindow();
    // 시스템 메세지를 출력하는 메소드
    void PrintSystemMessage(const char *str);
    // Score 서브 윈도우에 문자열을 출력하는 메소드
    void PrintScoreMessage(const char *str);
    // 출력될 문자열의 위치를 지정하여 출력하는 메소드
    void PrintScoreMessageXY(int x, int y, const char *str);
    // Mission 서브 윈도우에 문자열을 출력하는 메소드
    void PrintMissionMessage(const char *str);
    // 출력된 문자열의 위치를 지정하여 출력하는 메소드
    void PrintMissionMessageXY(int x, int y, const char *str);

private:
    // 서브 윈도우에 박스를 그리고 배경화면을 지정하는 메소드
    void DrawBox(WINDOW *win);

private:
    // 서브 윈도우 배열
    WINDOW *windows[4];
    // 서브 윈도우의 크기 및 위치를 저장하는 배열
    int subWinProperty[(int)WindowType::SIZE][4];
};
#endif
