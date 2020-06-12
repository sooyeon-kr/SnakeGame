#ifndef __TIMER__
#define __TIMER__

class Timer{
public:
    Timer() = default;
    ~Timer() = default;

public:
    //초기화
    void Init();

    //타이머 리셋
    void ResetTimer();

    //댈타타임갱신
    void UpdateTime();

    //델타타임가져오는 함수
    float GetDeltaTime();

    //경과시간을 리턴해주는 함수
    float GetFlowTime();


private:
    //틱 계산 함수
    unsigned int GetTickCount();


private:
    unsigned long startTime;
    unsigned long curTime;
    unsigned long prevTime;
    unsigned long endTime;
    unsigned long delTime;

};
#endif