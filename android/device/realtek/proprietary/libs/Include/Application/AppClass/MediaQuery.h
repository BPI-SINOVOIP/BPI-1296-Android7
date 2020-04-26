#ifndef _MEDIA_QUERY_H_
#define _MEDIA_QUERY_H_
#include <utils/StrongPointer.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>

using namespace android;

#define QUERY_FILTER_NONE	0
#define QUERY_FILTER_ALBUM	1
#define QUERY_FILTER_ARTIST	2
#define QUERY_FILTER_GENRE	3

typedef enum
{
       MEDIA_TYPE_MUSIC,       // music related files
       MEDIA_TYPE_ALBUM,
       MEDIA_TYPE_ALBUM_ART,
       MEDIA_TYPE_ARTIST,
       MEDIA_TYPE_GENRE,

       MEDIA_TYPE_PHOTO,       // photo related files
       MEDIA_TYPE_VIDEO,       // video related files
       MEDIA_TYPE_NUM
} MEDIA_TYPE_E;


class MediaQuery {

private:
    sp<IBinder> mMediaQueryServiceBinder;

public:
    MediaQuery();
    ~MediaQuery();

    char* queryMusic(int nOffset, int nMax, int nTarget, int nId);
    char* queryAlbum(int nOffset, int nMax, int nTarget, int nId);
	uint8_t* queryAlbumArt(int nId, int nWidth, int nHeight, uint8_t** ppAlbumArt, int* pAlbumArtSize);
    char* queryArtist(int nOffset, int nMax);
    char* queryGenre(int nOffset, int nMax);

    char* queryPhoto(int nOffset, int nMax);
    char* queryVideo(int nOffset, int nMax);

    int play(const char *path);
	int play(MEDIA_TYPE_E type, int nId);

    char* getContentProperty(MEDIA_TYPE_E type, int nId);
    
private:
    sp<IBinder> getMediaQueryServiceBinder();
};

#endif /* _MEDIA_QUERY_H_ */
