#ifndef IRTKTRANSCODEPLAYERSERVICE_H
#define IRTKTRANSCODEPLAYERSERVICE_H

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include "IRtkTranscodePlayerServiceClient.h"

namespace android{

/* this is the main service of Realtek transcode player service
 * client should call createConnection (with config parameter) to query connection
 * servier can reject client's request if it reaches it's limitation.
 */
class IRtkTranscodePlayerService: public IInterface
{
public:
	DECLARE_META_INTERFACE(RtkTranscodePlayerService);

    /* Create IRtkTranscodePlayerServiceClient.
     */
    virtual sp<IRtkTranscodePlayerServiceClient> create(int onlyTrack) = 0;

    /* Disconnect with RtkTranscodePlayerService
     */ 
    virtual bool disconnect() = 0;

    /**
     * Get number of connected clients
     */
    virtual int getClientsCount() = 0;

    virtual void increaseClientCount() = 0;
    virtual void decreaseClientCount() = 0;

};

// BnInterface.
class BnRtkTranscodePlayerService: public BnInterface<IRtkTranscodePlayerService>
{
public:

	virtual status_t onTransact(
		uint32_t code,
		const Parcel& data,
		Parcel* reply,
		uint32_t flags = 0);
};

}; // namespace android

#endif // IRTKTRANSCODEPLAYERSERVICE_H

