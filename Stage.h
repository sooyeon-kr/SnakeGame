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
    //메소드
    bool loadStage(char* filename);

    int** GetMap() { return map; }
    int GetRow() { return row; }
    int GetColumn() { return col; }

    bool CheckWall(int x, int y);


    //멤버변수
private:
    int length=3;

    int row = -1;
    int col = -1;

    int **map = nullptr;
};

#endif
