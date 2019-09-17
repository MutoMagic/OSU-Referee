#include <windows.h>
#include <stdlib.h>

#include "timer.h"
#include "stopwatch.h"

// ���֧��1ms�ֱ��ʣ����򽫳�����Χ
#define getResolution(timer) (timer->uResolution ? timer->uResolution : 1)

typedef struct
{
    UINT            uDelay;
    UINT            uResolution;
    LPTIMECALLBACK  lpTimeProc;
    DWORD_PTR       dwUser;
    UINT            fuEvent;

    DWORD           lpThreadId; // �̱߳�ʶ��
    HANDLE          hThread;    // �߳̾��
    BOOL            __close;
    BOOL            __error;

    BOOL            killSync;
    HANDLE          event;
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
    UINT next = 0, delay;
    while (TRUE)
    {
        next += timer->uDelay;
        while ((UINT)Stopwatch_ElapsedMilliseconds(&s) < next)
        {
            if (!timer->uResolution)
                continue; // �����ľ�׼����ߣ�������׼ȷ�ط����������¼�

            // Sleep(1) ���� 1ms �� 2ms ����״̬��������Ҳ���ǿ��ܻ���� +1ms ����������Ҫ���������
            // ���������� n ����ʱ��ʹ�� Sleep(n-1)����ͨ�� Stopwatch ��ʱ��ʣ��ȴ�ʱ���� Sleep(0) �����������䡣
            // Sleep(0) �� CPU �߸�������·ǳ����ȶ���ʵ����ܻ������ߴ� 6ms ʱ�䣬���Կ��ܻ�����������
            // �������������ͨ��������ʽʵ�֡�
            if ((delay = next - (UINT)s.ElapsedMilliseconds) > 1)
                Sleep(delay - 1);
        }

        timer->lpTimeProc(timer, 0, timer->dwUser, 0, 0); // ��������

        if (timer->__close)
            break; // ��ֹ�߳����е���ѷ������������̺߳�������
    }

    timeEndPeriod(getResolution(timer));
    WaitForSingleObject(timer->event, INFINITE);
    CloseHandle(timer->event);

    // ͳһ�ͷ���Դ
    CloseHandle(timer->hThread); // �ر��߳̾��������ֹ�������̻߳�ɾ���̶߳���
    free(timer);
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
        goto end;

    timer->uDelay = uDelay;
    timer->uResolution = uResolution;
    timer->lpTimeProc = lpTimeProc;
    timer->dwUser = dwUser;
    timer->fuEvent = fuEvent;

    timer->hThread = CreateThread(NULL, 0, lpStartAddress, timer, CREATE_SUSPENDED, &timer->lpThreadId);

    if (timer->hThread == NULL)
        goto clear;

    timer->__close = (timer->fuEvent & TIME_PERIODIC) != TIME_PERIODIC; // TIME_ONESHOT
    timer->__error = FALSE;
    timer->killSync = (timer->fuEvent & TIME_KILL_SYNCHRONOUS) == TIME_KILL_SYNCHRONOUS;
    timer->event = CreateEvent(NULL, FALSE, !timer->killSync, NULL);

    if (timer->event == NULL)
        goto err;

    // ��һ�����̵����ȼ�����Ϊ Realtime ��֪ͨ����ϵͳ�����Ǿ���ϣ���ý��̽� CPU ʱ����ø��������̡�
    // �����ĳ�������һ������ѭ�����ᷢ�������ǲ���ϵͳҲ����ס�ˣ���ֻ��ȥ����Դ��ť��o(>_<)o
    // ����������һԭ��High ͨ����ʵʱ��������ѡ��
    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)
        || !SetThreadPriority(timer->hThread, THREAD_PRIORITY_HIGHEST)
        || timeBeginPeriod(getResolution(timer)) == TIMERR_NOCANDO)
    {
        CloseHandle(timer->event);
        goto err; // ���ڸ߾�����˵�κ�һ��ʧ�ܶ���������
    }

    ResumeThread(timer->hThread);
    return timer;

err:
    timer->__error = TRUE;
    ResumeThread(timer->hThread);
    MWFMO(timer->hThread, INFINITE); // �ȴ��߳̽���
    CloseHandle(timer->hThread);

clear:
    free(timer);

end:
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

    timer->__close = TRUE; // TerminateThread
    if (timer->killSync)
    {
        HANDLE hThread, hProcess = GetCurrentProcess();
        DuplicateHandle(hProcess, timer->hThread, hProcess, &hThread, 0, FALSE, DUPLICATE_SAME_ACCESS);

        SetEvent(timer->event);
        //MWFMO(timer->hThread, INFINITE);
        MWFMO(hThread, INFINITE); // timer->hThread �����ѱ��ͷţ�ʹ��ǰ��Ҫ����һ��
        CloseHandle(hThread);
    }
    return TIMERR_NOERROR;
}