#include <ncurses.h>
#include <iostream>
#include <fstream>
#include "Stage.h"
using namespace std;

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

/*stage출력 준비하는 함수*/
void Stage::printStage()
{
    start_color();
    init_color(COLOR_WHITE, 1000,1000,1000);
    init_pair(1, COLOR_RED, COLOR_WHITE); //팔레트 넘버3, 전경색 RED, 배경색 WHITE
    init_pair(2, COLOR_YELLOW, COLOR_WHITE);
    init_pair(3, COLOR_CYAN, COLOR_WHITE);

    if(row==-1||col==-1)
        return;
    else
    {
        for(int i=0; i<row; i++)
        {
            for(int k=0; k<col; k++)
            {
                switch(map[i][k]){
                    case (int)TileType::Wall:
                        attron(COLOR_PAIR(1));
                        addch(ACS_BOARD);
                        attroff(COLOR_PAIR(1));
                        break;
                    case (int)TileType::ImmuneWall:
                        attron(COLOR_PAIR(2));
                        addch(ACS_CKBOARD);
                        attroff(COLOR_PAIR(2));
                        break;
                    case (int)TileType::Blank:
                        attron(COLOR_PAIR(3));
                        addch(ACS_BOARD);
                        attroff(COLOR_PAIR(3));
                        break;
                }
                // if(map[i][k] == 1){ //숫자 출력할 때 사용했음.
                //     addch(ACS_CKBOARD);
                // }
                // else
                //     printw("%d",map[i][k]);
            }
            printw("\n");
            refresh();
        }    
    }
}