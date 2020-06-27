#ifndef __SHARED_DATA_TYPE__
#define __SHARED_DATA_TYPE__

#define MAXROW 30 // Stage의 최대 ROW값
#define MAXCOL 30 // Stage의 최대 COL값

// 위치 값을 저장하는 구조체
struct Position{
    int x;
    int y;

    Position(int _x = 0, int _y = 0) : x(_x), y(_y){}

    // 연산자 오버로딩
    bool operator==(const Position& pos){
        return x == pos.x && y == pos.y;
    }
};

#endif