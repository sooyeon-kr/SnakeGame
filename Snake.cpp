#include "Snake.h"
#include <ncurses.h>

using namespace std;

void Snake::Init(){
    head.Dir = Direction::RIGHT;
    head.Pos.x = 10;
    head.Pos.y = 10;
}

DPosition Snake::NextSnakePos(Direction nextDir)
{


  //다음 방향 계산
  DPosition nextPos;
  nextPos.Dir = nextDir;
  Position curPos = { head.Pos };
  
  switch(nextDir){
    case Direction::LEFT:
      nextPos.Pos.x = curPos.x - 1;
      nextPos.Pos.y = curPos.y;
      break;
    case Direction::RIGHT:
      nextPos.Pos.x = curPos.x + 1;
      nextPos.Pos.y = curPos.y;
      break;
    case Direction::UP:
      nextPos.Pos.x = curPos.x;
      nextPos.Pos.y = curPos.y - 1;
      break;
    case Direction::DOWN:
      nextPos.Pos.x = curPos.x;
      nextPos.Pos.y = curPos.y + 1;
      break;
  }


return nextPos;


/*
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
*/
}


void Snake::UpdateSnakePos(DPosition& dPos){
  head.Dir = dPos.Dir;
  head.Pos = dPos.Pos;
}