#include <fstream>
#include <vector>
#include "Stage.h"
using namespace std;

void TileMap::Init(int col, int row)
{
    this->col = col;
    this->row = row;

    if (map == nullptr)
    {
        map = (int **)malloc(row * sizeof(int *)); //row pointer 공간 할당

        for (int i = 0; i < row; i++)
            map[i] = (int *)malloc(col * sizeof(int)); //col 공간 할당
    }
}

//Item의 생성 위치 결정 메소드
Position TileMap::GetRandomPos(TileType type)
{
    std::vector<Position> candidate;

    for (int i = 0; i < MAXROW; i++)
        for (int j = 0; j < MAXCOL; j++)
            if (map[i][j] == (int)type)
                candidate.emplace_back(j, i);

    int randIdx = rand() % candidate.size();

    return candidate[randIdx];
}

void TileMap::SetTile(int x, int y, int type)
{
    map[y][x] = type;
}

/*stage 로드 하는 함수*/
bool Stage::loadStage(const char *filename)
{
    ifstream mapFile;
    mapFile.open(filename);
    if (mapFile.fail())
    {
        return false;
    }

    int row, col;
    mapFile >> row >> col; //row, col 값 받아옴
    mapFile.ignore();

    map.Init(col, row);

    for (int i = 0; i < row; i++)
    {
        for (int k = 0; k < col; k++)
        {
            char temp = 0;
            mapFile >> temp;
            map.SetTile(k, i, temp - '0');
        }
    }

    mapFile.close();
}
