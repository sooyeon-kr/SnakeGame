#include "SnakeGame.h"
#include <unistd.h>


void SnakeGame::Init(){
    renderer.Init();
    mStage.loadStage("stage1");
    mSnake.Init();

}

void SnakeGame::Play(){
    /*틱마다 갱신 후 출력*/
    while(1){
        //뱀의 다음 방향 결정인데, 현재는 right로 해놨으나 같이 상의 후 랜덤이나 한 값으로 결정하면 좋을 것 같습니다.
        mSnake.SelectDirectionSnake();




        //해주시면 될 부분
        //뱀의 다음 방향의 타일 종류 파악 후
        //if(다음 위치 타일이 벽인 경우, mstage)break;
        /*"stage.h 보시면 int **map 이 있는데, 이거 이용하셔서 함수를 만드신 후에 매개변수로 위치값 y, x좌표를 넘겨서 그 값이 0인지 1인지 2인지에 따라
        타일의 종류를.파악하면 될 것
        같습니다*/
        if(!(mStage.CheckWall(mSnake.head.Pos.x,mSnake.head.Pos.y)))
        break;

        //아닌경우 뱀의 현재위치 업데이트
        renderer.Draw(mStage, mSnake);

        sleep(5);
    }
}
