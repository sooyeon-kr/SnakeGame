#ifndef __Stage__H__
#define __Stage__H__
#include "SharedDataType.h"

// 타일 종류를 저장하는 enum class형(열거형)
enum class TileType
{
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

/**/
class TileMap
{
public:
    // 소멸자
    ~TileMap()
    {
        if (map != nullptr)
        {
            for (int i = 0; i < row; i++)
                delete map[i];
            delete[] map;

            map = nullptr;
        }
    }

public:
    // Stage를 초기화 하는 메소드
    void Init(int x, int y);
    //Item이 생성되는 위치를 리턴해주는 메소드
    Position GetRandomPos(TileType type);
    // 맵을 리턴해주는 메소드
    int **GetMap() { return map; }
    // col값을 리턴해주는 메소드
    int GetCol() { return col; }
    // row값을 리턴해주는 메소드
    int GetRow() { return row; }
    // x, y에 type을 저장하는 메소드
    void SetTile(int x, int y, int type);

private:
    //map관련 int** map;
    int row = -1;
    int col = -1;
    int **map = nullptr;
};

/*Stage관련 클래스*/
class Stage
{
    //생성자
public:

    //메소드
public:
    // 스테이지를 load하는 메소드
    bool loadStage(const char *filename); //Stage의 Map 2차원 배열 동적할당
    // type에 따라 랜덤한 좌표를 리턴하는 메소드
    Position GetRandomPos(TileType type) { return map.GetRandomPos(type);}
    // 맵 객체를 받아오는 메소드
    int **GetMap() { return map.GetMap();}
    // 맵의 row값을 받아오는 메소드
    int GetRow() { return map.GetRow();}
    // 맵의 col값을 받아오는 메소드
    int GetColumn() { return map.GetCol();}

    //멤버변수
private:
    // Stage의 데이터를 저장하는 변수
    TileMap map;
};
#endif
