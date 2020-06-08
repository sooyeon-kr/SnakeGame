#include "Renderer.h"
#include <fstream>

void Renderer::Init(){
    initscr(); //ncurses 초기화

    wresize(stdscr, 200, 200 ); //스크린버퍼사이즈를 충분히 키움. 터미널크기는 제어 안됨.
	keypad(stdscr, true); // 방향키 사용
	noecho(); // 사용자로부터 받은 입력 문자를 출력X
	curs_set(0); // 커서 표시X

    start_color(); //color적용시 꼭 필요,터미널에서 지원되는 모든 색들을 초기화해서 준비
    init_color(COLOR_WHITE, 1000,1000,1000);
    init_pair(1, COLOR_RED, COLOR_WHITE); //팔레트 넘버3, 전경색 RED, 배경색 WHITE
    init_pair(2, COLOR_YELLOW, COLOR_WHITE);
    init_pair(3, COLOR_CYAN, COLOR_WHITE);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    bkgd(COLOR_PAIR(4)); //background 지정

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

    // refresh();
}

void Renderer::ClearScreen(){

}

void Renderer::Draw(Stage& stage, Snake& snake){
    DrawMap(stage);
    DrawSnake(snake);
    DrawBox(windows[(int)WindowType::SCORE]);
    DrawBox(windows[(int)WindowType::MISSION]);
}

void Renderer::Refresh(){
    refresh();
}

void Renderer::DrawMap(Stage& stage){
    //스테이지에 대한 정보를 받아옴
    auto map = stage.GetMap();
    int row = stage.GetRow();
    int col = stage.GetColumn();
    
    //커서 옮김
    move( 0,0);

    //벽 출력
    for(int i=0; i<row; i++)
    {
        for(int k=0; k<col; k++)
        {
            switch(map[i][k]){
                case (int)TileType::Wall:
                    attron(COLOR_PAIR(1));
                    addch(ACS_CKBOARD);
                    attroff(COLOR_PAIR(1));
                    break;
                case (int)TileType::ImmuneWall:
                    attron(COLOR_PAIR(2));
                    addch(ACS_CKBOARD);
                    attroff(COLOR_PAIR(2));
                    break;
                case (int)TileType::Blank:
                    attron(COLOR_PAIR(3));
                    addch(ACS_CKBOARD);
                    attroff(COLOR_PAIR(3));
                    break;
            }
        }
        printw("\n");
    }

}


void Renderer::DrawSnake(Snake& snake){
    
    //뱀을 그릴 위치와 뱀의 연결된 다음 방향 
    Position drawPos = snake.head.Pos;
    Direction nextPos = snake.head.Dir;

    for(int i=0;i<snake.length;i++) //뱀 만들기
    {
        //헤드일 경우
        if(drawPos.x == snake.head.Pos.x && drawPos.y == snake.head.Pos.y){
            attron(COLOR_PAIR(2));
            mvaddch(drawPos.y, drawPos.x, ACS_CKBOARD);
            attroff(COLOR_PAIR(2));
        }
        //꼬리일 경우
        else if((i + 1) == snake.length){
            attron(COLOR_PAIR(1));
            mvaddch(drawPos.y, drawPos.x, ACS_CKBOARD);
            attroff(COLOR_PAIR(1));
        }
        //나머지 몸 부분
        else{
            mvaddch(drawPos.y, drawPos.x, ACS_CKBOARD);
        }

        //뱀의 다음을 그릴 위치 결정
        switch(nextPos){
            case Direction::UP:
            drawPos.y += 1;
            break;

            case Direction::RIGHT:
            drawPos.x -= 1;
            break;

            case Direction::LEFT:
            drawPos.x += 1;
            break;

            case Direction::DOWN:
            drawPos.y -= 1;
            break;
        }
        
    }
}

void Renderer::DrawBox(WINDOW* win){
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(1)); //백그라운드 컬러도 지정
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