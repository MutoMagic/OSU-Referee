#include <windows.h>

#include "stopwatch.h"

void Stopwatch_StartNew(stopwatch_t t)
{
    // �������ܼ�������Ƶ�ʡ����ܼ�������Ƶ����ϵͳ����ʱ�ǹ̶��ģ����������д������ж���һ�µġ�
    // ��ˣ�ֻ����Ӧ�ó����ʼ��ʱ��ѯƵ�ʣ��Ϳ��Ի�������
    QueryPerformanceFrequency(&t.Frequency);
}

void Stopwatch_Reset(stopwatch_t t)
{
    t.StartingTime.QuadPart = 0;
    t.EndingTime.QuadPart = 0;
    t.ElapsedSeconds = 0;
    t.ElapsedMilliseconds = 0;
    t.IsRunning = FALSE;
}

void Stopwatch_Restart(stopwatch_t t)
{
    Stopwatch_Reset(t);
    Stopwatch_Start(t);
}

void Stopwatch_Start(stopwatch_t t)
{
    QueryPerformanceCounter(&t.StartingTime);
    t.IsRunning = TRUE;
}

void Stopwatch_Stop(stopwatch_t t)
{
    QueryPerformanceCounter(&t.EndingTime);
    t.ElapsedSeconds = (t.EndingTime.QuadPart - t.StartingTime.QuadPart) * 1.0 / t.Frequency.QuadPart;
    t.ElapsedMilliseconds = t.ElapsedSeconds * 1000;
    t.IsRunning = FALSE;
}