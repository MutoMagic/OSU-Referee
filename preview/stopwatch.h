#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <windows.h>

// �������ܼ�������Ƶ�ʡ����ܼ�������Ƶ����ϵͳ����ʱ�ǹ̶��ģ����������д������ж���һ�µġ�
// ��ˣ�ֻ����Ӧ�ó����ʼ��ʱ��ѯƵ�ʣ��Ϳ��Ի�������
#define Stopwatch_New(t) QueryPerformanceFrequency(&t.Frequency);

typedef struct
{
    LARGE_INTEGER Frequency;
    LARGE_INTEGER StartingTime;
    LARGE_INTEGER EndingTime;
    double ElapsedSeconds;
    double ElapsedMilliseconds;
    BOOL IsRunning;
} stopwatch_t;

void Stopwatch_StartNew(stopwatch_t t);
void Stopwatch_Reset(stopwatch_t t);
void Stopwatch_Restart(stopwatch_t t);
void Stopwatch_Start(stopwatch_t t);
void Stopwatch_Stop(stopwatch_t t);

stopwatch_t Stopwatch_Elapsed(stopwatch_t t);
UINT Stopwatch_ElapsedSeconds(stopwatch_t t);
UINT Stopwatch_ElapsedMilliseconds(stopwatch_t t);

#endif