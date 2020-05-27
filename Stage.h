#ifndef __Stage__H__
#define __Stage__H__
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
    void printStage();

    //멤버변수
private:
    int row = -1;
    int col = -1;
    int **map = nullptr;
};

#endif