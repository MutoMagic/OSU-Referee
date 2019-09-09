#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <windows.h>

// �������ܼ�������Ƶ�ʡ����ܼ�������Ƶ����ϵͳ����ʱ�ǹ̶��ģ����������д������ж���һ�µġ�
// ��ˣ�ֻ����Ӧ�ó����ʼ��ʱ��ѯƵ�ʣ��Ϳ��Ի�������
#define Stopwatch_New(p) QueryPerformanceFrequency(&(p)->Frequency)

typedef struct
{
    LARGE_INTEGER Frequency;
    LARGE_INTEGER StartingTime;
    LARGE_INTEGER EndingTime;
    double ElapsedSeconds;
    double ElapsedMilliseconds;
    BOOL IsRunning;
} stopwatch_t;

void Stopwatch_StartNew(stopwatch_t* p);
void Stopwatch_Reset(stopwatch_t* p);
void Stopwatch_Restart(stopwatch_t* p);
void Stopwatch_Start(stopwatch_t* p);
void Stopwatch_Stop(stopwatch_t* p);

stopwatch_t* Stopwatch_Elapsed(stopwatch_t* p);
int Stopwatch_ElapsedSeconds(stopwatch_t* p);
int Stopwatch_ElapsedMilliseconds(stopwatch_t* p);

#endif