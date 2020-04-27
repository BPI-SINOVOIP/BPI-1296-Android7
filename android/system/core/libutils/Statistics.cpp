
//#define LOG_NDEBUG 0
#define LOG_TAG "Statistics"

#include <string.h>
#include <utils/Log.h>
#include <inttypes.h>
#include <utils/Statistics.h>

namespace android {

Statistics::Statistics():
    mStartTime(systemTime()), mPrevTime(systemTime()), mSumShort(0), mSumLong(0), mCntShort(0), mCntLong(0)
{
    //ALOGD("new Statistics()");
}

Statistics::Statistics(const char *name, int period):
	mName(name), mPeriod(period), mStartTime(systemTime()), mPrevTime(systemTime()), mSumShort(0), mSumLong(0), mCntShort(0), mCntLong(0)
{
	ALOGD("name:%s period:%d", mName.string(), mPeriod);
}

Statistics::~Statistics()
{
}

void Statistics::setup(const char *name, int period)
{
    mName.setTo(name);
    mPeriod = period;
    ALOGD("name:%s period:%d", mName.string(), mPeriod);
}

void Statistics::rate(uint64_t count)
{
	nsecs_t tcurr = systemTime();
	ALOGV("tcurr:%" PRId64 " mPrevTime:%" PRId64 " mPeriod:%d count:%" PRIu64, tcurr, mPrevTime, mPeriod, count);
	//mMode = 0;
	mSumShort += count;
	mCntShort++;
	if((tcurr - mPrevTime) >= (int64_t)mPeriod * 1000000000){
		mSumLong += mSumShort;
		mCntLong += mCntShort;
		ALOGD("%s: rate:%.3f avg:%.3f", mName.string(), mSumShort * 1000000000.0 / (tcurr - mPrevTime), mSumLong * 1000000000.0 / (tcurr - mStartTime));
		mSumShort = 0;
		mCntShort = 0;
		mPrevTime = tcurr;
	}
}

void Statistics::count(uint64_t count)
{
	nsecs_t tcurr = systemTime();
	ALOGV("tcurr:%" PRId64 " mPrevTime:%" PRId64 " mPeriod:%d count:%" PRIu64, tcurr, mPrevTime, mPeriod, count);
	//mMode = 1;
	mSumShort += count;
	mCntShort++;
	if((tcurr - mPrevTime) >= (int64_t)mPeriod * 1000000000){
		mSumLong += mSumShort;
		mCntLong += mCntShort;
		ALOGD("%s: avg1:%.3f avg2:%.3f", mName.string(), (double)mSumShort / mCntShort, (double)mSumLong / mCntLong);
		mSumShort = 0;
		mCntShort = 0;
		mPrevTime = tcurr;
	}
}

void Statistics::calc(bool calc_option)
{
    nsecs_t tcurr = systemTime();
    if (calc_option)
        mCalcPrevTime = tcurr;
    else
    {
        mSumShort += (tcurr - mCalcPrevTime);
        mCntShort++;
        if((tcurr - mPrevTime) >= (int64_t)mPeriod * 1000000000){
            mSumLong += mSumShort;
            mCntLong += mCntShort;
            ALOGD("%s: avg1:%.3f avg2:%.3f", mName.string(), (tcurr - mCalcPrevTime) / 1000000.0, mSumLong / mCntLong / 1000000.0);
            mSumShort = 0;
            mCntShort = 0;
            mPrevTime = tcurr;
        }
    }
}


};  // namespace android

