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
    num++;      //num을 없애고 아래 부분에서도 num이 없다는 가정하에
                // 코드를 짜게되면 처음에 시작할때 몸통이 출력이 안되서요.
                //그래서 맨 처음 시작할때 머리, 몸통, 꼬리를 다 출력하기 위해서
                //  첫 1회만 따로 구분하도록 한것입니다
                //설명이 부족하다면 연락주시면 감사하겠습니다.
    //뱀을 그릴 위치와 뱀의 연결된 다음 방향
    Direction nextPos = snake.head.Dir; //방향

    for(int i=0;i<snake.length;i++){  //이 과정을 통해 이동하기 이전의 뱀의 좌표를
      snake.savesp[i] = snake.sp[i];  // savesp에 넣습니다
    }
    for(int i=0;i<snake.length;i++){
      if(i==0){                     //머리부분
        switch(nextPos){
            case Direction::UP:
            snake.sp[i].y -= 1;
            attron(COLOR_PAIR(2));
            mvaddch(snake.sp[i].y,snake.sp[i].x,ACS_CKBOARD);
            attroff(COLOR_PAIR(2));
            break;

            case Direction::RIGHT:
            snake.sp[i].x += 1;
            attron(COLOR_PAIR(2));
            mvaddch(snake.sp[i].y,snake.sp[i].x,ACS_CKBOARD);
            attroff(COLOR_PAIR(2));
            break;

            case Direction::LEFT:
            snake.sp[i].x -= 1;
            attron(COLOR_PAIR(2));
            mvaddch(snake.sp[i].y,snake.sp[i].x,ACS_CKBOARD);
            attroff(COLOR_PAIR(2));
            break;

            case Direction::DOWN:
            snake.sp[i].y += 1;
            attron(COLOR_PAIR(2));
            mvaddch(snake.sp[i].y,snake.sp[i].x,ACS_CKBOARD);
            attroff(COLOR_PAIR(2));
            break;
        }
      }
      else if(i!=0 && i!=snake.length-1){   //몸통 부분
        snake.sp[i] = snake.savesp[i-1];
        mvaddch(snake.sp[i].y, snake.sp[i].x, ACS_CKBOARD);

      }
      else if(i==snake.length-1){           //꼬리 부분
        if(num==1){                 //위에서 말씀드린 num을 쓰는 곳입니다.
          attron(COLOR_PAIR(1));    //첫 시작할때만 강제로 좌표를 설정해서
          mvaddch(10, 9, ACS_CKBOARD);  // 몸통을 출력하도록 합니다.
          attroff(COLOR_PAIR(1));
        }
        else if(num!=1){
        snake.sp[i] = snake.savesp[i-1];
        attron(COLOR_PAIR(1));
        mvaddch(snake.sp[i].y, snake.sp[i].x, ACS_CKBOARD);
        attroff(COLOR_PAIR(1));
      }
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
