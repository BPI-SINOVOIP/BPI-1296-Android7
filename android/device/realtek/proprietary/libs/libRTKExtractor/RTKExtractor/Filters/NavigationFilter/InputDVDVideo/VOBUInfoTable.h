#ifndef __DVD_VIDEO_VOBU_INFO_TABLE_H__
#define __DVD_VIDEO_VOBU_INFO_TABLE_H__

#include "common.h"

template <int size> class CVOBUInfoTable
{
protected:

    int head;
    int tail;

    DVDVideoVOBUInfo info[size];

public:

    inline CVOBUInfoTable() { Reset(); }

    inline DVDVideoVOBUInfo* GetFirst()
    {
        if(head == tail)
            return NULL;
        return &info[head];
    }

    inline DVDVideoVOBUInfo* GetSecond()
    {
        if(Size() < 2)
            return NULL;
        return &info[(head>=size-1)? 0 : head+1];
    }

    inline DVDVideoVOBUInfo* GetLast()
    {
        if(head == tail)
            return NULL;
        return &info[(tail==0)? size-1 : tail-1];
    }

    inline DVDVideoVOBUInfo* GetNew()
    {
        return &info[tail];
    }

    inline void Enqueue()
    {
        tail = (tail>=size-1)? 0 : tail+1;
        if(head == tail)
            head = (head>=size-1)? 0 : head+1;
    }

    inline void Dequeue()
    {
        if(head != tail)
            head = (head>=size-1)? 0 : head+1;
    }

    inline int Size()
    {
        return (tail>=head)? tail-head : tail+size-head;
    }

    inline bool IsEmpty()
    {
        return (head == tail);
    }

    inline bool IsFull()
    {
        return (Size() >= size-1);
    }

    inline void Reset()
    {
        head = tail = 0;
    }
};

#endif //__DVD_VIDEO_VOBU_INFO_TABLE_H__
