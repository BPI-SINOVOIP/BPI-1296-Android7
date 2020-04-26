#ifndef IORESOURCE_H_
#define IORESOURCE_H_

#include <io/dvd_path_define.h>
typedef enum
  {
    RESOURCE_DVD_READ=0,
    RESOURCE_DVD_WRITE,
    RESOURCE_HDD_READ,
    RESOURCE_HDD_WRITE,
    RESOURCE_DV_READ,
    RESOURCE_COUNT,   // indicate how many items above
    RESOURCE_NONE     // indicate no resource is used
  } RESOURCE_NAME;

class CIOResource {
 public:
    static bool GetResource(RESOURCE_NAME resource);
    static bool PeekResource(RESOURCE_NAME resource);
    static bool ReleaseResource(RESOURCE_NAME resource);
};





#endif
