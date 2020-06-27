#ifndef __Snake__
#define __Snake__
#include <vector>
#include <list>
#include "SharedDataType.h"

// 방향 종류를 저장하는 열거형
enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SIZE
};

// 방향과 좌표를 저장하는 구조체
struct DPosition
{
    Direction Dir;
    Position Pos;
};

/*Snake관련 클래스*/
class Snake
{
public:
    Snake() = default; // 생성자
    ~Snake() = default; // 디폴트

public:
    // Snake의 데이터를 초기화하는 메소드
    void Init(int x, int y, int dir);
    // 다음 방향 결정하여 리턴하는 메소드
    DPosition NextSnakeHeadPos(Direction nextDir);
    // Snake의 body위치를 업데이트하는 메소드
    void UpdateSnakePos(DPosition &dPos);
    // Snake의 방향을 리턴해주는 메소드
    Direction &GetCurDirection() { return head.Dir; }
    // Snake의 위치를 리턴해주는 메소드
    Position &GetCurHeadPos() { return head.Pos; }
    // Snake의 body를 리턴해주는 메소드
    std::list<Position> &GetCurBodyPos() { return body; }
    // Snake의 현재 길이를 리턴해주는 메소드
    int GetSnakeLength() { return body.size() + 1; }
    // Snake의 최대 길이를 리턴해주는 메소드
    int GetSnakeMaxLength() { return maxLength; }
    // Snake의 상태를 죽음으로 설정하는 메소드
    void Die() { isLive = false; }
    // Snake가 살아있는지 여부를 리턴해주는 메소드
    bool IsAlive() { return isLive; }

private:
    // 방향과 좌표를 갖는 Snake의 head 변수
    DPosition head;
    // 좌표를 갖는 list형의 body 변수
    std::list<Position> body;
    // Snake의 최대 길이를 저장하는 변수
    const int maxLength = 15;
    // Snake의 최소 길이를 저장하는 변수
    const int minLength = 3;
    // 뱀이 죽었는지 살았는지 체크하는 변수
    bool isLive = true;
};
#endif
