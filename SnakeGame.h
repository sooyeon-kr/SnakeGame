#include "Renderer.h"
#include "Timer.h"
#include <list>

// 아이템의 정보를 가지고 있는 구조체
struct Item
{
    // GrowthItem인지 PoisonItem인지 값을 저장하기 위한 TileType형의 변수
    TileType ItemType;
    // Item 출현 위치값을 저장하는 Position형의 변수
    Position Pos;
    // 소멸 시간을 파악하기 위해 살아있는 시간을 계산하는 Timer 객체
    Timer timer;
    // 소멸 시간 지정
    float lifeTime = 10;
};

// 점수 항목들을 가지고 있는 구조체
struct Score
{
    // Snake의 몸 길이
    int SnakeLength;
    // Snake가 획득한 GrowthItem의 수
    int GrowthItemNum;
    // Snake가 획득한 PoisonItem의 수
    int PoisonItemNum;
    // Snake가 통과한 Gate의 수
    int UsedGateNum;
    // Snake가 한 스테이지에서 소요한 시간
    float time;
};

// 한 쌍의 게이트 정보를 가지고 있는 구조체
struct Gate
{
    // 한 쌍의 게이트 위치를 각각 저장하기 위한 객체
    Position gPos1, gPos2;
    // Snake가 gate를 통과했을 경우 Gate 소멸을 위해 틱 계산 변수
    int lifeTurn = -1;
};

// 게임 실행을 전반적으로 관리하는 클래스
class SnakeGame
{

public:
    SnakeGame() = default; // dafault 생성자
    ~SnakeGame() = default; // default 소멸자

public:
    // 게임을 초기화하기 위한 메소드
    void Init();
    // 게임을 실행하기 위한 메소드
    bool Play();
    // 게임을 끝내기 위한 메소드
    void Exit();

private:
/*화면 출력 관련 메소드*/
    // scrBuffer를 모두 비워주는 메소드
    void ScreenClear();
    // Stage 정보를 scrBuffer에 저장하는 메소드
    void WriteStageToScreen(Stage &stage);
    // Snake 정보를 scrBuffer에 저장하는 메소드
    void WriteSnakeToScreen(Snake &snake);
    // Item 정보를 scrBuffer에 저장하는 메소드
    void WriteItemToScreen();
    // Gate 정보를 scrBuffer에 저장하는 메소드
    void WriteGateToScreen();

/*게임 (재)시작 및 미션 성공 관련 메소드*/
    // 게임 재시작 여부를 return 해주는 메소드
    bool IsGameOver();
    // 게임을 재시작할 때 호출되는 메소드
    void RestartGame();
    // 미션 성공 여부를 score와 mission의 멤버변수로 비교하여 리턴해주는 메소드
    bool IsClear();
    // 미션을 성공했을 경우, 호출되며 다음 스테이지로 실행 여부 메세지 출력 및 사용자 키를 입력 받는 메소드
    bool GameClear();
    
    // x, y값을 받아 그 좌표에 해당하는 TileType이 무엇인지 return하는 메소드
    TileType CheckBuffer(int x, int y);

    // x, y의 위치가 진입할 수 있는 위치인지 판단하여 return 해주는 메소드
    bool CanMovePos(int x, int y);

/*동기화관련 메소드*/
    // Blocking 설정 메소드
    void Blocking();
    // NonBlocking 설정 메소드
    void NonBlocking();

/*아이템관련 메소드*/
    // 아이템을 생성하는 메소드
    void CreateItem();
    // 일정 시간이 지났을 경우 아이템을 소멸시키는 메소드
    void DestructItem();
    // Snake가 아이템을 습득했을 경우 아이템을 소멸시키는 메소드
    void DestructItem(Position pos);

/* 게이트관련 메소드 */
    // 게이트를 생성해주는 메소드
    void CreateGate();                              
    // 게이트 통과 시에 Snake의 Head의 좌표와 방향 리턴해주는 메소드
    DPosition IndicatePassedDPos(DPosition headPos);
    // 벽인지 아닌지 체크하여 벽일 경우 false를 바로 리턴해주고 blank일 경우 true를 리턴해주는 메소드


private:
    //현재 Stage를 나타내고, 미션 성공 시 Stage를 변경하기 위한 변수
    int stageStep = 0;

    // 현재 점수와 미션을 화면에 출력하고, 미션 성공 여부 및 게임 오버 판단을 위한 Score 객체
    Score score, mission;
    // Stage 객체
    Stage mStage;
    // Renderer 객체
    Renderer renderer;
    // Snake 객체
    Snake mSnake;
    //게임의 시간 관리를 위한 Timer 객체
    Timer mGameTimer;
    // Growth 및 Poison Item의 정보를 저장하기 위한 list
    std::list<Item> Items;
    // Gate 한 쌍의 정보 저장하기 위한 변수
    Gate *gate = nullptr;

    // 화면 출력 시, 요소에 따라 출력을 달리하기 위해 요소들의 TileType을 갖고 있는 TileMap형의 scrBuffer
    TileMap scrBuffer;

    // 각각의 Stage의 파일 경로 저장
    const char *stages[4] = {"data/stage/stage1.txt",
                             "data/stage/stage2.txt",
                             "data/stage/stage3.txt",
                             "data/stage/stage4.txt"};

    // SnakeLen, GrowthIt, PosionIt, GateNum, Time
    // 각각의 Stage 미션 조건 설정 및 저장
    Score missions[4] = {{5, 2, 5, 2, 100.0f},
                         {6, 3, 4, 3, 150.0f},
                         {8, 3, 3, 4, 200.0f},
                         {10, 3, 2, 5, 300.0f}};

    // 각각의 Stage 시작 시,  Snake의 출현 위치 설정 및 저장
    Position snakeInitPos[4] = {{10, 10},
                                {15, 17},
                                {16, 14},
                                {22, 22}};
};
