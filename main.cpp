#include <ncurses.h> //ncurses 라이브러리를 사용하는 프로그램들의 컴파일을 위해 헤더파일 포함
#include <fstream>
#include <iostream>
#include "Stage.h"
using namespace std;

int main()
{
    setlocale(LC_ALL, ""); //로케일 설정, 유니코드 출력을 위해. 안 하면 cmd에 #표시가 됨.

    initscr(); //ncurses 초기화

    wresize(stdscr, 200, 200 ); //스크린버퍼사이즈를 충분히 키움. 터미널크기는 제어 안됨.
	keypad(stdscr, true); // 방향키 사용
	noecho(); // 사용자로부터 받은 입력 문자를 출력X
	curs_set(0); // 커서 표시X

    start_color(); //color적용시 꼭 필요,터미널에서 지원되는 모든 색들을 초기화해서 준비
    init_pair(1, COLOR_BLACK, COLOR_BLACK); //글자색, 배경색 순서
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1)); //background 지정
    Stage st1; //stage1 객체 생성
    st1.loadStage("stage1.txt"); //stage1.txt 로드

    /*틱마다 갱신 후 출력해야하므로 나중에 반복문 안에 들어가게 하면 될 듯?*/
    st1.printStage(); //화면출력
    refresh(); //화면 

    getch();

    endwin();

    return 0;
    
}