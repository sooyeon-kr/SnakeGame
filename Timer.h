#ifndef __TIMER__
#define __TIMER__

/*시간 관련 클래스*/
class Timer{
public:
    Timer() = default;
    ~Timer() = default;

public:
    // 시간을 초기화하는 메소드
    void Init();

    // 시간을 갱신하는 메소드
    void UpdateTime();

    // 델타타임을 리턴해주는 메소드
    float GetDeltaTime();

    // 경과시간을 리턴해주는 메소드
    float GetFlowTime();

private:
    // 틱을 계산하는 메소드
    unsigned int GetTickCount();

private:
    unsigned long startTime; // 시작 시간을 저장하는 변수
    unsigned long curTime; // 현재 시간을 저장하는 변수
    unsigned long prevTime; // 이전 시간을 저장하는 변수
    unsigned long delTime; // 델타타임을 저장하는 변수
};
#endif