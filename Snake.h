#ifndef __Snake__
#define __Snake__
#include <vector>
#include <list>

//방향 종류
enum class Direction{
    UP,
    RIGHT,
    LEFT,
    DOWN
};

//위치 값
struct Position{
    int x;
    int y;

    Position(int _x = 0, int _y = 0) : x(_x), y(_y){}
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
    void Init();
    //키 입력받아 다음 방향 결정하는 함수
    DPosition NextSnakeHeadPos(Direction nextDir);
    void UpdateSnakePos(DPosition& dPos);

    void Die(){isLive = false;}
    void Clear(){isClear = true;}
    // void IsClear(){return isClear;}

    Direction GetCurDirection(){return head.Dir;}
    int GetSnakeLength(){return body.size() + 1;}
    bool IsAlive(){return isLive;}
    bool IsClear(){return isClear;}

public:
    // std::vector<Position> savesp; //savesp는 이동하기 이전의 좌표를 기억하기 위한 배열입니다.
    // std::vector<Position> sp;   //sp는 뱀의 좌표 담는겁니다
    DPosition head; //뱀의 머리의 방향값과 좌표값을 가지고 있음
    std::list<Position> body;
    bool isLive = true; //뱀이 죽었는지 살았는지 체크
    bool isClear = false;
};

#endif
