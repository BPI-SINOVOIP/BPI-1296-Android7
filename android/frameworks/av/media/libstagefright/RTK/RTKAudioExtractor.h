#ifndef RTK_AUDIO_EXTRACTOR_H
#define RTK_AUDIO_EXTRACTOR_H

#include <utils/Errors.h>
#include <media/stagefright/MediaExtractor.h>
#include "RTKAudioALAC.h"

#define RTK_WARNING(fmt, args...) { ALOGD("[RTKEXT WARN]"); ALOGD("[%s %d]:" fmt, __FUNCTION__, __LINE__, ##args); }

namespace android {

struct AMessage;
class String8;

class RTKAudioExtractor : public MediaExtractor {
public:
    RTKAudioExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();

    RTK_ALAC *mALACInfo;

protected:
    virtual ~RTKAudioExtractor();

private:
    sp<DataSource> mDataSource;
    sp<MetaData> mMeta;
    status_t mInitCheck;

    String8 mimeType;

    RTKAudioExtractor(const RTKAudioExtractor &);
    RTKAudioExtractor &operator=(const RTKAudioExtractor &);
};

bool SniffRTKAudio(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *);

}  // namespace android

#endif  // #ifndef RTK_AUDIO_EXTRACTOR_H


