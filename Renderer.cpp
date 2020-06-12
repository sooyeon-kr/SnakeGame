#include "Renderer.h"
#include <fstream>
#include <memory.h>

void Renderer::Init(){
    initscr(); //ncurses 초기화

    wresize(stdscr, 200, 200 ); //스크린버퍼사이즈를 충분히 키움. 터미널크기는 제어 안됨.
	keypad(stdscr, true); // 방향키 사용
	noecho(); // 사용자로부터 받은 입력 문자를 출력X
	curs_set(0); // 커서 표시X

    start_color(); //color적용시 꼭 필요,터미널에서 지원되는 모든 색들을 초기화해서 준비
    init_color(COLOR_WHITE, 1000,1000,1000);
    init_pair((int)TileType::Wall, COLOR_RED, COLOR_WHITE); //팔레트 넘버3, 전경색 RED, 배경색 WHITE
    init_pair((int)TileType::ImmuneWall, COLOR_BLUE, COLOR_WHITE);
    init_pair((int)TileType::Snake_Head, COLOR_CYAN, COLOR_WHITE);
    init_pair((int)TileType::Snake_Body, COLOR_YELLOW, COLOR_WHITE);
    init_pair((int)TileType::Snake_Tail, COLOR_MAGENTA, COLOR_WHITE);
    init_pair((int)TileType::Size, COLOR_BLUE, COLOR_BLACK);
    bkgd(COLOR_PAIR((int)TileType::Size)); //background 지정

    //윈도우 속성 받아오기
    std::ifstream inStream("Screen.txt");
    if(!inStream.is_open()){
        printw("Error\n");
        endwin();
        return;
    }
    for(int i = 0; i < (int)WindowType::SIZE; i++){
        for(int j = 0; j < 4; j++)
            inStream >> subWinProperty[i][j];
    }
    inStream.close();

    //SubWindow 생성
    WINDOW* subscrs[(int)WindowType::SIZE];
    for(int i = 0; i < (int)WindowType::SIZE; i++){

        //서브윈도우 생성
        subscrs[i] = subwin(stdscr, subWinProperty[i][0], subWinProperty[i][1], subWinProperty[i][2], subWinProperty[i][3]);

        windows[i] = subscrs[i];
    }
}


void Renderer::Draw(int** buff){
    Present(buff);
    DrawBox(windows[(int)WindowType::SCORE]);
    DrawBox(windows[(int)WindowType::MISSION]);
}

void Renderer::Refresh(){
    refresh();
}

void Renderer::DrawBox(WINDOW* win){
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(1)); //백그라운드 컬러도 지정
}

void Renderer::Present(int** scrBuffer){
    move(0,0);
    for(int i = 0; i < MAXROW; i++){
        for(int k = 0; k < MAXCOL; k++){
            switch(scrBuffer[i][k]){
                case (int)TileType::Blank:
                // attron(COLOR_PAIR(1));
                addch(ACS_CKBOARD);
                // attroff(COLOR_PAIR(1));
                break;

                case (int)TileType::Wall:
                attron(COLOR_PAIR((int)TileType::Wall));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Wall));
                break;

                case (int)TileType::ImmuneWall:
                attron(COLOR_PAIR((int)TileType::ImmuneWall));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::ImmuneWall));
                break;

                case (int)TileType::Snake_Head:
                attron(COLOR_PAIR((int)TileType::Snake_Head));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Snake_Head));
                break;

                case (int)TileType::Snake_Body:
                attron(COLOR_PAIR((int)TileType::Snake_Body));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Snake_Body));
                break;

                case (int)TileType::Snake_Tail:
                attron(COLOR_PAIR((int)TileType::Snake_Tail));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR((int)TileType::Snake_Tail));
                break;
            }
        }
        printw("\n");
    }
}

void Renderer::PrintSystemMessage(const char* str){
    WINDOW* sysWin = subwin(stdscr, 10, 10, 10, 10);

    wattron(sysWin, COLOR_PAIR(3));
    wprintw(sysWin, str);
    wattroff(sysWin, COLOR_PAIR(3));
    refresh();

    delwin(sysWin);
}

void Renderer::PrintSystemMessage(std::string str){
    WINDOW* sysWin = subwin(stdscr, 10, 10, 10, 10);

    wattron(sysWin, COLOR_PAIR(3));
    wprintw(sysWin, str.c_str());
    wattroff(sysWin, COLOR_PAIR(3));
    refresh();

    delwin(sysWin);
}

void Renderer::PrintScoreMessage(const char* str){
    wmove(windows[(int)WindowType::SCORE], 0, 0);
    wattron(windows[(int)WindowType::SCORE], COLOR_PAIR(3));
    wprintw(windows[(int)WindowType::SCORE], str);
    wattroff(windows[(int)WindowType::SCORE], COLOR_PAIR(3));
    refresh();
}

void Renderer::End(){
    endwin();
}
