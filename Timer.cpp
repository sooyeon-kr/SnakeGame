#include "Timer.h"
#include <time.h>

void Timer::Init(){
    startTime = GetTickCount();//처음시작 시간
    curTime = startTime;//현재시간
    prevTime = startTime;//이전시간이지만 init이므로 현재시간 입력
}

void Timer::UpdateTime(){
    curTime = GetTickCount();//현재시간갱신
    delTime = curTime - prevTime;//delTIme
    prevTime = curTime;
}

float Timer::GetDeltaTime(){
    return (float)delTime/(CLOCKS_PER_SEC);
}

float Timer::GetFlowTime(){
    float time = GetTickCount();
    return (time - startTime)/(CLOCKS_PER_SEC);
}
unsigned int Timer::GetTickCount()
{
     return clock();
}