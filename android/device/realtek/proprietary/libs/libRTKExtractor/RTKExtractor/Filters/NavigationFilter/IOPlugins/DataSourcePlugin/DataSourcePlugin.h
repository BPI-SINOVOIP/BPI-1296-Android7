#ifndef __DUMMYFILESET_IO_PLUGIN_H__
#define __DUMMYFILESET_IO_PLUGIN_H__

//#include "../IOPlugins.h"
#include <Filters/NavigationFilter/NavPlugins.h>
#include <media/stagefright/DataSource.h>

namespace android {
#ifdef __cplusplus
extern "C" {
#endif

HRESULT openIOPlugin_DataSource(IOPLUGIN* pIOPlugin);

HRESULT IOPlugin_DataSource_SetDataSource(void* pInstance, sp<DataSource> source);

#ifdef __cplusplus
}
#endif

}//namespace android

#endif /*__DUMMYFILE_IO_PLUGIN_H__*/
