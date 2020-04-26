/*
 * Copyright 2012, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WIFI_DISPLAY_SOURCE_H_

#define WIFI_DISPLAY_SOURCE_H_

#include <media/stagefright/foundation/ANetworkSession.h>

#include <media/stagefright/foundation/AHandler.h>
#include <utils/Thread.h>

#include <netinet/in.h>

namespace android {

#define USE_1080P       0
#define USED_KEYEDVECTOR

struct IRemoteDisplayClient;
struct ParsedMessage;

// Represents the RTSP server acting as a wifi display source.
// Manages incoming connections, sets up Playback sessions as necessary.
struct RtkRtspServer : public AHandler {
    static const unsigned kWifiDisplayDefaultPort = 7236;

    RtkRtspServer(const sp<ANetworkSession> &netSession);

    status_t start(const char *iface, int port);
    status_t stop();

    status_t pause();
    status_t resume();

protected:
    virtual ~RtkRtspServer();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    struct ClientPushThread;

    enum State {
        INITIALIZED,
        AWAITING_CLIENT_CONNECTION,
        AWAITING_CLIENT_SETUP,
        AWAITING_CLIENT_PLAY,
        ABOUT_TO_PLAY,
        PLAYING,
        PLAYING_TO_PAUSED,
        PAUSED,
        PAUSED_TO_PLAYING,
        AWAITING_CLIENT_TEARDOWN,
        STOPPING,
        STOPPED,
    };

    enum {
        kWhatStart,
        kWhatRTSPNotify,
        kWhatStop,
        kWhatPause,
        kWhatResume,
        kWhatReapDeadClients,
        kWhatPlaybackSessionNotify,
        kWhatKeepAlive,
        kWhatFinishStop2,
        kWhatTeardownTriggerTimedOut,
    };

#if 0
    struct ResponseID {
        int32_t mSessionID;
        int32_t mCSeq;

        bool operator<(const ResponseID &other) const {
            return mSessionID < other.mSessionID
                || (mSessionID == other.mSessionID
                        && mCSeq < other.mCSeq);
        }
    };
#endif

    typedef status_t (RtkRtspServer::*HandleRTSPResponseFunc)(
            int32_t connectID, const sp<ParsedMessage> &msg);

    static const int64_t kReaperIntervalUs = 1000000ll;

    // We request that the dongle send us a "TEARDOWN" in order to
    // perform an orderly shutdown. We're willing to wait up to 2 secs
    // for this message to arrive, after that we'll force a disconnect
    // instead.
    static const int64_t kTeardownTriggerTimeouSecs = 2;

    static const int64_t kPlaybackSessionTimeoutSecs = 30;

    static const int64_t kPlaybackSessionTimeoutUs =
        kPlaybackSessionTimeoutSecs * 1000000ll;

    State mState;
    sp<ANetworkSession> mNetSession;
    struct in_addr mInterfaceAddr;
    int32_t mSessionID;

    sp<AReplyToken> mStopReplyID;

    int32_t mChosenRTPPort;  
	AString mBroadcastIP;

    int32_t mClientSessionID;

    struct ClientInfo {
        AString mRemoteIP;
        AString mLocalIP;
        int32_t mLocalPort;
        int32_t mPlaybackSessionID;
//        sp<PlaybackSession> mPlaybackSession;
    };
    ClientInfo mClientInfo;

	KeyedVector<uint32_t, sp<ClientPushThread> >	mMultiClient;

    enum TriggerType {
        TRIGGER_SETUP,
        TRIGGER_TEARDOWN,
        TRIGGER_PAUSE,
        TRIGGER_PLAY,
    };

    // M5
    status_t sendTrigger(sp<ClientPushThread> &ClientInfo, TriggerType triggerType);

    status_t sendKeepAlive(sp<ClientPushThread> &ClientInfo);

    void registerResponseHandler(
            int32_t connectID, int32_t cseq, HandleRTSPResponseFunc func);

    status_t onReceiveClientData(const sp<AMessage> &msg);

    status_t onOptionsRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    status_t onSetupRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data); 

    status_t onPlayRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    status_t onPauseRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    status_t onTeardownRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    status_t onGetParameterRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    status_t onSetParameterRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    status_t onDescribeRequest(
            sp<ClientPushThread> &ClientInfo,
            const sp<ParsedMessage> &data);

    void sendErrorResponse(
            int32_t connectID,
            const char *errorDetail,
            int32_t cseq);

    static void AppendCommonResponse(
            AString *response, int32_t cseq, int32_t playbackSessionID = -1ll);

    int32_t makeUniquePlaybackSessionID() const;

    void finishStop();
    void disconnectClientAsync();
    void disconnectClient2();
    void finishStopAfterDisconnectingClient();
    void finishStop2();

    DISALLOW_EVIL_CONSTRUCTORS(RtkRtspServer);
};

}  // namespace android

#endif  // WIFI_DISPLAY_SOURCE_H_
