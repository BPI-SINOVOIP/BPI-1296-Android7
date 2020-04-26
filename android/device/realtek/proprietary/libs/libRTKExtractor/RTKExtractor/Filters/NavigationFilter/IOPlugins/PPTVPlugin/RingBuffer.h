#ifndef  IPANEL_RINGBUFFER_H
#define  IPANEL_RINGBUFFER_H

#include <vector>


namespace PPLive{
namespace RingBuffer{


#define uint32_t        unsigned long
#define uint16_t        unsigned short
#define uint8_t         unsigned char
#define uint64_t	unsigned long long 
#define int64_t 	long long 

class RingBuffer 
{
public:
    /**
     *         
     */
    RingBuffer(uint32_t size, uint32_t maxAccessUnits);
    virtual ~RingBuffer();

    // RingBufferSender Interface
    //

    virtual uint32_t space() const;
    virtual void sendAccessUnitBegin(uint32_t flags, int64_t timestamp);
    virtual bool write(const unsigned char* data, uint32_t size);
    virtual bool sendAccessUnitEnd();

    // RingBufferReceiver Interface
    //

    virtual int read(unsigned char* data,int size );
    virtual bool readCommit();


    virtual bool readAccessUnitPeek(unsigned char* data,
                                    uint32_t &size,
                                    uint32_t &flags,
                                    int64_t &timestamp) const;

    virtual bool readAccessUnitCommit();
    virtual void flush();

private:
    static const uint32_t INVALID_WP = 0xFFFFFFFF;

    struct AccessUnit
    {
        uint32_t wpBegin_;
        uint32_t wpEnd_;
        uint32_t flags_;
        int64_t timestamp_;
    };

    uint32_t volatile m_wp;
    uint32_t volatile m_rp;
    uint32_t  m_size;
    unsigned char*  m_data;

    uint32_t pos;


    uint32_t volatile m_wpAuQueue;
    uint32_t volatile m_rpAuQueue;
    uint32_t  m_maxAccessUnits;
    AccessUnit*  m_accessUnitQueue;

    mutable std::vector<unsigned char> m_mergeBuffer;

private:
    virtual uint32_t aVailableSize() const;
	};

	}
	}
#endif // IPANEL_RINGBUFFER_H
