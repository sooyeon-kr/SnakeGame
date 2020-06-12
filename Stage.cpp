#include <fstream>
#include "Stage.h"
using namespace std;

Stage::~Stage(){
    if(map != nullptr){
        for(int i=0; i<row; i++)
            delete map[i];
        delete[] map;

        map = nullptr;
    }
}

/*stage 로드 하는 함수*/
bool Stage::loadStage(char* filename)
{
    ifstream mapFile;
    mapFile.open(filename);
    if(mapFile.fail())
    {
        return false;
    }

    mapFile>>row>>col;//row, col 값 받아옴
    mapFile.ignore();

    map=(int**)malloc(row*sizeof(int*));//row pointer 공간 할당

    for(int i=0; i<row; i++)
    {
        map[i] = (int*)malloc(col*sizeof(int));//col 공간 할당
    }
    for(int i=0; i<row; i++)
    {
        for(int k=0; k<col; k++)
        {
            char temp = 0;
            mapFile >> temp;
            map[i][k] = (int)(temp-'0');
        }
    }
    mapFile.close();

}