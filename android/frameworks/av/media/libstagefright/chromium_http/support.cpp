/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "ChromiumHTTPDataSourceSupport"
#include <utils/Log.h>

#include <media/stagefright/foundation/AString.h>

#include "support.h"

#include "android/net/android_network_library_impl.h"
#include "base/logging.h"
#include "base/threading/thread.h"
#include "net/base/cert_verifier.h"
#include "net/base/cookie_monster.h"
#include "net/base/host_resolver.h"
#include "net/base/ssl_config_service.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_cache.h"
#include "net/proxy/proxy_config_service_android.h"

#include "include/ChromiumHTTPDataSource.h"
#include <arpa/inet.h>
#include <binder/Parcel.h>
#include <cutils/log.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/Utils.h>
#include <string>
#ifdef ENABLE_DTCP_IP
#include <ctype.h>
#endif //ENABLE_DTCP_IP

#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <binder/IServiceManager.h>

namespace android {

// must be kept in sync with interface defined in IAudioService.aidl
class IAudioService : public IInterface
{
public:
    DECLARE_META_INTERFACE(AudioService);

    virtual int verifyX509CertChain(
            const std::vector<std::string>& cert_chain,
            const std::string& hostname,
            const std::string& auth_type) = 0;
};

class BpAudioService : public BpInterface<IAudioService>
{
public:
    BpAudioService(const sp<IBinder>& impl)
        : BpInterface<IAudioService>(impl)
    {
    }

    virtual int verifyX509CertChain(
            const std::vector<std::string>& cert_chain,
            const std::string& hostname,
            const std::string& auth_type)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IAudioService::getInterfaceDescriptor());

        // The vector of std::string we get isn't really a vector of strings,
        // but rather a vector of binary certificate data. If we try to pass
        // it to Java language code as a string, it ends up mangled on the other
        // side, so send them as bytes instead.
        // Since we can't send an array of byte arrays, send a single array,
        // which will be split out by the recipient.

        int numcerts = cert_chain.size();
        data.writeInt32(numcerts);
        size_t total = 0;
        for (int i = 0; i < numcerts; i++) {
            total += cert_chain[i].size();
        }
        size_t bytesize = total + numcerts * 4;
        uint8_t *bytes = (uint8_t*) malloc(bytesize);
        if (!bytes) {
            return 5; // SSL_INVALID
        }
        ALOGV("%d certs: %d -> %d", numcerts, total, bytesize);

        int offset = 0;
        for (int i = 0; i < numcerts; i++) {
            int32_t certsize = cert_chain[i].size();
            // store this in a known order, which just happens to match the default
            // byte order of a java ByteBuffer
            int32_t bigsize = htonl(certsize);
            ALOGV("cert %d, size %d", i, certsize);
            memcpy(bytes + offset, &bigsize, sizeof(bigsize));
            offset += sizeof(bigsize);
            memcpy(bytes + offset, cert_chain[i].data(), certsize);
            offset += certsize;
        }
        data.writeByteArray(bytesize, bytes);
        free(bytes);
        data.writeString16(String16(hostname.c_str()));
        data.writeString16(String16(auth_type.c_str()));

        int32_t result;
        if (remote()->transact(IBinder::FIRST_CALL_TRANSACTION, data, &reply) != NO_ERROR
                || reply.readExceptionCode() < 0 || reply.readInt32(&result) != NO_ERROR) {
            return 5; // SSL_INVALID;
        }
        return result;
    }

};

IMPLEMENT_META_INTERFACE(AudioService, "android.media.IAudioService");


static Mutex gNetworkThreadLock;
static base::Thread *gNetworkThread = NULL;
static scoped_refptr<SfRequestContext> gReqContext;
static scoped_ptr<net::NetworkChangeNotifier> gNetworkChangeNotifier;

bool logMessageHandler(
        int severity,
        const char* file,
        int line,
        size_t message_start,
        const std::string& str) {
    int androidSeverity = ANDROID_LOG_VERBOSE;
    switch(severity) {
    case logging::LOG_FATAL:
        androidSeverity = ANDROID_LOG_FATAL;
        break;
    case logging::LOG_ERROR_REPORT:
    case logging::LOG_ERROR:
        androidSeverity = ANDROID_LOG_ERROR;
        break;
    case logging::LOG_WARNING:
        androidSeverity = ANDROID_LOG_WARN;
        break;
    default:
        androidSeverity = ANDROID_LOG_VERBOSE;
        break;
    }
    android_printLog(androidSeverity, "chromium-libstagefright",
                    "%s:%d: %s", file, line, str.c_str());
    return false;
}

struct AutoPrioritySaver {
    AutoPrioritySaver()
        : mTID(androidGetTid()),
          mPrevPriority(androidGetThreadPriority(mTID)) {
        androidSetThreadPriority(mTID, ANDROID_PRIORITY_NORMAL);
    }

    ~AutoPrioritySaver() {
        androidSetThreadPriority(mTID, mPrevPriority);
    }

private:
    pid_t mTID;
    int mPrevPriority;

    DISALLOW_EVIL_CONSTRUCTORS(AutoPrioritySaver);
};

static void InitializeNetworkThreadIfNecessary() {
    Mutex::Autolock autoLock(gNetworkThreadLock);

    if (gNetworkThread == NULL) {
        // Make sure any threads spawned by the chromium framework are
        // running at normal priority instead of inheriting this thread's.
        AutoPrioritySaver saver;

        gNetworkThread = new base::Thread("network");
        base::Thread::Options options;
        options.message_loop_type = MessageLoop::TYPE_IO;
        CHECK(gNetworkThread->StartWithOptions(options));

        gReqContext = new SfRequestContext;

        gNetworkChangeNotifier.reset(net::NetworkChangeNotifier::Create());

        net::AndroidNetworkLibrary::RegisterSharedInstance(
                new SfNetworkLibrary);
        logging::SetLogMessageHandler(logMessageHandler);
    }
}

static void MY_LOGI(const char *s) {
    LOG_PRI(ANDROID_LOG_INFO, LOG_TAG, "%s", s);
}

static void MY_LOGV(const char *s) {
#if !defined(LOG_NDEBUG) || LOG_NDEBUG == 0
    LOG_PRI(ANDROID_LOG_VERBOSE, LOG_TAG, "%s", s);
#endif
}

SfNetLog::SfNetLog()
    : mNextID(1) {
}

void SfNetLog::AddEntry(
        EventType type,
        const base::TimeTicks &time,
        const Source &source,
        EventPhase phase,
        EventParameters *params) {
#if 0
    MY_LOGI(StringPrintf(
                "AddEntry time=%s type=%s source=%s phase=%s\n",
                TickCountToString(time).c_str(),
                EventTypeToString(type),
                SourceTypeToString(source.type),
                EventPhaseToString(phase)).c_str());
#endif
}

uint32 SfNetLog::NextID() {
    return mNextID++;
}

net::NetLog::LogLevel SfNetLog::GetLogLevel() const {
    return LOG_BASIC;
}

////////////////////////////////////////////////////////////////////////////////

SfRequestContext::SfRequestContext() {
    mUserAgent = MakeUserAgent().c_str();

    set_net_log(new SfNetLog());

    set_host_resolver(
        net::CreateSystemHostResolver(
                net::HostResolver::kDefaultParallelism,
                NULL /* resolver_proc */,
                net_log()));

    set_ssl_config_service(
        net::SSLConfigService::CreateSystemSSLConfigService());

    mProxyConfigService = new net::ProxyConfigServiceAndroid;

    set_proxy_service(net::ProxyService::CreateWithoutProxyResolver(
        mProxyConfigService, net_log()));

    set_http_transaction_factory(new net::HttpCache(
            host_resolver(),
            new net::CertVerifier(),
            dnsrr_resolver(),
            dns_cert_checker(),
            proxy_service(),
            ssl_config_service(),
            net::HttpAuthHandlerFactory::CreateDefault(host_resolver()),
            network_delegate(),
            net_log(),
            NULL));  // backend_factory

    set_cookie_store(new net::CookieMonster(NULL, NULL));
}

const std::string &SfRequestContext::GetUserAgent(const GURL &url) const {
    return mUserAgent;
}

status_t SfRequestContext::updateProxyConfig(
        const char *host, int32_t port, const char *exclusionList) {
    Mutex::Autolock autoLock(mProxyConfigLock);

    if (host == NULL || *host == '\0') {
        MY_LOGV("updateProxyConfig NULL");

        std::string proxy;
        std::string exList;
        mProxyConfigService->UpdateProxySettings(proxy, exList);
    } else {
#if !defined(LOG_NDEBUG) || LOG_NDEBUG == 0
        LOG_PRI(ANDROID_LOG_VERBOSE, LOG_TAG,
                "updateProxyConfig %s:%d, exclude '%s'",
                host, port, exclusionList);
#endif

        std::string proxy = StringPrintf("%s:%d", host, port).c_str();
        std::string exList = exclusionList;
        mProxyConfigService->UpdateProxySettings(proxy, exList);
    }

    return OK;
}

////////////////////////////////////////////////////////////////////////////////

SfNetworkLibrary::SfNetworkLibrary() {}

SfNetworkLibrary::VerifyResult SfNetworkLibrary::VerifyX509CertChain(
        const std::vector<std::string>& cert_chain,
        const std::string& hostname,
        const std::string& auth_type) {

    sp<IBinder> binder =
        defaultServiceManager()->checkService(String16("audio"));
    if (binder == 0) {
        ALOGW("Thread cannot connect to the audio service");
    } else {
        sp<IAudioService> service = interface_cast<IAudioService>(binder);
        int code = service->verifyX509CertChain(cert_chain, hostname, auth_type);
        ALOGV("verified: %d", code);
        if (code == -1) {
            return VERIFY_OK;
        } else if (code == 2) { // SSL_IDMISMATCH
            return VERIFY_BAD_HOSTNAME;
        } else if (code == 3) { // SSL_UNTRUSTED
            return VERIFY_NO_TRUSTED_ROOT;
        }
    }
    return VERIFY_INVOCATION_ERROR;
}

////////////////////////////////////////////////////////////////////////////////

SfDelegate::SfDelegate()
    : mOwner(NULL),
      mURLRequest(NULL),
      mReadBuffer(new net::IOBufferWithSize(8192)),
      mNumBytesRead(0),
      mNumBytesTotal(0),
      mDataDestination(NULL),
      mAtEOS(false) {

#ifdef ENABLE_DTCP_IP
    mpDtcpSinkWrapper = NULL;
    resetDtcpip();
#endif //ENABLE_DTCP_IP

    InitializeNetworkThreadIfNecessary();
}

SfDelegate::~SfDelegate() {
    CHECK(mURLRequest == NULL);

#ifdef ENABLE_DTCP_IP
    ReleaseDtcpip();
#endif //ENABLE_DTCP_IP
}

// static
status_t SfDelegate::UpdateProxyConfig(
        const char *host, int32_t port, const char *exclusionList) {
    InitializeNetworkThreadIfNecessary();

    return gReqContext->updateProxyConfig(host, port, exclusionList);
}

void SfDelegate::setOwner(ChromiumHTTPDataSource *owner) {
    mOwner = owner;
}

void SfDelegate::setUID(uid_t uid) {
    gReqContext->setUID(uid);
}

bool SfDelegate::getUID(uid_t *uid) const {
    return gReqContext->getUID(uid);
}

AString SfDelegate::getRedirectURL() {
    return mRedirectUrl;
}

void SfDelegate::OnReceivedRedirect(
            net::URLRequest *request, const GURL &new_url, bool *defer_redirect) {
    std::string location;

    request->GetResponseHeaderByName("Location", &location);
    mRedirectUrl = location.c_str();
    //ALOGD("\033[0;31;31m [%s - %d] loacation url:'%s'  \033[m\n", __FILE__, __LINE__, location.c_str());
    MY_LOGV("OnReceivedRedirect");
    mOwner->onRedirect(new_url.spec().c_str());
}

void SfDelegate::OnAuthRequired(
            net::URLRequest *request, net::AuthChallengeInfo *auth_info) {
    MY_LOGV("OnAuthRequired");

    inherited::OnAuthRequired(request, auth_info);
}

void SfDelegate::OnCertificateRequested(
            net::URLRequest *request, net::SSLCertRequestInfo *cert_request_info) {
    MY_LOGV("OnCertificateRequested");

    inherited::OnCertificateRequested(request, cert_request_info);
}

void SfDelegate::OnSSLCertificateError(
            net::URLRequest *request, int cert_error, net::X509Certificate *cert) {
    fprintf(stderr, "OnSSLCertificateError cert_error=%d\n", cert_error);

    inherited::OnSSLCertificateError(request, cert_error, cert);
}

void SfDelegate::OnGetCookies(net::URLRequest *request, bool blocked_by_policy) {
    MY_LOGV("OnGetCookies");
}

void SfDelegate::OnSetCookie(
        net::URLRequest *request,
        const std::string &cookie_line,
        const net::CookieOptions &options,
        bool blocked_by_policy) {
    MY_LOGV("OnSetCookie");
}

void SfDelegate::OnResponseStarted(net::URLRequest *request) {
    if (request->status().status() != net::URLRequestStatus::SUCCESS) {
        MY_LOGI(StringPrintf(
                    "Request failed with status %d and os_error %d",
                    request->status().status(),
                    request->status().os_error()).c_str());

        delete mURLRequest;
        mURLRequest = NULL;

        mOwner->onConnectionFailed(ERROR_IO);
        return;
    } else if (mRangeRequested && request->GetResponseCode() != 206) {
        MY_LOGI(StringPrintf(
                    "We requested a content range, but server didn't "
                    "support that. (responded with %d)",
                    request->GetResponseCode()).c_str());

        delete mURLRequest;
        mURLRequest = NULL;

        mOwner->onConnectionFailed(-EPIPE);
        return;
    } else if ((request->GetResponseCode() / 100) != 2) {
        MY_LOGI(StringPrintf(
                    "Server responded with http status %d",
                    request->GetResponseCode()).c_str());

        delete mURLRequest;
        mURLRequest = NULL;

        mOwner->onConnectionFailed(ERROR_IO);
        return;
    }

    MY_LOGV("OnResponseStarted");

    std::string headers;
    request->GetAllResponseHeaders(&headers);

    MY_LOGV(StringPrintf("response headers: %s", headers.c_str()).c_str());

    std::string contentType;
    request->GetResponseHeaderByName("Content-Type", &contentType);

    mOwner->onConnectionEstablished(
            request->GetExpectedContentSize(), contentType.c_str());
}

void SfDelegate::OnReadCompleted(net::URLRequest *request, int bytes_read) {
    if (bytes_read == -1) {
        MY_LOGI(StringPrintf(
                    "OnReadCompleted, read failed, status %d",
                    request->status().status()).c_str());

        mOwner->onReadCompleted(ERROR_IO);
        return;
    }

    MY_LOGV(StringPrintf("OnReadCompleted, read %d bytes", bytes_read).c_str());

    if (bytes_read < 0) {
        MY_LOGI(StringPrintf(
                    "Read failed w/ status %d\n",
                    request->status().status()).c_str());

        mOwner->onReadCompleted(ERROR_IO);
        return;
    } else if (bytes_read == 0) {
        mAtEOS = true;
        mOwner->onReadCompleted(mNumBytesRead);
        return;
    }

    CHECK_GT(bytes_read, 0);

#ifdef ENABLE_DTCP_IP
    MY_LOGV(StringPrintf("OnReadCompleted(), mNumBytesRead:%d, mNumBytesTotal:%d", mNumBytesRead, mNumBytesTotal).c_str());
    size_t dst_buf_size = mNumBytesTotal - mNumBytesRead;//free space of destination buffer
    if (DecryptDtcpipData((unsigned char*)mReadBuffer->data(), (size_t)bytes_read, (unsigned char*)mDataDestination + mNumBytesRead, &dst_buf_size)) {
        MY_LOGV(StringPrintf("decrypt dtcpip data successfully, enc bytes:[%d], dec bytes:[%u]",bytes_read, dst_buf_size).c_str());
        //decrypt successfully
        mNumBytesRead += dst_buf_size;
    } else
#endif //ENABLE_DTCP_IP
    {
    CHECK_LE(mNumBytesRead + bytes_read, mNumBytesTotal);

    memcpy((uint8_t *)mDataDestination + mNumBytesRead,
           mReadBuffer->data(),
           bytes_read);

    mNumBytesRead += bytes_read;
    }

    readMore(request);
}

void SfDelegate::readMore(net::URLRequest *request) {
    while (mNumBytesRead < mNumBytesTotal) {
        size_t copy = mNumBytesTotal - mNumBytesRead;
        if (copy > mReadBuffer->size()) {
            copy = mReadBuffer->size();
        }
#ifdef ENABLE_DTCP_IP
        if (copy < 30 /*PCP header(14 bytes) + padding(max 16 bytes) */) {
            copy = 30;
        }
        MY_LOGV(StringPrintf("readMore(), mNumBytesRead:[%u], mNumBytesTotal:[%u], copy:[%u]", mNumBytesRead, mNumBytesTotal, copy).c_str());
#endif //ENABLE_DTCP_IP

        int n;
        if (request->Read(mReadBuffer, copy, &n)) {
            MY_LOGV(StringPrintf("Read %d bytes directly.", n).c_str());

#ifdef ENABLE_DTCP_IP
            size_t dst_buf_size = mNumBytesTotal - mNumBytesRead;
            if (DecryptDtcpipData((unsigned char*)mReadBuffer->data(), (size_t)n, (unsigned char*)mDataDestination + mNumBytesRead, &dst_buf_size)) {
                MY_LOGV(StringPrintf("decrypt dtcpip data successfully, enc bytes:[%d], dec bytes:[%u]", n, dst_buf_size).c_str());
                //decrypt successfully
                mNumBytesRead += dst_buf_size;
            } else
#endif //ENABLE_DTCP_IP
            {
            CHECK_LE((size_t)n, copy);

            memcpy((uint8_t *)mDataDestination + mNumBytesRead,
                   mReadBuffer->data(),
                   n);

            mNumBytesRead += n;
            }

            if (n == 0) {
                mAtEOS = true;
                break;
            }
        } else {
            MY_LOGV("readMore pending read");

            if (request->status().status() != net::URLRequestStatus::IO_PENDING) {
                MY_LOGI(StringPrintf(
                            "Direct read failed w/ status %d\n",
                            request->status().status()).c_str());

                mOwner->onReadCompleted(ERROR_IO);
                return;
            }

            return;
        }
    }

    mOwner->onReadCompleted(mNumBytesRead);
}

void SfDelegate::initiateConnection(
        const char *uri,
        const KeyedVector<String8, String8> *headers,
        off64_t offset) {
    GURL url(uri);
#ifdef ENABLE_DTCP_IP
    //parse dtcpip info from input url
    parseDtcpipInfo(url);

    //DTCP-IP AKE
    doAKE();
#endif //ENABLE_DTCP_IP

    MessageLoop *loop = gNetworkThread->message_loop();
    loop->PostTask(
            FROM_HERE,
            NewRunnableFunction(
                &SfDelegate::OnInitiateConnectionWrapper,
                this,
                url,
                headers,
                offset));

}

// static
void SfDelegate::OnInitiateConnectionWrapper(
        SfDelegate *me, GURL url,
        const KeyedVector<String8, String8> *headers,
        off64_t offset) {
    me->onInitiateConnection(url, headers, offset);
}

void SfDelegate::onInitiateConnection(
        const GURL &url,
        const KeyedVector<String8, String8> *extra,
        off64_t offset) {
    CHECK(mURLRequest == NULL);

    mURLRequest = new net::URLRequest(url, this);
    mAtEOS = false;

    mRangeRequested = false;

    if (offset != 0 || extra != NULL) {
        net::HttpRequestHeaders headers =
            mURLRequest->extra_request_headers();

        if (offset != 0) {
            headers.AddHeaderFromString(
                    StringPrintf("Range: bytes=%lld-", offset).c_str());

            mRangeRequested = true;
        }

        if (extra != NULL) {
            for (size_t i = 0; i < extra->size(); ++i) {
                AString s;
                s.append(extra->keyAt(i).string());
                s.append(": ");
                s.append(extra->valueAt(i).string());

                headers.AddHeaderFromString(s.c_str());
            }
        }

        mURLRequest->SetExtraRequestHeaders(headers);
    }

    mURLRequest->set_context(gReqContext);

    mURLRequest->Start();
}

void SfDelegate::initiateDisconnect() {
#ifdef ENABLE_DTCP_IP
    resetDtcpip();
#endif //ENABLE_DTCP_IP
    MessageLoop *loop = gNetworkThread->message_loop();
    loop->PostTask(
            FROM_HERE,
            NewRunnableFunction(
                &SfDelegate::OnInitiateDisconnectWrapper, this));
}

// static
void SfDelegate::OnInitiateDisconnectWrapper(SfDelegate *me) {
    me->onInitiateDisconnect();
}

void SfDelegate::onInitiateDisconnect() {
    if (mURLRequest == NULL) {
        return;
    }

    mURLRequest->Cancel();

    delete mURLRequest;
    mURLRequest = NULL;

    mOwner->onDisconnectComplete();
}

void SfDelegate::initiateRead(void *data, size_t size) {
    MessageLoop *loop = gNetworkThread->message_loop();
    loop->PostTask(
            FROM_HERE,
            NewRunnableFunction(
                &SfDelegate::OnInitiateReadWrapper, this, data, size));
}

// static
void SfDelegate::OnInitiateReadWrapper(
        SfDelegate *me, void *data, size_t size) {
    me->onInitiateRead(data, size);
}

void SfDelegate::onInitiateRead(void *data, size_t size) {
    CHECK(mURLRequest != NULL);

    mNumBytesRead = 0;
    mNumBytesTotal = size;
    mDataDestination = data;

    if (mAtEOS) {
        mOwner->onReadCompleted(0);
        return;
    }

    readMore(mURLRequest);
}

#ifdef ENABLE_DTCP_IP
bool SfDelegate::DecryptDtcpipData(unsigned char* pEncBuf, size_t encSize, unsigned char* pDecBuf, size_t* decSize) {
    MY_LOGV(StringPrintf("[%s, %d]",__func__,__LINE__).c_str());
    if (NULL != mpDtcpSinkWrapper && mAkeOk) {
        char* pSrc;
        size_t encDataSize;
        size_t prcdDataSize = 0; //processed encrypted data size
        size_t contentDataSize = *decSize;// clear content buffer size as input
        MY_LOGV(StringPrintf("before.. mRemainingEncDataSize:[%u]\n", mRemainingEncDataSize).c_str());
        if (mRemainingEncDataSize > 0) {
            //Concatenates remaining enc data and input enc buffer
            pSrc = (char*)mRemainingEncData;
            encDataSize = mRemainingEncDataSize + encSize;
            if (encDataSize > sizeof(mRemainingEncData)) {
                LOG_PRI(ANDROID_LOG_ERROR, LOG_TAG, "[DTCP-IP] encrypt data size is larger than buffer size!!");
                mRemainingEncDataSize = 0;//skip remaining data
                return false;
            }
            memcpy(pSrc + mRemainingEncDataSize, pEncBuf, encSize);
        } else {
            // no encrypted data remained, just decrypt data from input enc buffer
            pSrc = (char*) pEncBuf;
            encDataSize = encSize;
        }

        MY_LOGV(StringPrintf("before depacketizeData(), enc data size:[%u], content data size:[%u], processed data size:[%u]\n", encDataSize, contentDataSize, prcdDataSize).c_str());
        int res = mpDtcpSinkWrapper->depacketizeData(
            pSrc,
            encDataSize,
            (char*) pDecBuf,
            &contentDataSize,
            &prcdDataSize);

        MY_LOGV(StringPrintf("after depacketizeData(), content data size:[%u], processed data size:[%u], return:[%d]\n", contentDataSize, prcdDataSize, res).c_str());

        if (res == DTCP_W_OK) {
            *decSize = contentDataSize; // decrypted data size as output

            if (encDataSize > prcdDataSize) {
                if (mRemainingEncDataSize > 0) {
                    // pSrc is pointer mRemainingEncData
                    mRemainingEncDataSize = encDataSize - prcdDataSize;
                    if (prcdDataSize > 0) {
                        unsigned char* pRemainSrc = (unsigned char*)pSrc + prcdDataSize;
                        for(size_t i = 0; i < mRemainingEncDataSize; i++) {
                            mRemainingEncData[i] = *(pRemainSrc + i);
                        }
                    }
                } else {
                    // the pSrc is pointer to pEncBuf
                    mRemainingEncDataSize = encDataSize - prcdDataSize;
                    memcpy(mRemainingEncData, pSrc + prcdDataSize, mRemainingEncDataSize);
                }
            } else {
                //all enc data consumed....
                mRemainingEncDataSize = 0;
            }
            MY_LOGV(StringPrintf("after.. mRemainingEncDataSize:[%u]\n", mRemainingEncDataSize).c_str());
            return true;
        } else {
            mRemainingEncDataSize = 0;
            return false;
        }
    }
    return false;
}

void SfDelegate::ReleaseDtcpip() {
    MY_LOGV(StringPrintf("[%s, %d]",__func__,__LINE__).c_str());
    if (NULL != mpDtcpSinkWrapper) {
        DtcpSinkWrapper::releaseInstance(mpDtcpSinkWrapper);
        mpDtcpSinkWrapper = NULL;
    }
}

void SfDelegate::decodeUrl(std::string & url)
{
    MY_LOGV(StringPrintf("[%s, %d]",__func__,__LINE__).c_str());
    if (!url.empty()) {
        char* buf = (char*)malloc(url.length() + 1);
        memset(buf, 0 , url.length() + 1);
        char* dst = buf;
        const char* src = url.c_str();
        char a, b;

        while (*src) {
            if ((*src == '%') &&
                ((a = src[1]) && (b = src[2])) &&
                (isxdigit(a) && isxdigit(b))) {
                if (a >= 'a') {
                    a -= 'A'-'a';
                }
                if (a >= 'A') {
                    a -= ('A' - 10);
                } else {
                    a -= '0';
                }
                if (b >= 'a') {
                    b -= 'A'-'a';
                }
                if (b >= 'A') {
                    b -= ('A' - 10);
                } else {
                    b -= '0';
                }
                *dst++ = 16*a+b;
                src+=3;
            } else {
                *dst++ = *src++;
            }
        }
        *dst++ = '\0';

        url.assign(buf);
        free(buf);
    }

}

void SfDelegate::parseDtcpipInfo(GURL &url)
{
    MY_LOGV(StringPrintf("[%s, %d]",__func__,__LINE__).c_str());
    std::string query = url.query();
    if (!query.empty()) {
        MY_LOGV(StringPrintf("query from GURL: %s", query.c_str()).c_str());
        size_t key_pos = query.find("DtcpipInfo=");
        if (std::string::npos != key_pos) {
            //To find dtcpip info, we should extract it, and remove it from the original url
            size_t value_begin = key_pos + strlen("DtcpipInfo=");
            size_t value_end = query.find("&", value_begin);
            size_t remove_begin;
            size_t remove_end;
            if (std::string::npos != value_end) {
                // there is other query part on tail
                remove_begin = key_pos;
                remove_end = value_end;
                value_end--;
            } else {
                // no more query part on tail
                remove_begin = (key_pos == 0)? key_pos : (key_pos - 1);
                remove_end = value_end = query.length() - 1;
            }
            MY_LOGV(StringPrintf("[value start, value end][%d, %d]; [remove_start, remove_end][%d, %d]",value_begin, value_end, remove_begin, remove_end).c_str());

            //copy value
            mDtcpPInfo = query.substr(value_begin, (value_end - value_begin +1));
            decodeUrl(mDtcpPInfo);
            MY_LOGV(StringPrintf("dtcpip pinfo:%s",mDtcpPInfo.c_str()).c_str());

            //remove from query part
            std::string newQueryStr(query);
            newQueryStr.erase(remove_begin, (remove_end - remove_begin + 1));
            MY_LOGV(StringPrintf("new query: %s",newQueryStr.c_str()).c_str());
            GURL::Replacements repl;
            repl.SetQueryStr(newQueryStr);
            url = url.ReplaceComponents(repl);
            MY_LOGV(StringPrintf("new url: %s", url.spec().c_str()).c_str());
        }
    }
}

void SfDelegate::doAKE()
{
    MY_LOGV(StringPrintf("[%s, %d]",__func__,__LINE__).c_str());
    mAkeOk = false;
    if (!mDtcpPInfo.empty()) {
        if (NULL == mpDtcpSinkWrapper) {
            mpDtcpSinkWrapper = DtcpSinkWrapper::getInstance();
        }

        if (NULL != mpDtcpSinkWrapper) {
            MY_LOGV("Hey, start to do AKE!");
            int retVal = mpDtcpSinkWrapper->startAKE(mDtcpPInfo.c_str());
            if (DTCP_W_OK != retVal) {
                MY_LOGI(StringPrintf("startAKE failed with ERROR code: %d", retVal).c_str());
            } else {
                MY_LOGV("AKE successfully!!!");
                mAkeOk = true;
            }
        }
    }
}

void SfDelegate::resetDtcpip()
{
    MY_LOGV(StringPrintf("[%s, %d]",__func__,__LINE__).c_str());
    mDtcpPInfo.clear();
    mAkeOk = false;
    mRemainingEncDataSize = 0;
}

#endif //ENABLE_DTCP_IP


}  // namespace android

