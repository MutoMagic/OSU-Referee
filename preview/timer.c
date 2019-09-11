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
    BOOL            __close;
    HANDLE          __cLock;
    BOOL            __error;
} timer_t;

// ThreadProc
DWORD WINAPI lpStartAddress(LPVOID lpParameter)
{
    timer_t* timer = (timer_t*)lpParameter;

    if (timer->__error)
        return 1; // ExitThread

    stopwatch_t s;
    Stopwatch_StartNew(&s);

    // �̶�ʱ����
    UINT next = 0;
    while (TRUE)
    {
        next += timer->uDelay;
        while ((UINT)Stopwatch_ElapsedMilliseconds(&s) < next)
            Sleep(next - (UINT)s.ElapsedMilliseconds); // ���� + ����

        WaitForSingleObject(timer->__cLock, 0);
        if (timer->lpTimeProc != NULL)
            timer->lpTimeProc(timer->lpThreadId, 0, timer->dwUser, 0, 0); // ��������
        ReleaseMutex(timer->__cLock);

        if (timer->__close)
            break; // ��ֹ�߳����е���ѷ������������̺߳�������
    }

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
    timer_t* timer = (timer_t*)malloc(sizeof(timer_t));

    if (timer == NULL)
        return NULL;

    timer->uDelay = uDelay;
    timer->uResolution = uResolution == 0 ? 1 : timer->uResolution; // ���֧��1ms�ֱ��ʣ����򽫳�����Χ
    timer->lpTimeProc = lpTimeProc;
    timer->dwUser = dwUser;
    timer->fuEvent = fuEvent;

    timer->hThread = CreateThread(NULL, 0, lpStartAddress, timer, CREATE_SUSPENDED, &timer->lpThreadId);

    if (timer->hThread == NULL)
        goto err;

    timer->__close = (timer->fuEvent & TIME_PERIODIC) != TIME_PERIODIC; // TIME_ONESHOT

    // ��һ�����̵����ȼ�����Ϊ Realtime ��֪ͨ����ϵͳ�����Ǿ���ϣ���ý��̽� CPU ʱ����ø��������̡�
    // �����ĳ�������һ������ѭ�����ᷢ�������ǲ���ϵͳҲ����ס�ˣ���ֻ��ȥ����Դ��ť��o(>_<)o
    // ����������һԭ��High ͨ����ʵʱ��������ѡ��
    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)
        || !SetThreadPriority(timer->hThread, THREAD_PRIORITY_HIGHEST)
        || timeBeginPeriod(timer->uResolution) == TIMERR_NOCANDO)
    {
        timer->__error = TRUE;
        ResumeThread(timer->hThread);

        MWFMO(timer->hThread, INFINITE);
        CloseHandle(timer->hThread);
        goto err; // ���ڸ߾�����˵�κ�һ��ʧ�ܶ��������ġ�
    }

    timer->__cLock = CreateMutex(NULL, FALSE, NULL);
    timer->__error = FALSE;

    ResumeThread(timer->hThread);
    return timer;

err:
    free(timer);
    return NULL;
}

MMRESULT TIMER_timeKillEvent(UINT uTimerID)
{
    timer_t* timer = (timer_t*)uTimerID;

    // ȷ�����ý����Ƿ���ж�ָ����ַ���ڴ�Ķ�����Ȩ
    if (timer == NULL || IsBadCodePtr(timer))
        return MMSYSERR_INVALPARAM;

    // �ж��߳̾���Ƿ���Ч
    DWORD lpThreadId = GetThreadId(timer->hThread);
    if (!lpThreadId || lpThreadId != timer->lpThreadId)
        return MMSYSERR_INVALPARAM;

    MWFMO(timer->__cLock, INFINITE);
    timer->__close = TRUE; // TerminateThread
    if ((timer->fuEvent & TIME_KILL_SYNCHRONOUS) == TIME_KILL_SYNCHRONOUS)
        timer->lpTimeProc = NULL;
    ReleaseMutex(timer->__cLock);

    MWFMO(timer->hThread, INFINITE); // Wait Functions
    timeEndPeriod(timer->uResolution);
    CH(timer->__cLock);
    CloseHandle(timer->hThread); // �ر��߳̾��������ֹ�������̻߳�ɾ���̶߳���
    free(timer);
    return TIMERR_NOERROR;
}