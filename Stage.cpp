#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "Stage.h"
using namespace std;

/*stage 로드 하는 함수*/
bool Stage::loadStage(char* filename)
{
    ifstream mapFile;
    mapFile.open(filename);
    if(mapFile.fail())
    {
        return false;
    }

    mapFile>>row>>col;//row, col 값 받아옴
    mapFile.ignore();

    map=(int**)malloc(row*sizeof(int*));//row pointer 공간 할당

    for(int i=0; i<row; i++)
    {
        map[i] = (int*)malloc(col*sizeof(int));//col 공간 할당
    }
    for(int i=0; i<row; i++)
    {
        for(int k=0; k<col; k++)
        {
            char temp = 0;
            mapFile >> temp;
            map[i][k] = (int)(temp-'0');
        }
    }
    mapFile.close();

}

/*stage출력 준비하는 함수*/
void Stage::printStage()
{

    start_color();
    init_color(COLOR_WHITE, 1000,1000,1000);
    init_pair(1, COLOR_RED, COLOR_WHITE); //팔레트 넘버3, 전경색 RED, 배경색 WHITE
    init_pair(2, COLOR_YELLOW, COLOR_WHITE);
    init_pair(3, COLOR_CYAN, COLOR_WHITE);

    if(row==-1||col==-1)
        return;
    else
    {
        for(int i=0; i<row; i++)
        {
            for(int k=0; k<col; k++)
            {
                switch(map[i][k]){
                    case (int)TileType::Wall:
                        attron(COLOR_PAIR(1));
                        addch(ACS_BOARD);
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
                // if(map[i][k] == 1){ //숫자 출력할 때 사용했음.
                //     addch(ACS_CKBOARD);
                // }
                // else
                //     printw("%d",map[i][k]);
            }
            printw("\n");
            refresh();
        }
    }
    for(int i=0;i<length;i++) //뱀 만들기
    {
      x[i]=row/2;         //처음에 맵 중간 즈음에 뱀을 만듭니다.
      y[i]=col/2+i;
      direction = 'l';
      move(x[i],y[i]);
      if(i==0)        //머리와 몸통을 똑같이 하시려면 지우시면 됩니다.
      addch('x');
      else
      addch(body);
  }
}

void Stage::movesnake()
{
  if(x[0] == 0 || x[0] == row-1 || y[0] ==0 || y[0] == col-1){ //벽에 닿으면 gameover
    move(15,35);
    printw("Game over");
  }
  int b=getch(); //입력받은 키를 b에
  switch(b){
    case KEY_LEFT:            //왼쪽 방향키를 눌렀을때
      if(direction == 'r'){ //현재 진행 방향과 반대일 경우 gameover
        move(15,35);
        printw("Game over");
        break;
      }
      else if(direction !='r'){ //현재 진행 방향과 반대가 아닐 경우
        direction='l';
        break;
      }
    case KEY_RIGHT:             //오른쪽 방향키
      if(direction == 'l'){   //현재 진행 방향과 반대일 경우 gameover
        move(15,35);
        printw("Game over");
        break;
      }
      else if(direction != 'l'){ //현재 진행 방향과 반대가 아닐 경우
        direction='r';
        break;
      }
    case KEY_UP:                //위쪽 방향키
      if(direction == 'd'){   //현재 진행 방향과 반대일 경우 gameover
        move(15,35);
        printw("Game over");
        break;
      }
      else if(direction != 'd'){  //현재 진행 방향과 반대가 아닐 경우
        direction='u';
        break;
      }
    case KEY_DOWN:              //아래쪽 방향키
      if(direction == 'u'){   //현재 진행 방향과 반대일 경우 gameover
        move(15,35);
        printw("Game over");
        break;
      }
      else if(direction != 'u'){  //현재 진행 방향과 반대가 아닐 경우
        direction='d';
        break;
      }
  }

  for(int i=0;i<length;i++){ //움직이기 전의 좌표를 보존하기 위해 따로 배열에 넣었습니다.
    newx[i]=x[i];
    newy[i]=y[i];
  }
  for(int i=0;i<length;i++){
    if(i==0){                 //머리부분은 다른 모양으로 만들기 위해 이 부분을 만들었습니다.
      if(direction == 'l'){   //만약 머리 몸통 같은 모양으로 하신다면 지우시면됩니다.
        y[i]-=1;
        move(x[i],y[i]);
        addch('x');
      }
      else if(direction == 'r'){
        y[i]+=1;
        move(x[i],y[i]);
        addch('x');
      }
      else if(direction == 'u'){
        x[i]-=1;
        move(x[i],y[i]);
        addch('x');
      }
      else if(direction == 'd'){
        x[i]+=1;
        move(x[i],y[i]);
        addch('x');
      }
      refresh();
    }
    else if(i!=0){                   //i-1번째 좌표로 i번째 몸통이 움직입니다.
      x[i]=newx[i-1];
      y[i]=newy[i-1];
      move(x[i],y[i]);
      addch(body);
    }
    move(newx[length-1],newy[length-1]); //기존의 꼬리 좌표는 다시 blank로 바꿉니다
    attron(COLOR_PAIR(3));
    addch(ACS_BOARD);
    attroff(COLOR_PAIR(3));

  refresh();

}
}
