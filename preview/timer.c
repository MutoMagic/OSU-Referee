#include <windows.h>
#include <stdlib.h>

#include "timer.h"
#include "stopwatch.h"

typedef struct
{
    UINT            uDelay;
    UINT            uResolution;
    LPTIMECALLBACK  lpTimeProc;
    DWORD_PTR       dwUser;
    UINT            fuEvent;

    DWORD           lpThreadId; // �̱߳�ʶ��
    HANDLE          hThread;    // �̵߳ľ��
} timer_t;

// ThreadProc
DWORD lpStartAddress(LPVOID lpParameter)
{
    timer_t* timer = (timer_t*)lpParameter;

    stopwatch_t s;
    Stopwatch_StartNew(s);

    // �̶�ʱ����
    UINT next = 0;
    do {
        next += timer->uDelay;
        while (Stopwatch_ElapsedMilliseconds(s) < next)
            ; // ����

        timer->lpTimeProc(timer->lpThreadId, 0, timer->dwUser, 0, 0); // ��������

        if (timer->fuEvent & TIME_PERIODIC == TIME_PERIODIC)
            continue;
    } while (FALSE); // Ĭ��TIME_ONESHOT

    return 0;
}

//-------------------------------------------------------------------------------

MMRESULT TIMER_timeSetEvent(
    UINT            uDelay,
    UINT            uResolution,
    LPTIMECALLBACK  lpTimeProc,
    DWORD_PTR       dwUser,
    UINT            fuEvent
)
{
    timer_t* lpParameter = (timer_t*)malloc(sizeof(timer_t));

    lpParameter->uDelay = uDelay;
    lpParameter->uResolution = uResolution;
    lpParameter->lpTimeProc = lpTimeProc;
    lpParameter->dwUser = dwUser;
    lpParameter->fuEvent = fuEvent;

    lpParameter->hThread = CreateThread(NULL, 0, lpStartAddress, &lpParameter, 0, lpParameter->lpThreadId);
    return lpParameter;
}

MMRESULT TIMER_timeKillEvent(UINT uTimerID)
{
    timer_t* timer = (timer_t*)uTimerID;

    if (!CloseHandle(timer->hThread))
        return MMSYSERR_INVALPARAM;

    return TIMERR_NOERROR;
}