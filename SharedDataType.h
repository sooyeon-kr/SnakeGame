#ifndef __SHARED_DATA_TYPE__
#define __SHARED_DATA_TYPE__

#define MAXROW 30
#define MAXCOL 30

//위치 값
struct Position{
    int x;
    int y;

    Position(int _x = 0, int _y = 0) : x(_x), y(_y){}

    bool operator==(const Position& pos){
        return x == pos.x && y == pos.y;
    }
};

#endif