#ifndef __Stage__H__
#define __Stage__H__
#include "SharedDataType.h"

//타일 종류
enum class TileType{
    Blank,
    Wall,
    ImmuneWall,
    Snake_Head,
    Snake_Body,
    Snake_Tail,
    Item_Growth,
    Item_Poison,
    Gate,
    Gate2,
    Size
};
class TileMap{

public:
    ~TileMap() {     
        if(map != nullptr){
            for(int i=0; i<row; i++)
                delete map[i];
            delete[] map;

        map = nullptr;
        }
    }

public:
    void Init(int x, int y);
    Position GetRandomPos(TileType type);

    int** GetMap(){return map;}
    int GetCol(){return col;}
    int GetRow(){return row;}

    void SetTile(int x, int y, int type);

private:
    //map관련 int** map;
    int row = -1;
    int col = -1;
    int **map = nullptr;
};

class Stage
{
    //생성자
public:

public:
    //메소드
    bool loadStage(const char* filename); //Stage의 Map 2차원 배열 동적할당

    Position GetRandomPos(TileType type) { return map.GetRandomPos(type);}
    int** GetMap() { return map.GetMap(); }
    int GetRow() { return map.GetRow(); }
    int GetColumn() { return map.GetCol(); }

    //멤버변수
private:
    TileMap map;
};

#endif
