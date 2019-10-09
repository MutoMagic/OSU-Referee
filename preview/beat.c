#include "beat.h"
#include "beatmap.h"

beat_t getNextBeatMillisecondByPosition(beatmap_t *p, timing_t *nowTiming, int pos)
{
    beat_t nextBeat = { -p->AudioLeadIn,-1 };
    if (nowTiming == NULL)
    {
        nowTiming = getFirstTiming(p);
        if (nowTiming == NULL)
            return nextBeat;

        nextBeat.time = nowTiming->Offset;
        nextBeat.type = 0;
        return nextBeat;
    }

    double halfMillisecondsPerBeat = nowTiming->MillisecondsPerBeat / 2.0; // ÿ���ĳ���ʱ��
    int interval = pos - nowTiming->Offset, // ȡֵ��Χ�� [0, nextTiming.Offset - nowTiming.Offset)
        halfBeatNum = (int)(interval / halfMillisecondsPerBeat) + 1, // �ѹ������� + 1
        measure = halfBeatNum % (nowTiming->Meter * 2) + 1; // ��һ������С���е�λ�ã�1ΪС�ڿ�ͷ

    nextBeat.time = halfBeatNum * halfMillisecondsPerBeat;
    timing_t *nextTiming = getNextTiming(p, nowTiming);
    if (nextTiming != NULL && nextBeat.time >= nextTiming->Offset)
    {
        nextBeat.time = nextTiming->Offset;
        nextBeat.type = 0;
        return nextBeat;
    }

    switch (nowTiming->Meter)
    {
    case 4:
        if (measure & 1)
        {
            measure = (measure + 1) / 2;
            if (measure & 1)
                nextBeat.type = 0;
            else
                nextBeat.type = 1;
        }
        break;
    case 6:
        break;

    case 3:
        break;
    case 5:
        break;
    case 7:
        break;
    }
    return nextBeat;
}