#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

/**
* Ϊ�˽���������⣬�˺�������ԭ������������д��fuEvent��ֹʹ��TIME_CALLBACK_EVENT_SET��TIME_CALLBACK_EVENT_PULSE��
* Ϊ�˱�֤�����ԭ������ͬ���º�������fuEvent�����ֵ��������������κβ�����
*
* ����Ϊԭ�������ݣ�
*
* �õ�timeSetEvent��������ָ���Ķ�ʱ���¼�����ý���ʱ�������Լ����߳������С�
* �����¼�����������ָ���Ļص����������û�����ָ�����¼�����
*
* ÿ�ε���timeSetEvent���������Զ�ʱ���¼�����Ҫ��timeKillEvent����������Ӧ�ĵ��á�
* ʹ��TIME_KILL_SYNCHRONOUS��TIME_CALLBACK_FUNCTION��־�����¼��ɷ�ֹ�ڵ���timeKillEvent���������¼���
*
* @param uDelay         �¼��ӳ٣��Ժ���Ϊ��λ�������ֵ���ڼ�ʱ��֧�ֵ���С������¼��ӳٷ�Χ�ڣ���ú������ش���
* @param uResolution    ��ʱ���¼��Ľ���������Ժ���Ϊ��λ���ֱ�������ֵ�����Ӷ�����;
*                       �ֱ���Ϊ0��ʾӦ������׼ȷ�ط����������¼������ǣ�Ϊ�˼���ϵͳ��������Ӧ��ʹ���ʺ�����Ӧ�ó�������ֵ��
* @param lpTimeProc     ָ��һ���ص�������ָ�룬�ú����ڵ����¼�����ʱ����һ�Σ��������������¼�����ʱ���ڵ��á�
*                       ���fuEventָ��TIME_CALLBACK_EVENT_SET��TIME_CALLBACK_EVENT_PULSE��־��
*                       ��lpTimeProc������������Ϊ�¼�����ľ�����¼����ڵ����¼���ɺ����û����壬���ڶ����¼���ɺ������á�
*                       ����fuEvent���κ�����ֵ��lpTimeProc������ָ��LPTIMECALLBACK���͵Ļص�������ָ�롣
* @param dwUser         �û��ṩ�Ļص����ݡ�
* @param fuEvent        ��ʱ���¼����͡��˲������԰�������ֵ֮һ��
*                       TIME_ONESHOT                ��uDelay����֮���¼�����һ�Ρ�
*                       TIME_PERIODIC               ÿ��uDelay���뷢��һ���¼���
*                       ����fuEvent���������԰�������ֵ�е�һ����
*                       TIME_CALLBACK_FUNCTION	    ����ʱ������ʱ��Windows�����lpTimeProc����ָ��ĺ���������Ĭ��ֵ��
*                       TIME_CALLBACK_EVENT_SET	    ����ʱ������ʱ��Windows����SetEvent����������lpTimeProc����ָ����¼���
*                                                   ��dwUser���������ԡ�
*                       TIME_CALLBACK_EVENT_PULSE   ����ʱ������ʱ��Windows����PulseEvent����������lpTimeProc����ָ����¼���
*                                                   ��dwUser���������ԡ�
*                       TIME_KILL_SYNCHRONOUS	    ���ݴ˱�־�ɷ�ֹ�ڵ���timeKillEvent���������¼���
* @return               ����ɹ��򷵻ؼ�ʱ���¼��ı�ʶ�������򷵻ش������ʧ�ܲ���δ������ʱ���¼�����˺�������NULL��
*                       ���˱�ʶ��Ҳ���ݸ��ص���������
*/
MMRESULT TIMER_timeSetEvent(
    UINT            uDelay,
    UINT            uResolution,
    LPTIMECALLBACK  lpTimeProc,
    DWORD_PTR       dwUser,
    UINT            fuEvent
);

/**
* ע�⣡�����Ƿº�����TIMER_timeKillEvent������˲�֧��ȡ��ԭ������timeSetEvent�������ļ�ʱ���¼���
* ͬ��ԭ������timeKillEvent��Ҳ�޷��ͷ��ɷº�����TIMER_timeSetEvent�������ļ�ʱ���¼���
*
* ����Ϊԭ�������ݣ�
*
* ��timeKillEvent����ȡ��ָ����ʱ���¼���
*
* @param uTimerID   Ҫȡ���ļ�ʱ���¼��ı�ʶ�������ü�ʱ���¼�ʱ��timeSetEvent�������ش˱�ʶ����
* @return           ����TIMERR_NOERROR����ɹ���MMSYSERR_INVALPARAM���ָ����ʱ���¼������ڡ�
*/
MMRESULT TIMER_timeKillEvent(UINT uTimerID);

/**
* ���õȴ�������ֱ�ӻ��Ӵ������ڵĴ���ʱҪС�ġ����һ���̴߳������κδ��ڣ������봦����Ϣ��
* ��Ϣ�㲥�����͵�ϵͳ�е����д��ڡ�ʹ��û�г�ʱ����ĵȴ��������߳̿��ܻᵼ��ϵͳ������
* ��ˣ��������һ���������ڵ��̣߳���ʹ�� MsgWaitForMultipleObjects �� MsgWaitForMultipleObjectsEx��
*/
DWORD MsgWaitForSingleObject(HANDLE hHandle, DWORD  dwMilliseconds);

#endif