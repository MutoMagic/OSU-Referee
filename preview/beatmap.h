#ifndef BEATMAP_H
#define BEATMAP_H

#include <windows.h>
#include <limits.h>

#define NSIZE_MAX 32767 // win32 GetPrivateProfileSection lpReturnedString ���������ַ���
#define INT_STR_LMAX 11 // int32 ���λ����int string length max��������λ
#define OSU_COLUMN_MAX UCHAR_MAX
#define OSU_TIMING_MAX NSIZE_MAX

// beatmap,timing ��Ϊָ��
#define getTimingIndex(beatmap, timing) (timing - &beatmap->TimingPoints[0]) / sizeof(timing_t)

#define isEffectEnabled(k, e) ((k & e) != 0)

typedef enum
{
    Effect_None = 0,
    Effect_Kiai = 1,
    OmitFirstBarLine = 8,
} Effect;

typedef struct
{
    int Offset;
    double MillisecondsPerBeat;
    int Meter;
    int SampleSet;
    int SampleIndex;
    int Volume;
    BOOL Inherited;
    int KiaiMode;
} timing_t;

typedef struct
{
    // General
    char AudioFilename[MAX_PATH]; // (String) ��Ƶ�ļ�����ڵ�ǰ�ļ��е�λ�á�
    int AudioLeadIn; // (Integer, ����) �ӳٲ��š�����������ʼ����Ƶ�ļ������á�
    int PreviewTime; // (Integer, ����) ��ѡ��˵�����Ƶ�ļ�����ʼ���ţ�Ԥ����λ�á�
    int Countdown; // (Integer) "Are you ready? 3, 2, 1, GO!"����ʱ��(0=No countdown, 1=Normal, 2=Half, 3=Double)
    char SampleSet[OSU_COLUMN_MAX]; // (String) ��Ч�顣
    float StackLeniency; //(Decimal) hit objects�ѵ�Ƶ�ʣ�����0ʱ������ʵ��Ĵ�λ��ʾ������playʱ��Ч��
    int Mode; // (Integer) ��Ϸģʽ��(0=osu!, 1=Taiko, 2=Catch the Beat, 3=osu!mania)
    BOOL LetterboxInBreaks; // (Boolean) ��break�ڼ��Ƿ�������º�����
    BOOL StoryFireInFront; // (Boolean) �Ƿ���combo fire���ù����ѱ�������ǰ��ʾstoryboard��
    char SkinPreference[OSU_COLUMN_MAX]; // (String) ��ѡ��ۡ�
    BOOL EpilepsyWarning; // (Boolean) �Ƿ��ڿ�ͷ��ʾ"This beatmap contains scenes with rapidly flashing colours..."���档
    int CountdownOffset; // (Integer) ָ������ʱ��ʼǰ�Ľ�������
    BOOL WidescreenStoryboard; // (Boolean) ָ��storyboard�Ƿ�ӦΪ������
    BOOL SpecialStyle; // (Boolean) �Ƿ�ʹ��osu!mania��N+1������ʽ��
    BOOL UseSkinSprites; // (Boolean) ָ��storyboard�Ƿ����ʹ���û���Ƥ����Դ��

    // Difficulty
    double SliderTickRate;

    // TimingPoints
    timing_t TimingPoints[OSU_TIMING_MAX];

} beatmap_t;

/**
* ��ȡ��ǰλ��������Timing������2��ʾ�ú���Ϊ���������İ汾��
*
* @param beatmap_t* beatmapָ��
* @param pos ��ǰ����λ�ã���λ������
* @return pos������Timing��ΪNULLʱ��ʾ��ǰλ�ò��ں�����
*/
timing_t *getTimingFromPosition2(beatmap_t *p, int pos);
timing_t *getTimingFromPosition(beatmap_t *p, timing_t *prev, int pos);
timing_t *getNextTiming(beatmap_t *p, timing_t *prev);
timing_t *getFirstTiming(beatmap_t *p);
void loadBeatmap(beatmap_t *p, char *file);

int getLineFromSection(int index, char *returnedString, char *section);
int getStringFromSection(char *key, char *defaultString, char *returnedString, char *section);
int getIntFromSection(char *key, int defaultInt, char *section);

int getStringFromLine(int index, char *defaultString, char *returnedString, char *line);
int getIntFromLine(int index, int defaultInt, char *line);

#endif