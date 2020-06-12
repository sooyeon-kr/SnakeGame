#ifndef __Stage__H__
#define __Stage__H__

//Wall 종류
enum class TileType{
    Blank,
    Wall,
    ImmuneWall
};

class Stage
{
    //생성자
public:
    Stage() = default;
    ~Stage();


public:
    //메소드
    bool loadStage(char* filename); //Stage의 Map 2차원 배열 동적할당

    int** GetMap() { return map; }
    int GetRow() { return row; }
    int GetColumn() { return col; }

    bool CheckWall(int x, int y);


    //멤버변수
private:
    int row = -1;
    int col = -1;
    int **map = nullptr;
};

#endif
