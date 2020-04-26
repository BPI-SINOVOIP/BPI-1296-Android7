#ifndef __UTIL_SHUFFLE_H__
#define __UTIL_SHUFFLE_H__

#include <OSAL.h>
#include <stdlib.h>
#include <assert.h>

template <long NUM_OF_DWORDS>
class CShuffler
{
protected:

    uint32_t m_shuffleHistory[NUM_OF_DWORDS];
    uint32_t m_itemTotal; // total number of items to shuffle
    uint32_t m_itemCount; // remaining un-shuffled item count

public:

    CShuffler() { m_itemTotal = 0; }

    void Reset(uint32_t itemTotal) {
        assert(itemTotal > 0 && itemTotal <= NUM_OF_DWORDS*32);
        m_itemTotal = itemTotal;
        m_itemCount = itemTotal;
        memset(m_shuffleHistory, 0, sizeof(m_shuffleHistory));
    }

    uint32_t Shuffle() {
        assert(m_itemTotal > 0); // make sure Reset() is done before start using Shuffle() 

        if(m_itemCount == 0)
            Reset(m_itemTotal);

        uint32_t sel = (rand()%m_itemCount) + 1;
        uint32_t mask;
        int i, j;

        for(i=0; i<NUM_OF_DWORDS; i++) {

            mask = 1;

            for(j=0; j<32; j++) {

                if((m_shuffleHistory[i]&mask) == 0) {
                    sel--;
                    if(sel == 0) {
                        m_shuffleHistory[i] |= mask;
                        m_itemCount--;
                        return i*32 + j;
                    }
                }

                mask <<= 1;
            }
        }

        assert(false); // should never get here
        return 0;
    }
};

#endif //__UTIL_SHUFFLE_H__
