#define LOG_TAG "Sideband"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <ui/Point.h>
#include <ui/Rect.h>

#include <gui/IGraphicBufferProducer.h>
#include <gui/ISidebandClient.h>

// ---------------------------------------------------------------------------

namespace android {

enum {
    CREATE_SURFACE = IBinder::FIRST_CALL_TRANSACTION,
    DESTROY_SURFACE,
    GET_BINDER_KEY,
    BINDER_SURFACE,
    SURFACE_SETCROP,
    SURFACE_SETSIZE,
    SURFACE_SETLAYER,
    REQUEST_HWSYNC,
    GETAUDIOHWSYNC_BY_RESOURCE,
    DESTROYAUDIOHWSYNC_BY_RESOURCE,
    SURFACE_SETAUDIOHWSYNC,
    SURFACE_SETPOSITION,
    SURFACE_SETALPHA,
    SURFACE_SETFLAGS,
    SURFACE_SETRESOURCE,
    SURFACE_GETHDCPINFO,
};

class BpSidebandClient : public BpInterface<ISidebandClient>
{
public:
    BpSidebandClient(const sp<IBinder>& impl)
        : BpInterface<ISidebandClient>(impl) {
    }

    virtual ~BpSidebandClient();

    virtual status_t createSurface(const String8& name, uint32_t w,
            uint32_t h, PixelFormat format, uint32_t flags,
            sp<IBinder>* handle,
            sp<IGraphicBufferProducer>* gbp) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeString8(name);
        data.writeInt32(static_cast<int32_t>(w));
        data.writeInt32(static_cast<int32_t>(h));
        data.writeInt32(format);
        data.writeInt32(static_cast<int32_t>(flags));
        remote()->transact(CREATE_SURFACE, data, &reply);
        *handle = reply.readStrongBinder();
        *gbp = interface_cast<IGraphicBufferProducer>(reply.readStrongBinder());
        return reply.readInt32();
    }

    virtual status_t destroySurface(const sp<IBinder>& handle) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeStrongBinder(handle);
        remote()->transact(DESTROY_SURFACE, data, &reply);
        return reply.readInt32();
    }

    virtual status_t getBinderKey(const sp<IBinder>& handle, uint64_t * key) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeStrongBinder(handle);
        remote()->transact(GET_BINDER_KEY, data, &reply);
        *key = static_cast<uint64_t>(reply.readInt64());
        return reply.readInt32();
    }

    virtual status_t binderSurface(uint64_t key, sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt64(static_cast<int64_t>(key));
        remote()->transact(BINDER_SURFACE, data, &reply);
        *handle = reply.readStrongBinder();
        *gbp = interface_cast<IGraphicBufferProducer>(reply.readStrongBinder());
        return reply.readInt32();
    }

    virtual status_t setCrop(const sp<IBinder>& handle, const Rect& crop) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeUnpadded(&crop, sizeof(Rect));
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETCROP, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setPosition(const sp<IBinder>& handle, float x, float y) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeFloat(x);
        data.writeFloat(y);
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETPOSITION, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setSize(const sp<IBinder>& handle, uint32_t w, uint32_t h) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt32(static_cast<int32_t>(w));
        data.writeInt32(static_cast<int32_t>(h));
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETSIZE, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setLayer(const sp<IBinder>& handle, int32_t z) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt32(z);
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETLAYER, data, &reply);
        return reply.readInt32();
    }

    virtual status_t requestHwSync(int32_t* resource) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        remote()->transact(REQUEST_HWSYNC, data, &reply);
        *resource = static_cast<int32_t>(reply.readInt32());
        return reply.readInt32();
    }

    virtual status_t getHwSyncByResource(int32_t resource, int32_t* audioHwSync) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt32(resource);
        remote()->transact(GETAUDIOHWSYNC_BY_RESOURCE, data, &reply);
        *audioHwSync = static_cast<int32_t>(dup(reply.readFileDescriptor()));
        return reply.readInt32();
    }

    virtual status_t destroyHwSyncByResource(int32_t resource) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt32(resource);
        remote()->transact(DESTROYAUDIOHWSYNC_BY_RESOURCE, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setAudioHwSync(const sp<IBinder>& handle, int32_t audioHwSync) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeDupFileDescriptor(audioHwSync);
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETAUDIOHWSYNC, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setFlags(const sp<IBinder>& handle, uint32_t flags, uint32_t mask) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt32(static_cast<int32_t>(flags));
        data.writeInt32(static_cast<int32_t>(mask));
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETFLAGS, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setAlpha(const sp<IBinder>& handle, float alpha) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeFloat(alpha);
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETALPHA, data, &reply);
        return reply.readInt32();
    }

    virtual status_t setResource(const sp<IBinder>& handle, uint64_t resource) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeInt64(static_cast<int64_t>(resource));
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_SETRESOURCE, data, &reply);
        return reply.readInt32();
    }

    virtual status_t getHDCPInfo(const sp<IBinder>& handle, bool *info) {
        Parcel data, reply;
        data.writeInterfaceToken(ISidebandClient::getInterfaceDescriptor());
        data.writeStrongBinder(handle);
        remote()->transact(SURFACE_GETHDCPINFO, data, &reply);
        *info = static_cast<uint64_t>(reply.readInt32());
        return reply.readInt32();
    }

};

// Out-of-line virtual method definition to trigger vtable emission in this
// translation unit (see clang warning -Wweak-vtables)
BpSidebandClient::~BpSidebandClient() {}

IMPLEMENT_META_INTERFACE(SidebandClient, "android.ui.ISidebandClient");

// ----------------------------------------------------------------------

status_t BnSidebandClient::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
     switch(code) {
        case CREATE_SURFACE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            String8 name = data.readString8();
            uint32_t w = static_cast<uint32_t>(data.readInt32());
            uint32_t h = static_cast<uint32_t>(data.readInt32());
            PixelFormat format = data.readInt32();
            uint32_t _flags = static_cast<uint32_t>(data.readInt32());
            sp<IBinder> handle;
            sp<IGraphicBufferProducer> gbp;
            status_t result = createSurface(name, w, h, format, _flags,
                    &handle, &gbp);
            reply->writeStrongBinder(handle);
            reply->writeStrongBinder(IInterface::asBinder(gbp));
            reply->writeInt32(result);
            return NO_ERROR;
        }
        case DESTROY_SURFACE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            reply->writeInt32(destroySurface( data.readStrongBinder() ) );
            return NO_ERROR;
        }
        case GET_BINDER_KEY: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            uint64_t key;
            sp<IBinder> handle = data.readStrongBinder();
            status_t result = getBinderKey(handle, &key);
            reply->writeInt64(static_cast<int64_t>(key));
            reply->writeInt32(static_cast<int32_t>(result));
            return NO_ERROR;
        }
        case BINDER_SURFACE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            sp<IBinder> handle;
            sp<IGraphicBufferProducer> gbp;
            uint64_t key = static_cast<uint64_t>(data.readInt64());
            status_t result = binderSurface(key, &handle, &gbp);
            reply->writeStrongBinder(handle);
            reply->writeStrongBinder(IInterface::asBinder(gbp));
            reply->writeInt32(result);
            return NO_ERROR;
        }
        case SURFACE_SETCROP: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            const Rect *pCrop = static_cast<const Rect *>(data.readInplace(sizeof(Rect)));
            reply->writeInt32( setCrop( data.readStrongBinder(), *pCrop) );
            return NO_ERROR;
        }
        case SURFACE_SETSIZE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            uint32_t w = static_cast<uint32_t>(data.readInt32());
            uint32_t h = static_cast<uint32_t>(data.readInt32());
            reply->writeInt32( setSize( data.readStrongBinder(), w, h) );
            return NO_ERROR;
        }
        case SURFACE_SETLAYER: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            int32_t z = data.readInt32();
            reply->writeInt32( setLayer( data.readStrongBinder(), z) );
            return NO_ERROR;
        }
        case REQUEST_HWSYNC: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            int32_t resource;
            status_t result = requestHwSync(&resource);
            reply->writeInt32(static_cast<int32_t>(resource));
            reply->writeInt32(static_cast<int32_t>(result));
            return NO_ERROR;
        }
        case GETAUDIOHWSYNC_BY_RESOURCE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            int32_t audioHwSync;
            int32_t resource = static_cast<int32_t>(data.readInt32());
            status_t result = getHwSyncByResource(resource, &audioHwSync);
            reply->writeDupFileDescriptor(static_cast<int32_t>(audioHwSync));
            reply->writeInt32(static_cast<int32_t>(result));
            return NO_ERROR;
        }
        case DESTROYAUDIOHWSYNC_BY_RESOURCE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            int32_t resource = static_cast<int32_t>(data.readInt32());
            status_t result = destroyHwSyncByResource(resource);
            reply->writeInt32(static_cast<int32_t>(result));
            return NO_ERROR;
        }
        case SURFACE_SETAUDIOHWSYNC: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            int audioHwSync = dup(data.readFileDescriptor());
            reply->writeInt32( setAudioHwSync( data.readStrongBinder(), audioHwSync) );
            return NO_ERROR;
        }
        case SURFACE_SETPOSITION: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            float x = data.readFloat();
            float y = data.readFloat();
            reply->writeInt32( setPosition( data.readStrongBinder(), x, y) );
            return NO_ERROR;
        }
        case SURFACE_SETALPHA: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            float alpha = data.readFloat();
            reply->writeInt32( setAlpha( data.readStrongBinder(), alpha) );
            return NO_ERROR;
        }
        case SURFACE_SETFLAGS: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            uint32_t _flags = static_cast<uint32_t>(data.readInt32());
            uint32_t mask = static_cast<uint32_t>(data.readInt32());
            reply->writeInt32( setFlags( data.readStrongBinder(), _flags, mask) );
            return NO_ERROR;
        }
        case SURFACE_SETRESOURCE: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            uint64_t resource = static_cast<uint64_t>(data.readInt64());
            reply->writeInt32( setResource( data.readStrongBinder(), resource) );
            return NO_ERROR;
        }
        case SURFACE_GETHDCPINFO: {
            CHECK_INTERFACE(ISidebandClient, data, reply);
            bool info;
            sp<IBinder> handle = data.readStrongBinder();
            status_t result = getHDCPInfo(handle, &info);
            reply->writeInt32(static_cast<int32_t>(info));
            reply->writeInt32(static_cast<int32_t>(result));
            return NO_ERROR;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}; // namespace android
