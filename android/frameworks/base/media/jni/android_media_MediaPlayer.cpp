/*
**
** Copyright 2007, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayer-JNI"
#include "utils/Log.h"

#include <media/mediaplayer.h>
#include <media/AudioResamplerPublic.h>
#include <media/IMediaHTTPService.h>
#include <media/MediaPlayerInterface.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <utils/threads.h>
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
#include "android_runtime/android_view_Surface.h"
#include "android_runtime/Log.h"
#include "utils/Errors.h"  // for status_t
#include "utils/KeyedVector.h"
#include "utils/String8.h"
#include "android_media_MediaDataSource.h"
#include "android_media_PlaybackParams.h"
#include "android_media_SyncParams.h"
#include "android_media_Utils.h"

#include "android_os_Parcel.h"
#include "android_util_Binder.h"
#include <binder/Parcel.h>
#include <gui/IGraphicBufferProducer.h>
#include <gui/Surface.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#ifdef USE_RT_MEDIA_PLAYER
#include <sys/stat.h>
#include <cutils/properties.h>
#endif // USE_RT_MEDIA_PLAYER

#include "android_util_Binder.h"
// ----------------------------------------------------------------------------

using namespace android;

// ----------------------------------------------------------------------------

static int isFolder(const char* dir)
{
    struct stat s;
    int res = stat(dir, &s);
    if (res == 0) {
        return S_ISDIR(s.st_mode);
    }
    return 0;
}

/**
 * Keep going official Android media flow.
 */
bool keepOriginalFlow(const char *url)
{
    static const char* const FILE_EXTS[] = {
        //".3gp", 3gp should be handled by RTMediaPlayer
        ".m4a",
        ".aac",
        ".flac",
        ".mp3",
        ".mid",
        ".midi",
        ".smf",
        ".xmf",
        ".mxmf",
        ".rtttl",
        ".rtx",
        ".ota",
        ".imy",
        ".ogg",
        ".dsf",
        ".wav",
        ".wma",
        ".amr",
        ".ape"
    };
    int lenURL = strlen(url);
    for (int i = 0; i < NELEM(FILE_EXTS); ++i) {
        int len = strlen(FILE_EXTS[i]);
        int start = lenURL - len;
        if (start > 0) {
            if (!strncasecmp(url + start, FILE_EXTS[i], len)) {
                return true;
            }
        }
    }
    // redirct to stagefrightPlayer in following cases
    // 1. system sound effect
    // 2. apk sound effect
    if (strncmp(url, "/system/", 8) == 0
        || strncmp(url, "/data/app/", 10) == 0) {
        return true;
    } else if (strncmp(url, "/data/data/", 11) == 0) {
        static const char* const VIDEO_FILE_EXTS[] = {
            ".ts",
            ".mp4",
            ".avi",
            ".wmv",
            ".3gp",
            ".mkv",
            ".vob",
            ".mpg",
            ".mpeg"
        };

        for (int i = 0; i < NELEM(VIDEO_FILE_EXTS); ++i) {
            int len = strlen(VIDEO_FILE_EXTS[i]);
            int start = lenURL - len;
            if (start > 0) {
                if (!strncasecmp(url + start, VIDEO_FILE_EXTS[i], len)) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

struct fields_t {
    jfieldID    context;
    jfieldID    surface_texture;

    jmethodID   post_event;

    jmethodID   proxyConfigGetHost;
    jmethodID   proxyConfigGetPort;
    jmethodID   proxyConfigGetExclusionList;
};
static fields_t fields;

static PlaybackParams::fields_t gPlaybackParamsFields;
static SyncParams::fields_t gSyncParamsFields;

static Mutex sLock;
#ifdef USE_RT_MEDIA_PLAYER
int mForceUseRTMediaPlayer = FORCE_NONE;
#endif
// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class JNIMediaPlayerListener: public MediaPlayerListener
{
public:
    JNIMediaPlayerListener(JNIEnv* env, jobject thiz, jobject weak_thiz);
    ~JNIMediaPlayerListener();
    virtual void notify(int msg, int ext1, int ext2, const Parcel *obj = NULL);
private:
    JNIMediaPlayerListener();
    jclass      mClass;     // Reference to MediaPlayer class
    jobject     mObject;    // Weak ref to MediaPlayer Java object to call on
};

JNIMediaPlayerListener::JNIMediaPlayerListener(JNIEnv* env, jobject thiz, jobject weak_thiz)
{

    // Hold onto the MediaPlayer class for use in calling the static method
    // that posts events to the application thread.
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) {
        ALOGE("Can't find android/media/MediaPlayer");
        jniThrowException(env, "java/lang/Exception", NULL);
        return;
    }
    mClass = (jclass)env->NewGlobalRef(clazz);

    // We use a weak reference so the MediaPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mObject  = env->NewGlobalRef(weak_thiz);
}

JNIMediaPlayerListener::~JNIMediaPlayerListener()
{
    // remove global references
    JNIEnv *env = AndroidRuntime::getJNIEnv();
    env->DeleteGlobalRef(mObject);
    env->DeleteGlobalRef(mClass);
}

void JNIMediaPlayerListener::notify(int msg, int ext1, int ext2, const Parcel *obj)
{
    JNIEnv *env = AndroidRuntime::getJNIEnv();
    if (obj && obj->dataSize() > 0) {
        jobject jParcel = createJavaParcelObject(env);
        if (jParcel != NULL) {
            Parcel* nativeParcel = parcelForJavaObject(env, jParcel);
            nativeParcel->setData(obj->data(), obj->dataSize());
            env->CallStaticVoidMethod(mClass, fields.post_event, mObject,
                    msg, ext1, ext2, jParcel);
            env->DeleteLocalRef(jParcel);
        }
    } else {
        env->CallStaticVoidMethod(mClass, fields.post_event, mObject,
                msg, ext1, ext2, NULL);
    }
    if (env->ExceptionCheck()) {
        ALOGW("An exception occurred while notifying an event.");
        LOGW_EX(env);
        env->ExceptionClear();
    }
}

// ----------------------------------------------------------------------------

static sp<MediaPlayer> getMediaPlayer(JNIEnv* env, jobject thiz)
{
    Mutex::Autolock l(sLock);
    MediaPlayer* const p = (MediaPlayer*)env->GetLongField(thiz, fields.context);
    return sp<MediaPlayer>(p);
}

static sp<MediaPlayer> setMediaPlayer(JNIEnv* env, jobject thiz, const sp<MediaPlayer>& player)
{
    Mutex::Autolock l(sLock);
    sp<MediaPlayer> old = (MediaPlayer*)env->GetLongField(thiz, fields.context);
    if (player.get()) {
        player->incStrong((void*)setMediaPlayer);
    }
    if (old != 0) {
        old->decStrong((void*)setMediaPlayer);
    }
    env->SetLongField(thiz, fields.context, (jlong)player.get());
    return old;
}

// If exception is NULL and opStatus is not OK, this method sends an error
// event to the client application; otherwise, if exception is not NULL and
// opStatus is not OK, this method throws the given exception to the client
// application.
static void process_media_player_call(JNIEnv *env, jobject thiz, status_t opStatus, const char* exception, const char *message)
{
    if (exception == NULL) {  // Don't throw exception. Instead, send an event.
        if (opStatus != (status_t) OK) {
            sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
            if (mp != 0) mp->notify(MEDIA_ERROR, opStatus, 0);
        }
    } else {  // Throw exception!
        if ( opStatus == (status_t) INVALID_OPERATION ) {
            jniThrowException(env, "java/lang/IllegalStateException", NULL);
        } else if ( opStatus == (status_t) BAD_VALUE ) {
            jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        } else if ( opStatus == (status_t) PERMISSION_DENIED ) {
            jniThrowException(env, "java/lang/SecurityException", NULL);
        } else if ( opStatus != (status_t) OK ) {
            if (strlen(message) > 230) {
               // if the message is too long, don't bother displaying the status code
               jniThrowException( env, exception, message);
            } else {
               char msg[256];
                // append the status code to the message
               sprintf(msg, "%s: status=0x%X", message, opStatus);
               jniThrowException( env, exception, msg);
            }
        }
    }
}

static void
android_media_MediaPlayer_setDataSourceAndHeaders(
        JNIEnv *env, jobject thiz, jobject httpServiceBinderObj, jstring path,
        jobjectArray keys, jobjectArray values) {

    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    if (path == NULL) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }

    const char *tmp = env->GetStringUTFChars(path, NULL);
    if (tmp == NULL) {  // Out of memory
        return;
    }
    ALOGV("setDataSource: path %s", tmp);

    String8 pathStr(tmp);
    env->ReleaseStringUTFChars(path, tmp);
    tmp = NULL;

    // We build a KeyedVector out of the key and val arrays
    KeyedVector<String8, String8> headersVector;
    if (!ConvertKeyValueArraysToKeyedVector(
            env, keys, values, &headersVector)) {
        return;
    }

    sp<IMediaHTTPService> httpService;
    if (httpServiceBinderObj != NULL) {
        sp<IBinder> binder = ibinderForJavaObject(env, httpServiceBinderObj);
        httpService = interface_cast<IMediaHTTPService>(binder);
    }

    if (isFolder(pathStr.string())) {
        if(mForceUseRTMediaPlayer != FORCE_ANDROID_MEDIAPLAYER) {
            status_t opStatus = mp->setDataSource(httpService, pathStr.string(), NULL);
            process_media_player_call(env, thiz, opStatus, "java/io/IOException", "setDataSource failed.");
        }
        else {
            jniThrowException(env, "java/io/IOException", "setDataSource failed. Folder is not supported.");
        }
    } else {
        status_t opStatus =
            mp->setDataSource(
                httpService,
                pathStr,
                headersVector.size() > 0? &headersVector : NULL);
        
        process_media_player_call(
                env, thiz, opStatus, "java/io/IOException",
                "setDataSource failed." );
    }

}

static void
android_media_MediaPlayer_setDataSourceFD(JNIEnv *env, jobject thiz, jobject fileDescriptor, jlong offset, jlong length)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    if (fileDescriptor == NULL) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }
    int fd = jniGetFDFromFileDescriptor(env, fileDescriptor);
    ALOGV("setDataSourceFD: fd %d", fd);
#ifdef USE_RT_MEDIA_PLAYER
    ALOGV("setDataSourceFD: mForceUseRTMediaPlayer %d", mForceUseRTMediaPlayer);
    char value[PROPERTY_VALUE_MAX];
    if (mForceUseRTMediaPlayer==FORCE_RT_MEDIAPLAYER || mForceUseRTMediaPlayer==FORCE_BY_SCORE
        || (property_get("persist.media.USE_RTMediaPlayer", value, NULL)
            && (!strcmp("1", value) || !strcasecmp("true", value)))) {
        // jason for link to RTMediaPlayer
        char fd_buff[256];
        char link_buff[1024];
        sprintf(fd_buff, "/proc/self/fd/%d", fd);
        ssize_t len = readlink(fd_buff, link_buff, sizeof(link_buff)-1);
        if (len != -1) {
            char path_buff[1024];
            link_buff[len] = '\0';

            // BUG_FIX: 41979
            int prefix_len = strlen("/data/media/");
            if(strncmp(link_buff,"/data/media/",prefix_len)==0){
                char link_buff2[512];
                memset(link_buff2,0x0,512);
                char *ptmp = &(link_buff[prefix_len]);
                snprintf(link_buff2,512,"/storage/emulated/%s",ptmp);
                ALOGE("setDataSourceFD: convert link_buff: %s -> %s",link_buff,link_buff2);
                memcpy(link_buff,link_buff2,512);
            }

            // BUG_FIX: PHOENIX-90, for nas version
            int prefix_len_nas = strlen("/android/data/media/");
            if(strncmp(link_buff,"/android/data/media/",prefix_len_nas)==0){
                char link_buff2[512];
                memset(link_buff2,0x0,512);
                char *ptmp = &(link_buff[prefix_len_nas]);
                snprintf(link_buff2,512,"/android/storage/emulated/%s",ptmp);
                ALOGE("setDataSourceFD: convert link_buff: %s -> %s",link_buff,link_buff2);
                memcpy(link_buff,link_buff2,512);
            }

            // BUG-FIX: Phoenix - 541
            int prefix_len_external_storage = strlen("/mnt/media_rw/");
            if(strncmp(link_buff,"/mnt/media_rw/",prefix_len_external_storage)==0){
                char link_buff2[512];
                memset(link_buff2,0x0,512);
                char *ptmp = &(link_buff[prefix_len_external_storage]);
                snprintf(link_buff2,512,"/storage/%s",ptmp);
                ALOGD("setDataSourceFD: convert link_buff: %s -> %s",link_buff,link_buff2);
                memcpy(link_buff,link_buff2,512);
            }

            // check file exist or not
            if (access(link_buff, F_OK)) {
                ALOGE("setDataSourceFD: access file %s failed (%s)", link_buff, strerror(errno));
                jniThrowException(env, "java/io/IOException", NULL);
                return;
            }
            ALOGV("setDataSourceFD: fd %d, fd : %s, url : %s", fd, fd_buff, link_buff);
            if (keepOriginalFlow(link_buff)) {
                process_media_player_call(env, thiz, mp->setDataSource(fd, offset, length), "java/io/IOException", "setDataSourceFD failed." );
            } else {
                sprintf(path_buff, "%s?offset=%lld&length=%lld", link_buff, (long long)offset, (long long)length);
                sp<IMediaHTTPService> httpService;
/*
                if (httpServiceBinderObj != NULL) {
                    sp<IBinder> binder = ibinderForJavaObject(env, httpServiceBinderObj);
                    httpService = interface_cast<IMediaHTTPService>(binder);
                }
*/
                status_t opStatus = mp->setDataSource(httpService, path_buff, NULL);
                process_media_player_call(env, thiz, opStatus, "java/io/IOException", "setDataSourceFD failed.");
            }
        } else {
            process_media_player_call( env, thiz, mp->setDataSource(fd, offset, length), "java/io/IOException", "setDataSourceFD failed." );
        }
    }
    else
#endif // USE_RT_MEDIA_PLAYER
    process_media_player_call( env, thiz, mp->setDataSource(fd, offset, length), "java/io/IOException", "setDataSourceFD failed." );
}

static void
android_media_MediaPlayer_setDataSourceCallback(JNIEnv *env, jobject thiz, jobject dataSource)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    if (dataSource == NULL) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }
    sp<IDataSource> callbackDataSource = new JMediaDataSource(env, dataSource);
    process_media_player_call(env, thiz, mp->setDataSource(callbackDataSource), "java/lang/RuntimeException", "setDataSourceCallback failed." );
}

static sp<IGraphicBufferProducer>
getVideoSurfaceTexture(JNIEnv* env, jobject thiz) {
    IGraphicBufferProducer * const p = (IGraphicBufferProducer*)env->GetLongField(thiz, fields.surface_texture);
    return sp<IGraphicBufferProducer>(p);
}

static void
decVideoSurfaceRef(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        return;
    }

    sp<IGraphicBufferProducer> old_st = getVideoSurfaceTexture(env, thiz);
    if (old_st != NULL) {
        old_st->decStrong((void*)decVideoSurfaceRef);
    }
}

static void
setVideoSurface(JNIEnv *env, jobject thiz, jobject jsurface, jboolean mediaPlayerMustBeAlive)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        if (mediaPlayerMustBeAlive) {
            jniThrowException(env, "java/lang/IllegalStateException", NULL);
        }
        return;
    }

    decVideoSurfaceRef(env, thiz);

    sp<IGraphicBufferProducer> new_st;
    if (jsurface) {
        sp<Surface> surface(android_view_Surface_getSurface(env, jsurface));
        if (surface != NULL) {
            new_st = surface->getIGraphicBufferProducer();
            if (new_st == NULL) {
                jniThrowException(env, "java/lang/IllegalArgumentException",
                    "The surface does not have a binding SurfaceTexture!");
                return;
            }
            new_st->incStrong((void*)decVideoSurfaceRef);
        } else {
            jniThrowException(env, "java/lang/IllegalArgumentException",
                    "The surface has been released");
            return;
        }
    }

    env->SetLongField(thiz, fields.surface_texture, (jlong)new_st.get());

    // This will fail if the media player has not been initialized yet. This
    // can be the case if setDisplay() on MediaPlayer.java has been called
    // before setDataSource(). The redundant call to setVideoSurfaceTexture()
    // in prepare/prepareAsync covers for this case.
    mp->setVideoSurfaceTexture(new_st);
}

static void
android_media_MediaPlayer_setVideoSurface(JNIEnv *env, jobject thiz, jobject jsurface)
{
    setVideoSurface(env, thiz, jsurface, true /* mediaPlayerMustBeAlive */);
}

static void
android_media_MediaPlayer_prepare(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    // Handle the case where the display surface was set before the mp was
    // initialized. We try again to make it stick.
    sp<IGraphicBufferProducer> st = getVideoSurfaceTexture(env, thiz);
    mp->setVideoSurfaceTexture(st);

    process_media_player_call( env, thiz, mp->prepare(), "java/io/IOException", "Prepare failed." );
}

static void
android_media_MediaPlayer_prepareAsync(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    // Handle the case where the display surface was set before the mp was
    // initialized. We try again to make it stick.
    sp<IGraphicBufferProducer> st = getVideoSurfaceTexture(env, thiz);
    mp->setVideoSurfaceTexture(st);

    process_media_player_call( env, thiz, mp->prepareAsync(), "java/io/IOException", "Prepare Async failed." );
}

static void
android_media_MediaPlayer_start(JNIEnv *env, jobject thiz)
{
    ALOGV("start");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
#ifdef USE_RT_MEDIA_PLAYER
    int pstate = 0;
    mp->getPlayerCurrentState(&pstate);
    if (pstate & MEDIA_PLAYER_PREPARING) {
        int i = 0;
        for (i=0; i<50; i++) {
            mp->getPlayerCurrentState(&pstate);
            if (!(pstate & MEDIA_PLAYER_PREPARING))
                break;
            usleep(100000); // 0.1 s
        }
        ALOGV("preparing : %d x 100ms, State = %d ", i, pstate);
    }
#endif // USE_RT_MEDIA_PLAYER
    process_media_player_call( env, thiz, mp->start(), NULL, NULL );
}

static void
android_media_MediaPlayer_stop(JNIEnv *env, jobject thiz)
{
    ALOGV("stop");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->stop(), NULL, NULL );
}

static void
android_media_MediaPlayer_pause(JNIEnv *env, jobject thiz)
{
    ALOGV("pause");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->pause(), NULL, NULL );
}

static jboolean
android_media_MediaPlayer_isPlaying(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return JNI_FALSE;
    }
    const jboolean is_playing = mp->isPlaying();

    ALOGV("isPlaying: %d", is_playing);
    return is_playing;
}

static void
android_media_MediaPlayer_setPlaybackParams(JNIEnv *env, jobject thiz, jobject params)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    PlaybackParams pbp;
    pbp.fillFromJobject(env, gPlaybackParamsFields, params);
    ALOGV("setPlaybackParams: %d:%f %d:%f %d:%u %d:%u",
            pbp.speedSet, pbp.audioRate.mSpeed,
            pbp.pitchSet, pbp.audioRate.mPitch,
            pbp.audioFallbackModeSet, pbp.audioRate.mFallbackMode,
            pbp.audioStretchModeSet, pbp.audioRate.mStretchMode);

    AudioPlaybackRate rate;
    status_t err = mp->getPlaybackSettings(&rate);
    if (err == OK) {
        bool updatedRate = false;
        if (pbp.speedSet) {
            rate.mSpeed = pbp.audioRate.mSpeed;
            updatedRate = true;
        }
        if (pbp.pitchSet) {
            rate.mPitch = pbp.audioRate.mPitch;
            updatedRate = true;
        }
        if (pbp.audioFallbackModeSet) {
            rate.mFallbackMode = pbp.audioRate.mFallbackMode;
            updatedRate = true;
        }
        if (pbp.audioStretchModeSet) {
            rate.mStretchMode = pbp.audioRate.mStretchMode;
            updatedRate = true;
        }
        if (updatedRate) {
            err = mp->setPlaybackSettings(rate);
        }
    }
    process_media_player_call(
            env, thiz, err,
            "java/lang/IllegalStateException", "unexpected error");
}

static jobject
android_media_MediaPlayer_getPlaybackParams(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return NULL;
    }

    PlaybackParams pbp;
    AudioPlaybackRate &audioRate = pbp.audioRate;
    process_media_player_call(
            env, thiz, mp->getPlaybackSettings(&audioRate),
            "java/lang/IllegalStateException", "unexpected error");
    ALOGV("getPlaybackSettings: %f %f %d %d",
            audioRate.mSpeed, audioRate.mPitch, audioRate.mFallbackMode, audioRate.mStretchMode);

    pbp.speedSet = true;
    pbp.pitchSet = true;
    pbp.audioFallbackModeSet = true;
    pbp.audioStretchModeSet = true;

    return pbp.asJobject(env, gPlaybackParamsFields);
}

static void
android_media_MediaPlayer_setSyncParams(JNIEnv *env, jobject thiz, jobject params)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    SyncParams scp;
    scp.fillFromJobject(env, gSyncParamsFields, params);
    ALOGV("setSyncParams: %d:%d %d:%d %d:%f %d:%f",
          scp.syncSourceSet, scp.sync.mSource,
          scp.audioAdjustModeSet, scp.sync.mAudioAdjustMode,
          scp.toleranceSet, scp.sync.mTolerance,
          scp.frameRateSet, scp.frameRate);

    AVSyncSettings avsync;
    float videoFrameRate;
    status_t err = mp->getSyncSettings(&avsync, &videoFrameRate);
    if (err == OK) {
        bool updatedSync = scp.frameRateSet;
        if (scp.syncSourceSet) {
            avsync.mSource = scp.sync.mSource;
            updatedSync = true;
        }
        if (scp.audioAdjustModeSet) {
            avsync.mAudioAdjustMode = scp.sync.mAudioAdjustMode;
            updatedSync = true;
        }
        if (scp.toleranceSet) {
            avsync.mTolerance = scp.sync.mTolerance;
            updatedSync = true;
        }
        if (updatedSync) {
            err = mp->setSyncSettings(avsync, scp.frameRateSet ? scp.frameRate : -1.f);
        }
    }
    process_media_player_call(
            env, thiz, err,
            "java/lang/IllegalStateException", "unexpected error");
}

static jobject
android_media_MediaPlayer_getSyncParams(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return NULL;
    }

    SyncParams scp;
    scp.frameRate = -1.f;
    process_media_player_call(
            env, thiz, mp->getSyncSettings(&scp.sync, &scp.frameRate),
            "java/lang/IllegalStateException", "unexpected error");

    ALOGV("getSyncSettings: %d %d %f %f",
            scp.sync.mSource, scp.sync.mAudioAdjustMode, scp.sync.mTolerance, scp.frameRate);

    // sanity check params
    if (scp.sync.mSource >= AVSYNC_SOURCE_MAX
            || scp.sync.mAudioAdjustMode >= AVSYNC_AUDIO_ADJUST_MODE_MAX
            || scp.sync.mTolerance < 0.f
            || scp.sync.mTolerance >= AVSYNC_TOLERANCE_MAX) {
        jniThrowException(env,  "java/lang/IllegalStateException", NULL);
        return NULL;
    }

    scp.syncSourceSet = true;
    scp.audioAdjustModeSet = true;
    scp.toleranceSet = true;
    scp.frameRateSet = scp.frameRate >= 0.f;

    return scp.asJobject(env, gSyncParamsFields);
}

static void
android_media_MediaPlayer_seekTo(JNIEnv *env, jobject thiz, jint msec)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    ALOGV("seekTo: %d(msec)", msec);
    process_media_player_call( env, thiz, mp->seekTo(msec), NULL, NULL );
}

static jint
android_media_MediaPlayer_getVideoWidth(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int w;
    if (0 != mp->getVideoWidth(&w)) {
        ALOGE("getVideoWidth failed");
        w = 0;
    }
    ALOGV("getVideoWidth: %d", w);
    return (jint) w;
}

static jint
android_media_MediaPlayer_getVideoHeight(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int h;
    if (0 != mp->getVideoHeight(&h)) {
        ALOGE("getVideoHeight failed");
        h = 0;
    }
    ALOGV("getVideoHeight: %d", h);
    return (jint) h;
}


static jint
android_media_MediaPlayer_getCurrentPosition(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int msec;
    process_media_player_call( env, thiz, mp->getCurrentPosition(&msec), NULL, NULL );
    ALOGV("getCurrentPosition: %d (msec)", msec);
    return (jint) msec;
}

static jint
android_media_MediaPlayer_getDuration(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int msec;
    process_media_player_call( env, thiz, mp->getDuration(&msec), NULL, NULL );
    ALOGV("getDuration: %d (msec)", msec);
    return (jint) msec;
}

static void
android_media_MediaPlayer_reset(JNIEnv *env, jobject thiz)
{
    ALOGV("reset");
#ifdef USE_RT_MEDIA_PLAYER
    mForceUseRTMediaPlayer = FORCE_NONE;
#endif
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->reset(), NULL, NULL );
}

static void
android_media_MediaPlayer_setAudioStreamType(JNIEnv *env, jobject thiz, jint streamtype)
{
    ALOGV("setAudioStreamType: %d", streamtype);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->setAudioStreamType((audio_stream_type_t) streamtype) , NULL, NULL );
}

static jint
android_media_MediaPlayer_getAudioStreamType(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    audio_stream_type_t streamtype;
    process_media_player_call( env, thiz, mp->getAudioStreamType(&streamtype), NULL, NULL );
    ALOGV("getAudioStreamType: %d (streamtype)", streamtype);
    return (jint) streamtype;
}

static jboolean
android_media_MediaPlayer_setParameter(JNIEnv *env, jobject thiz, jint key, jobject java_request)
{
    ALOGV("setParameter: key %d", key);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }

    Parcel *request = parcelForJavaObject(env, java_request);
    status_t err = mp->setParameter(key, *request);
    if (err == OK) {
        return true;
    } else {
        return false;
    }
}

static void
android_media_MediaPlayer_setLooping(JNIEnv *env, jobject thiz, jboolean looping)
{
    ALOGV("setLooping: %d", looping);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->setLooping(looping), NULL, NULL );
}

static jboolean
android_media_MediaPlayer_isLooping(JNIEnv *env, jobject thiz)
{
    ALOGV("isLooping");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return JNI_FALSE;
    }
    return mp->isLooping() ? JNI_TRUE : JNI_FALSE;
}

static void
android_media_MediaPlayer_setVolume(JNIEnv *env, jobject thiz, jfloat leftVolume, jfloat rightVolume)
{
    ALOGV("setVolume: left %f  right %f", (float) leftVolume, (float) rightVolume);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->setVolume((float) leftVolume, (float) rightVolume), NULL, NULL );
}

// Sends the request and reply parcels to the media player via the
// binder interface.
static jint
android_media_MediaPlayer_invoke(JNIEnv *env, jobject thiz,
                                 jobject java_request, jobject java_reply)
{
    sp<MediaPlayer> media_player = getMediaPlayer(env, thiz);
    if (media_player == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return UNKNOWN_ERROR;
    }

    Parcel *request = parcelForJavaObject(env, java_request);
    Parcel *reply = parcelForJavaObject(env, java_reply);

    // Don't use process_media_player_call which use the async loop to
    // report errors, instead returns the status.
    return (jint) media_player->invoke(*request, reply);
}

// Sends the new filter to the client.
static jint
android_media_MediaPlayer_setMetadataFilter(JNIEnv *env, jobject thiz, jobject request)
{
    sp<MediaPlayer> media_player = getMediaPlayer(env, thiz);
    if (media_player == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return UNKNOWN_ERROR;
    }

    Parcel *filter = parcelForJavaObject(env, request);

    if (filter == NULL ) {
        jniThrowException(env, "java/lang/RuntimeException", "Filter is null");
        return UNKNOWN_ERROR;
    }

    return (jint) media_player->setMetadataFilter(*filter);
}

static jboolean
android_media_MediaPlayer_getMetadata(JNIEnv *env, jobject thiz, jboolean update_only,
                                      jboolean apply_filter, jobject reply)
{
    sp<MediaPlayer> media_player = getMediaPlayer(env, thiz);
    if (media_player == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return JNI_FALSE;
    }

    Parcel *metadata = parcelForJavaObject(env, reply);

    if (metadata == NULL ) {
        jniThrowException(env, "java/lang/RuntimeException", "Reply parcel is null");
        return JNI_FALSE;
    }

    metadata->freeData();
    // On return metadata is positioned at the beginning of the
    // metadata. Note however that the parcel actually starts with the
    // return code so you should not rewind the parcel using
    // setDataPosition(0).
    if (media_player->getMetadata(update_only, apply_filter, metadata) == OK) {
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block in MediaPlayer, which won't run until the
// first time an instance of this class is used.
static void
android_media_MediaPlayer_native_init(JNIEnv *env)
{
    jclass clazz;

    clazz = env->FindClass("android/media/MediaPlayer");
    if (clazz == NULL) {
        return;
    }

    fields.context = env->GetFieldID(clazz, "mNativeContext", "J");
    if (fields.context == NULL) {
        return;
    }

    fields.post_event = env->GetStaticMethodID(clazz, "postEventFromNative",
                                               "(Ljava/lang/Object;IIILjava/lang/Object;)V");
    if (fields.post_event == NULL) {
        return;
    }

    fields.surface_texture = env->GetFieldID(clazz, "mNativeSurfaceTexture", "J");
    if (fields.surface_texture == NULL) {
        return;
    }

    env->DeleteLocalRef(clazz);

    clazz = env->FindClass("android/net/ProxyInfo");
    if (clazz == NULL) {
        return;
    }

    fields.proxyConfigGetHost =
        env->GetMethodID(clazz, "getHost", "()Ljava/lang/String;");

    fields.proxyConfigGetPort =
        env->GetMethodID(clazz, "getPort", "()I");

    fields.proxyConfigGetExclusionList =
        env->GetMethodID(clazz, "getExclusionListAsString", "()Ljava/lang/String;");

    env->DeleteLocalRef(clazz);

    gPlaybackParamsFields.init(env);
    gSyncParamsFields.init(env);
}

static void
android_media_MediaPlayer_native_setup(JNIEnv *env, jobject thiz, jobject weak_this)
{
    ALOGV("native_setup");
    sp<MediaPlayer> mp = new MediaPlayer();
    if (mp == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
        return;
    }

    // create new listener and give it to MediaPlayer
    sp<JNIMediaPlayerListener> listener = new JNIMediaPlayerListener(env, thiz, weak_this);
    mp->setListener(listener);

    // Stow our new C++ MediaPlayer in an opaque field in the Java object.
    setMediaPlayer(env, thiz, mp);
}

static void
android_media_MediaPlayer_release(JNIEnv *env, jobject thiz)
{
    ALOGV("release");
    decVideoSurfaceRef(env, thiz);
    sp<MediaPlayer> mp = setMediaPlayer(env, thiz, 0);
    if (mp != NULL) {
        // this prevents native callbacks after the object is released
        mp->setListener(0);
        mp->disconnect();
    }

//[ RTK begin ]
#ifdef USE_RT_MEDIA_PLAYER
    mForceUseRTMediaPlayer = FORCE_NONE;
#endif
//[ RTK end ]
}

static void
android_media_MediaPlayer_native_finalize(JNIEnv *env, jobject thiz)
{
    ALOGV("native_finalize");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp != NULL) {
        ALOGW("MediaPlayer finalized without being released");
    }
    android_media_MediaPlayer_release(env, thiz);
}

static void android_media_MediaPlayer_set_audio_session_id(JNIEnv *env,  jobject thiz,
        jint sessionId) {
    ALOGV("set_session_id(): %d", sessionId);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->setAudioSessionId((audio_session_t) sessionId), NULL,
            NULL);
}

static jint android_media_MediaPlayer_get_audio_session_id(JNIEnv *env,  jobject thiz) {
    ALOGV("get_session_id()");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }

    return (jint) mp->getAudioSessionId();
}

static void
android_media_MediaPlayer_setAuxEffectSendLevel(JNIEnv *env, jobject thiz, jfloat level)
{
    ALOGV("setAuxEffectSendLevel: level %f", level);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->setAuxEffectSendLevel(level), NULL, NULL );
}

static void android_media_MediaPlayer_attachAuxEffect(JNIEnv *env,  jobject thiz, jint effectId) {
    ALOGV("attachAuxEffect(): %d", effectId);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->attachAuxEffect(effectId), NULL, NULL );
}

static jint
android_media_MediaPlayer_pullBatteryData(
        JNIEnv *env, jobject /* thiz */, jobject java_reply)
{
    sp<IBinder> binder = defaultServiceManager()->getService(String16("media.player"));
    sp<IMediaPlayerService> service = interface_cast<IMediaPlayerService>(binder);
    if (service.get() == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "cannot get MediaPlayerService");
        return UNKNOWN_ERROR;
    }

    Parcel *reply = parcelForJavaObject(env, java_reply);

    return (jint) service->pullBatteryData(reply);
}

static jint
android_media_MediaPlayer_setRetransmitEndpoint(JNIEnv *env, jobject thiz,
                                                jstring addrString, jint port) {
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return INVALID_OPERATION;
    }

    const char *cAddrString = NULL;

    if (NULL != addrString) {
        cAddrString = env->GetStringUTFChars(addrString, NULL);
        if (cAddrString == NULL) {  // Out of memory
            return NO_MEMORY;
        }
    }
    ALOGV("setRetransmitEndpoint: %s:%d",
            cAddrString ? cAddrString : "(null)", port);

    status_t ret;
    if (cAddrString && (port > 0xFFFF)) {
        ret = BAD_VALUE;
    } else {
        ret = mp->setRetransmitEndpoint(cAddrString,
                static_cast<uint16_t>(port));
    }

    if (NULL != addrString) {
        env->ReleaseStringUTFChars(addrString, cAddrString);
    }

    if (ret == INVALID_OPERATION ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
    }

    return (jint) ret;
}

static void
android_media_MediaPlayer_setNextMediaPlayer(JNIEnv *env, jobject thiz, jobject java_player)
{
    ALOGV("setNextMediaPlayer");
    sp<MediaPlayer> thisplayer = getMediaPlayer(env, thiz);
    if (thisplayer == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", "This player not initialized");
        return;
    }
    sp<MediaPlayer> nextplayer = (java_player == NULL) ? NULL : getMediaPlayer(env, java_player);
    if (nextplayer == NULL && java_player != NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", "That player not initialized");
        return;
    }

    if (nextplayer == thisplayer) {
        jniThrowException(env, "java/lang/IllegalArgumentException", "Next player can't be self");
        return;
    }
    // tie the two players together
    process_media_player_call(
            env, thiz, thisplayer->setNextMediaPlayer(nextplayer),
            "java/lang/IllegalArgumentException",
            "setNextMediaPlayer failed." );
    ;
}

#ifdef USE_RT_MEDIA_PLAYER
static void
android_media_MediaPlayer_useRTMediaPlayer(JNIEnv *env, jobject thiz, jint forceType)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    ALOGV("[%s,%s,%d] forceType=%d", __FILE__, __func__, __LINE__ , forceType);
    mForceUseRTMediaPlayer = forceType;
    process_media_player_call(env, thiz, mp->useRTMediaPlayer(forceType), NULL, NULL);
}

static jboolean
android_media_MediaPlayer_fastforward(JNIEnv *env, jobject thiz, jint speed)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
    /* process_media_player_call( env, thiz, mp->fastforward((int)speed), NULL, NULL ); */
    status_t ret = mp->fastforward((int)speed);
    if (ret == NO_ERROR) {
        ALOGV("[%s,%s,%d] speed=%d ok\n",__FILE__,__func__,__LINE__,(int)speed);
        return true;
    }
    else {
        ALOGV("[%s,%s,%d] speed=%d not support\n",__FILE__,__func__,__LINE__,(int)speed);
        return false;
    }
}

static jboolean
android_media_MediaPlayer_fastrewind(JNIEnv *env, jobject thiz, jint speed)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
    /* process_media_player_call( env, thiz, mp->fastrewind((int)speed), NULL, NULL ); */
    status_t ret = mp->fastrewind((int)speed);
    if (ret == NO_ERROR) {
        ALOGV("[%s,%s,%d] speed=%d ok\n",__FILE__,__func__,__LINE__,(int)speed);
        return true;
    }
    else {
        ALOGV("[%s,%s,%d] speed=%d not support\n",__FILE__,__func__,__LINE__,(int)speed);
        return false;
    }
}

/**
 * @20140313
 * deprecated
 */
static jintArray
android_media_MediaPlayer_getSubtitleInfo(JNIEnv *env, jobject thiz,jint /*streamNum*/)
{
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return NULL;
    }

    return NULL;
#if 0
    jintArray jRetBuffer = env->NewIntArray(5);
    ALOGI("[%s %s] %d", __FILE__, __func__, streamNum);
    int *cbuffer=mp->getSubtitleInfo((int)streamNum);
    ALOGI("[%s,%s,%d] m_subtitleInfo 0x%x = %d,%d,%d,%d\n",__FILE__,__func__,__LINE__,(int)cbuffer,(int)cbuffer[0],(int)cbuffer[1],(int)cbuffer[2],(int)cbuffer[3]);
    jint jiBuffer[5];
    if(cbuffer)
    {
        for(int i=0;i<5;i++)
            jiBuffer[i]=(jint)cbuffer[i];
    }
    else
    {
        for(int i=0;i<5;i++)
            jiBuffer[i]=0;
    }
    env->SetIntArrayRegion(jRetBuffer,0,5,jiBuffer);
    return jRetBuffer;
#endif
}

static void
android_media_MediaPlayer_setSubtitleInfo(JNIEnv *env, jobject thiz,jint streamNum,jint enable,jint textEncoding,jint textColor,jint fontSize,jint syncTime,jint offset)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    ALOGV("[%s,%s,%d] streamNum= %d,enable= %d,textEncoding= %d,textColor= %d,fontSize= %d\n",__FILE__,__func__,__LINE__,(int)streamNum,(int)enable,(int)textEncoding,(int)textColor,(int)fontSize);
    process_media_player_call( env, thiz, mp->setSubtitleInfo((int)streamNum,(int)enable,(int)textEncoding,(int)textColor,(int)fontSize,(int)syncTime,(int)offset), NULL, NULL );
}

static void
android_media_MediaPlayer_setAutoScanExtSubtitle(JNIEnv *env, jobject thiz, jint autoScan)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    ALOGV("[%s,%s,%d] auto = %d\n",__FILE__,__func__,__LINE__,(int)autoScan);
    process_media_player_call( env, thiz, mp->setAutoScanExtSubtitle((int)autoScan), NULL, NULL );
}

static void
android_media_MediaPlayer_setExtSubtitlePath(JNIEnv *env, jobject thiz, jstring path)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    if (path == NULL) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }
    const char *tmp = env->GetStringUTFChars(path, NULL);
    if (tmp == NULL) {  // Out of memory
        return;
    }
    ALOGV("setExtSubtitlePath: path %s ", tmp);

    String8 pathStr(tmp);
    env->ReleaseStringUTFChars(path, tmp);
    tmp = NULL;

    process_media_player_call( env, thiz, mp->setExtSubtitlePath(pathStr), NULL, NULL );
}

/**
 * @20140313
 * deprecated
 */
static jintArray
android_media_MediaPlayer_getAudioTrackInfo(JNIEnv *env, jobject thiz,jint /*streamNum*/)
{
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return NULL;
    }

    return NULL;
#if 0
    jintArray jRetBuffer = env->NewIntArray(8);
    int *cbuffer=mp->getAudioTrackInfo((int)streamNum);
    ALOGI("[%s,%s,%d] AudioTrackInfo 0x%x = %d,%d,%d,%d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,(int)cbuffer,cbuffer[0],cbuffer[1],cbuffer[2],cbuffer[3],cbuffer[4],cbuffer[5],cbuffer[6],cbuffer[7]);
    jint jiBuffer[8];
    if(cbuffer)
    {
        for(int i=0;i<8;i++)
            jiBuffer[i]=(jint)cbuffer[i];
    }
    else
    {
        for(int i=0;i<8;i++)
            jiBuffer[i]=0;
    }
    env->SetIntArrayRegion(jRetBuffer,0,8,jiBuffer);
    {
        jint* array;
        array = (jint*) env->GetIntArrayElements(jRetBuffer, NULL);
        ALOGI("[%s,%s,%d] AudioTrackInfo 0x%x = %d,%d,%d,%d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,(int)array,(int)array[0],(int)array[1],(int)array[2],(int)array[3],(int)array[4],(int)array[5],(int)array[6],(int)array[7]);
        env->ReleaseIntArrayElements(jRetBuffer, array, JNI_ABORT);
    }
    return jRetBuffer;
#endif
}

static void
android_media_MediaPlayer_setAudioTrackInfo(JNIEnv *env, jobject thiz,jint streamNum)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    ALOGV("[%s,%s,%d] streamNum= %d\n",__FILE__,__func__,__LINE__,(int)streamNum);
    process_media_player_call( env, thiz, mp->setAudioTrackInfo((int)streamNum), NULL, NULL );
}

static int
android_media_MediaPlayer_getPlayerCurrentState(JNIEnv *env, jobject thiz)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int pstate;
    process_media_player_call( env, thiz, mp->getPlayerCurrentState(&pstate), NULL, NULL );
    ALOGV("getPlayerCurrentState: %d ", pstate);
    return pstate;
}

static jbyteArray
android_media_MediaPlayer_execSetGetNavProperty(JNIEnv *env, jobject thiz,jint propertyID,jbyteArray inputArray)
{
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return NULL;
    }
    int outDataSize=0;
    unsigned char *outData=0;
/* start - transfer input java array to C array */
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    jsize inDataSize = env->GetArrayLength(inputArray);
    ALOGV("[%s,%s,%d] inDataSize = %d \n",__FILE__,__func__,__LINE__,(int)inDataSize);
    jbyte *inDataBuffer = env->GetByteArrayElements(inputArray, 0);
    for(int i=0;i<inDataSize;i++)
    {
        ALOGV("[%s,%d] inDataBuffer[%d] = %d\n",__FILE__,__LINE__,i,(int)inDataBuffer[i]);
    }
    unsigned char *ucp_Input = (unsigned char *)malloc(inDataSize);
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    memcpy(ucp_Input,inDataBuffer,inDataSize);
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    env->ReleaseByteArrayElements(inputArray, inDataBuffer, 0);
    ALOGV("[%s,%s,%d] inDataSize = %d ucp_Input[0] = %x , ucp_Input[1] = %x\n",__FILE__,__func__,__LINE__,
        (int)inDataSize,*((int *)ucp_Input),*((int *)(ucp_Input+4)));
/* end - transfer input java array to C array */
    status_t opStatus=mp->execSetGetNavProperty((int)propertyID,ucp_Input,inDataSize,&outData,&outDataSize);
    ALOGV("[%s,%s,%d] outDataSize = %d\n",__FILE__,__func__,__LINE__,outDataSize);
    for(int i=0;i<outDataSize;i++)
    {
        ALOGV("[%s,%d] outData[%d] = %d\n",__FILE__,__LINE__,i,(int)outData[i]);
    }
    free((void *)ucp_Input);
    process_media_player_call(
            env, thiz, opStatus, "java/lang/Exception",
            "SetGetNavProperty failed." );
/* start - transfer out c array to java array */
    if(outDataSize == -1)
        outDataSize = SETGETNAV_SHEMEM_SIZE - inDataSize - 4;
    if(outDataSize > 0)
    {
        jbyteArray jRetBuffer = env->NewByteArray(outDataSize);
        jbyte jByteBuffer[outDataSize];
        if(outData)
        {
            for(int i=0;i<outDataSize;i++)
                jByteBuffer[i]=(jbyte)outData[i];
        }
        else
        {
            for(int i=0;i<outDataSize;i++)
                jByteBuffer[i]=0;
        }
        env->SetByteArrayRegion(jRetBuffer,0,outDataSize,jByteBuffer);
    /* end - transfer out c array to java array */
        ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
        return jRetBuffer;
    }
    ALOGV("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
    return NULL;
}

static jboolean
android_media_MediaPlayer_deliverNaviControlCmd(JNIEnv *env, jobject thiz, jint parm1, jint parm2)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
    status_t ret = mp->deliverNaviControlCmd((int)parm1, (int)parm2);
    ALOGV("[%s,%s,%d] parm1 = %d, parm2 = %d, ret = 0x%X\n",__FILE__,__func__,__LINE__,(int)parm1, (int)parm2, ret);
    if ( ret == (status_t) NO_ERROR)
    {
        ALOGV("[%s,%s,%d] deliverNaviControlCmd, OK",__FILE__,__func__,__LINE__);
        return true;
    }
    else
    {
        ALOGV("[%s,%s,%d] deliverNaviControlCmd, FAIL",__FILE__,__func__,__LINE__);
        return false;
    }
}

static void
android_media_MediaPlayer_setStartTime(JNIEnv *env, jobject thiz, jint msec)
{
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    ALOGV("setStartTime: %d(msec)", msec);
    process_media_player_call( env, thiz, mp->setStartTime(msec), NULL, NULL );
}
#endif // USE_RT_MEDIA_PLAYER

// ----------------------------------------------------------------------------

static const JNINativeMethod gMethods[] = {
    {
        "nativeSetDataSource",
        "(Landroid/os/IBinder;Ljava/lang/String;[Ljava/lang/String;"
        "[Ljava/lang/String;)V",
        (void *)android_media_MediaPlayer_setDataSourceAndHeaders
    },

    {"_setDataSource",      "(Ljava/io/FileDescriptor;JJ)V",    (void *)android_media_MediaPlayer_setDataSourceFD},
    {"_setDataSource",      "(Landroid/media/MediaDataSource;)V",(void *)android_media_MediaPlayer_setDataSourceCallback },
    {"_setVideoSurface",    "(Landroid/view/Surface;)V",        (void *)android_media_MediaPlayer_setVideoSurface},
    {"_prepare",            "()V",                              (void *)android_media_MediaPlayer_prepare},
    {"prepareAsync",        "()V",                              (void *)android_media_MediaPlayer_prepareAsync},
    {"_start",              "()V",                              (void *)android_media_MediaPlayer_start},
    {"_stop",               "()V",                              (void *)android_media_MediaPlayer_stop},
    {"getVideoWidth",       "()I",                              (void *)android_media_MediaPlayer_getVideoWidth},
    {"getVideoHeight",      "()I",                              (void *)android_media_MediaPlayer_getVideoHeight},
    {"setPlaybackParams", "(Landroid/media/PlaybackParams;)V", (void *)android_media_MediaPlayer_setPlaybackParams},
    {"getPlaybackParams", "()Landroid/media/PlaybackParams;", (void *)android_media_MediaPlayer_getPlaybackParams},
    {"setSyncParams",     "(Landroid/media/SyncParams;)V",  (void *)android_media_MediaPlayer_setSyncParams},
    {"getSyncParams",     "()Landroid/media/SyncParams;",   (void *)android_media_MediaPlayer_getSyncParams},
    {"seekTo",              "(I)V",                             (void *)android_media_MediaPlayer_seekTo},
    {"_pause",              "()V",                              (void *)android_media_MediaPlayer_pause},
    {"isPlaying",           "()Z",                              (void *)android_media_MediaPlayer_isPlaying},
    {"getCurrentPosition",  "()I",                              (void *)android_media_MediaPlayer_getCurrentPosition},
    {"getDuration",         "()I",                              (void *)android_media_MediaPlayer_getDuration},
    {"_release",            "()V",                              (void *)android_media_MediaPlayer_release},
    {"_reset",              "()V",                              (void *)android_media_MediaPlayer_reset},
    {"_setAudioStreamType", "(I)V",                             (void *)android_media_MediaPlayer_setAudioStreamType},
    {"_getAudioStreamType", "()I",                              (void *)android_media_MediaPlayer_getAudioStreamType},
    {"setParameter",        "(ILandroid/os/Parcel;)Z",          (void *)android_media_MediaPlayer_setParameter},
    {"setLooping",          "(Z)V",                             (void *)android_media_MediaPlayer_setLooping},
    {"isLooping",           "()Z",                              (void *)android_media_MediaPlayer_isLooping},
    {"_setVolume",          "(FF)V",                            (void *)android_media_MediaPlayer_setVolume},
    {"native_invoke",       "(Landroid/os/Parcel;Landroid/os/Parcel;)I",(void *)android_media_MediaPlayer_invoke},
    {"native_setMetadataFilter", "(Landroid/os/Parcel;)I",      (void *)android_media_MediaPlayer_setMetadataFilter},
    {"native_getMetadata", "(ZZLandroid/os/Parcel;)Z",          (void *)android_media_MediaPlayer_getMetadata},
    {"native_init",         "()V",                              (void *)android_media_MediaPlayer_native_init},
    {"native_setup",        "(Ljava/lang/Object;)V",            (void *)android_media_MediaPlayer_native_setup},
    {"native_finalize",     "()V",                              (void *)android_media_MediaPlayer_native_finalize},
    {"getAudioSessionId",   "()I",                              (void *)android_media_MediaPlayer_get_audio_session_id},
    {"setAudioSessionId",   "(I)V",                             (void *)android_media_MediaPlayer_set_audio_session_id},
    {"_setAuxEffectSendLevel", "(F)V",                          (void *)android_media_MediaPlayer_setAuxEffectSendLevel},
    {"attachAuxEffect",     "(I)V",                             (void *)android_media_MediaPlayer_attachAuxEffect},
    {"native_pullBatteryData", "(Landroid/os/Parcel;)I",        (void *)android_media_MediaPlayer_pullBatteryData},
    {"native_setRetransmitEndpoint", "(Ljava/lang/String;I)I",  (void *)android_media_MediaPlayer_setRetransmitEndpoint},
    {"setNextMediaPlayer",  "(Landroid/media/MediaPlayer;)V",   (void *)android_media_MediaPlayer_setNextMediaPlayer},
#ifdef USE_RT_MEDIA_PLAYER
    {"useRTMediaPlayer",    "(I)V",                              (void *)android_media_MediaPlayer_useRTMediaPlayer},
    {"fastforward",         "(I)Z",                             (void *)android_media_MediaPlayer_fastforward},
    {"fastrewind",          "(I)Z",                             (void *)android_media_MediaPlayer_fastrewind},
    {"getSubtitleInfo",     "(I)[I",                            (void *)android_media_MediaPlayer_getSubtitleInfo},
    {"setSubtitleInfo",     "(IIIIIII)V",                         (void *)android_media_MediaPlayer_setSubtitleInfo},
    {"setAutoScanExtSubtitle",  "(I)V",                         (void *)android_media_MediaPlayer_setAutoScanExtSubtitle},
    {"setExtSubtitlePath",  "(Ljava/lang/String;)V",            (void *)android_media_MediaPlayer_setExtSubtitlePath},
    {"getAudioTrackInfo",   "(I)[I",                            (void *)android_media_MediaPlayer_getAudioTrackInfo},
    {"setAudioTrackInfo",   "(I)V",                             (void *)android_media_MediaPlayer_setAudioTrackInfo},
    {"getPlayerCurrentState", "()I",                            (void *)android_media_MediaPlayer_getPlayerCurrentState},
    {"execSetGetNavProperty", "(I[B)[B",                        (void *)android_media_MediaPlayer_execSetGetNavProperty},
    {"deliverNaviControlCmd","(II)Z",                           (void *)android_media_MediaPlayer_deliverNaviControlCmd},
    {"setStartTime",         "(I)V",                            (void *)android_media_MediaPlayer_setStartTime},
#endif // USE_RT_MEDIA_PLAYER
};

// This function only registers the native methods
static int register_android_media_MediaPlayer(JNIEnv *env)
{
    return AndroidRuntime::registerNativeMethods(env,
                "android/media/MediaPlayer", gMethods, NELEM(gMethods));
}
extern int register_android_media_ExifInterface(JNIEnv *env);
extern int register_android_media_ImageReader(JNIEnv *env);
extern int register_android_media_ImageWriter(JNIEnv *env);
extern int register_android_media_Crypto(JNIEnv *env);
extern int register_android_media_Drm(JNIEnv *env);
extern int register_android_media_MediaCodec(JNIEnv *env);
extern int register_android_media_MediaExtractor(JNIEnv *env);
extern int register_android_media_MediaCodecList(JNIEnv *env);
extern int register_android_media_MediaHTTPConnection(JNIEnv *env);
extern int register_android_media_MediaMetadataRetriever(JNIEnv *env);
extern int register_android_media_MediaMuxer(JNIEnv *env);
extern int register_android_media_MediaRecorder(JNIEnv *env);
extern int register_android_media_MediaScanner(JNIEnv *env);
extern int register_android_media_MediaSync(JNIEnv *env);
extern int register_android_media_ResampleInputStream(JNIEnv *env);
extern int register_android_media_MediaProfiles(JNIEnv *env);
extern int register_android_media_AmrInputStream(JNIEnv *env);
extern int register_android_mtp_MtpDatabase(JNIEnv *env);
extern int register_android_mtp_MtpDevice(JNIEnv *env);
extern int register_android_mtp_MtpServer(JNIEnv *env);
extern int register_android_media_MediaColorConverter(JNIEnv *env);


jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed\n");
        goto bail;
    }
    assert(env != NULL);

    if (register_android_media_ImageWriter(env) != JNI_OK) {
        ALOGE("ERROR: ImageWriter native registration failed");
        goto bail;
    }

    if (register_android_media_ImageReader(env) < 0) {
        ALOGE("ERROR: ImageReader native registration failed");
        goto bail;
    }

    if (register_android_media_MediaPlayer(env) < 0) {
        ALOGE("ERROR: MediaPlayer native registration failed\n");
        goto bail;
    }

    if (register_android_media_MediaRecorder(env) < 0) {
        ALOGE("ERROR: MediaRecorder native registration failed\n");
        goto bail;
    }

    if (register_android_media_MediaScanner(env) < 0) {
        ALOGE("ERROR: MediaScanner native registration failed\n");
        goto bail;
    }

    if (register_android_media_MediaMetadataRetriever(env) < 0) {
        ALOGE("ERROR: MediaMetadataRetriever native registration failed\n");
        goto bail;
    }

    if (register_android_media_AmrInputStream(env) < 0) {
        ALOGE("ERROR: AmrInputStream native registration failed\n");
        goto bail;
    }

    if (register_android_media_ResampleInputStream(env) < 0) {
        ALOGE("ERROR: ResampleInputStream native registration failed\n");
        goto bail;
    }

    if (register_android_media_MediaProfiles(env) < 0) {
        ALOGE("ERROR: MediaProfiles native registration failed");
        goto bail;
    }

    if (register_android_mtp_MtpDatabase(env) < 0) {
        ALOGE("ERROR: MtpDatabase native registration failed");
        goto bail;
    }

    if (register_android_mtp_MtpDevice(env) < 0) {
        ALOGE("ERROR: MtpDevice native registration failed");
        goto bail;
    }

    if (register_android_mtp_MtpServer(env) < 0) {
        ALOGE("ERROR: MtpServer native registration failed");
        goto bail;
    }

    if (register_android_media_MediaCodec(env) < 0) {
        ALOGE("ERROR: MediaCodec native registration failed");
        goto bail;
    }

    if (register_android_media_MediaSync(env) < 0) {
        ALOGE("ERROR: MediaSync native registration failed");
        goto bail;
    }

    if (register_android_media_MediaExtractor(env) < 0) {
        ALOGE("ERROR: MediaCodec native registration failed");
        goto bail;
    }

    if (register_android_media_MediaMuxer(env) < 0) {
        ALOGE("ERROR: MediaMuxer native registration failed");
        goto bail;
    }

    if (register_android_media_MediaCodecList(env) < 0) {
        ALOGE("ERROR: MediaCodec native registration failed");
        goto bail;
    }

    if (register_android_media_Crypto(env) < 0) {
        ALOGE("ERROR: MediaCodec native registration failed");
        goto bail;
    }

    if (register_android_media_Drm(env) < 0) {
        ALOGE("ERROR: MediaDrm native registration failed");
        goto bail;
    }

    if (register_android_media_MediaHTTPConnection(env) < 0) {
        ALOGE("ERROR: MediaHTTPConnection native registration failed");
        goto bail;
    }

    if (register_android_media_ExifInterface(env) < 0) {
        ALOGE("ERROR: ExifInterface native registration failed");
        goto bail;
    }

    if (register_android_media_MediaColorConverter(env) < 0) {
        ALOGE("ERROR: ColorConverter native registration failed");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}

// KTHXBYE
