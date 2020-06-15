#include "Snake.h"

using namespace std;

void Snake::Init(){
  //해드 초기값
    head.Dir = Direction::RIGHT;
    head.Pos.x = 10;
    head.Pos.y = 10;

    //바디 초기값
    body.clear();
    body.emplace_back(9, 10);
    body.emplace_back(8, 10);

    isLive = true;
    isClear = false;

}

DPosition Snake::NextSnakeHeadPos(Direction nextDir)
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
}


void Snake::UpdateSnakePos(DPosition& dPos){

  //바디 갱신
  for(auto it = body.rbegin(); it != body.rend(); ){
    auto prevIt = it;
    it++;
    if(it == body.rend()){
      prevIt->x = head.Pos.x;
      prevIt->y = head.Pos.y;
      break;
    }
    prevIt->x = it->x;
    prevIt->y = it->y;
  }

  //헤드 갱신
  head.Dir = dPos.Dir;
  head.Pos = dPos.Pos;

}
