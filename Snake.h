#ifndef __Snake__
#define __Snake__
#include <vector>
#include <list>
#include "SharedDataType.h"

//방향 종류
enum Direction{
    UP ,
    RIGHT,
    DOWN,
    LEFT ,
    SIZE 
};

//헤드
struct DPosition{
    Direction Dir;
    Position Pos;

};

class Snake{
public:
    Snake() = default;
    ~Snake() = default;

public:
    //초기화
    void Init(int x, int y, int dir);
    //키 입력받아 다음 방향 결정하는 함수
    DPosition NextSnakeHeadPos(Direction nextDir);
    void UpdateSnakePos(DPosition& dPos);

    void Die(){isLive = false;}
    void Clear(){isClear = true;}
    // void IsClear(){return isClear;}

    Direction GetCurDirection(){return head.Dir;}
    int GetSnakeLength(){return body.size() + 1;}
    int GetSnakeMaxLength(){return maxLength;}
    bool IsAlive(){return isLive;}
    bool IsClear(){return isClear;}

public:
    DPosition head; //뱀의 머리의 방향값과 좌표값을 가지고 있음
    std::list<Position> body;
    const int maxLength = 15;
    const int minLength = 3;
    bool isLive = true; //뱀이 죽었는지 살았는지 체크
    bool isClear = false;
};

#endif
