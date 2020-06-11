#ifndef __Snake__
#define __Snake__
#include <vector>

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
    DPosition NextSnakePos(Direction nextDir);
    void UpdateSnakePos(DPosition& dPos);

    void Die(){isLive = false;}

    Direction GetCurDirection(){return head.Dir;}
    bool IsAlive(){return isLive;}

public:
    std::vector<Position> savesp; //savesp는 이동하기 이전의 좌표를 기억하기 위한 배열입니다.
    std::vector<Position> sp;   //sp는 뱀의 좌표 담는겁니다
    DPosition head;
    int length = 3; //뱀의 초기 길이
    bool isLive = true; //뱀이 죽었는지 살았는지 체크
};

#endif
