#ifndef __C_MEDIA_TYPE_H__
#define __C_MEDIA_TYPE_H__

#include "SType.h"

class CMediaType{
public:
                                CMediaType(void);
                                ~CMediaType(void);
    bool                        operator== (const CMediaType&) const;
    bool                        operator== (const MEDIA_TYPE&) const;
    bool                        operator== (const MEDIA_TYPE*) const;

                                // cast operator
                                // operator const char*();
                                operator const MEDIA_TYPE*() { return &m_mt; }
    void                        InitMediaType(void);
    void                        CopyTo(MEDIA_TYPE*) const;
    void                        CopyTo(MEDIA_TYPE&) const;
    void                        SetMajorType(const ENUM_MEDIA_TYPE);
    void                        SetSubType(const ENUM_MEDIA_TYPE);
    void                        SetSampleSize(const long);
    void                        SetFormatType(const FORMAT_TYPE);
    void                        SetFormat(const BYTE*, const ULONG length);

    ENUM_MEDIA_TYPE             GetMajorType(void);
    ENUM_MEDIA_TYPE             GetSubType(void);
    long                        GetSampleSize(void);
    FORMAT_TYPE                 GetFormatType(void);
    void                        GetFormat(BYTE*&, ULONG& length);

    MEDIA_TYPE*                 GetMediaType(void);

private:
    MEDIA_TYPE                  m_mt;
};

#endif
