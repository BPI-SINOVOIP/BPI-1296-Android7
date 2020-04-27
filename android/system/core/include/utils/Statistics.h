
#ifndef _LIBS_UTILS_STATISTICS_H
#define _LIBS_UTILS_STATISTICS_H

#include <utils/Timers.h>
#include <utils/String8.h>

namespace android {

class Statistics {
public:
    Statistics();
	Statistics(const char *name, int period); //period in sec
	~Statistics();
    void setup(const char *name, int period);
	void rate(uint64_t count);
	void count(uint64_t count);
    void calc(bool calc_option);
private:
    String8 mName;
	int mPeriod;
	//int mMode; //0:avg in time, 1:avg in count
	nsecs_t mStartTime;
	nsecs_t mPrevTime;
    nsecs_t mCalcPrevTime;  // for calc
	uint64_t mSumShort; //in short period
	uint64_t mSumLong; //in long period
	int mCntShort; //in short period
	int mCntLong; //in long period
};

}; // namespace android

#endif //_LIBS_UTILS_STATISTICS_H

