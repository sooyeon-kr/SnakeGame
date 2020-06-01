#ifndef __Stage__H__
#define __Stage__H__

class Stage
{
    //생성자
public:
    //메소드
    bool loadStage(char* filename);

    char body='o';
    void movesnake();

    int** GetMap() { return map; }
    int GetRow() { return row; }
    int GetColumn() { return col; }

    //멤버변수
private:
  int x[30],y[30]; //뱀을 구현하는데 필요한 좌표 배열입니다.
  int newx[30], newy[30]; //뱀이 이동 방식이 i번째 몸통은 i-1번째의 좌표로 가기때문에
                            //움직이기 전의 뱀
  int length=3;
  char direction;
    int row = -1;
    int col = -1;
    int **map = nullptr;
};

#endif
