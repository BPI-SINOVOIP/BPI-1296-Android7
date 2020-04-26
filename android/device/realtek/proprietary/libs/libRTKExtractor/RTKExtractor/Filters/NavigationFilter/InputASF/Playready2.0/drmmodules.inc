############################################################################## 
#
# Reset DRM_MODULES_LIBS to an empty value
#
##############################################################################
DRM_MODULES_LIBS=


############################################################################## 
#
# ACTIVATION module
#
##############################################################################

!IF "$(DRM_MODULES_ACTIVATION_DISABLE)"=="1"
!IF "$(DRM_MODULES_ACTIVATION_ENABLE)"=="1"
!ERROR DRM_MODULES_ACTIVATION_DISABLE conflicts with DRM_MODULES_ACTIVATION_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_ACTIVATION_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmactivationstub.lib

!IF "$(DRM_MODULES_ACTIVATION_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_ACTIVATION_DISABLED_LIBRARY)         \
!ENDIF

DRM_MODULES_ACTIVATION_ENABLED_LIBRARY=                      \
    $(DRM_PK_SOURCE_LIB_PATH)\drmactivationreal_internal.lib \
    $(DRM_PK_SOURCE_LIB_PATH)\drmactivationreal.lib          \
    
!IF "$(DRM_MODULES_ACTIVATION_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_ACTIVATION_ENABLED_LIBRARY)          \
!ENDIF

############################################################################## 
#
# ANTIROLLBACK_CLOCK module
#
##############################################################################

!IF "$(DRM_MODULES_ANTIROLLBACK_CLOCK_DISABLE)"=="1"
!IF "$(DRM_MODULES_ANTIROLLBACK_CLOCK_ENABLE)"=="1"
!ERROR DRM_MODULES_ANTIROLLBACK_CLOCK_DISABLE conflicts with DRM_MODULES_ANTIROLLBACK_CLOCK_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_ANTIROLLBACK_CLOCK_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmantirollbackclockstub.lib

!IF "$(DRM_MODULES_ANTIROLLBACK_CLOCK_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_ANTIROLLBACK_CLOCK_DISABLED_LIBRARY) \
!ENDIF

DRM_MODULES_ANTIROLLBACK_CLOCK_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmantirollbackclockreal.lib

!IF "$(DRM_MODULES_ANTIROLLBACK_CLOCK_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_ANTIROLLBACK_CLOCK_ENABLED_LIBRARY)  \
!ENDIF

############################################################################## 
#
# APP_POLICY module
#
##############################################################################

!IF "$(DRM_MODULES_APP_POLICY_DISABLE)"=="1"
!IF "$(DRM_MODULES_APP_POLICY_ENABLE)"=="1"
!ERROR DRM_MODULES_APP_POLICY_DISABLE conflicts with DRM_MODULES_APP_POLICY_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_APP_POLICY_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmapppolicystub.lib

!IF "$(DRM_MODULES_APP_POLICY_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_APP_POLICY_DISABLED_LIBRARY)         \
!ENDIF

DRM_MODULES_APP_POLICY_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmapppolicyreal.lib

!IF "$(DRM_MODULES_APP_POLICY_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_APP_POLICY_ENABLED_LIBRARY)          \
!ENDIF

############################################################################## 
#
# APPREVOCATION module
#
##############################################################################

!IF "$(DRM_MODULES_APPREVOCATION_DISABLE)"=="1"
!IF "$(DRM_MODULES_APPREVOCATION_ENABLE)"=="1"
!ERROR DRM_MODULES_APPREVOCATION_DISABLE conflicts with DRM_MODULES_APPREVOCATION_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_APPREVOCATION_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmapprevocationstub.lib

!IF "$(DRM_MODULES_APPREVOCATION_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_APPREVOCATION_DISABLED_LIBRARY)      \
!ENDIF

DRM_MODULES_APPREVOCATION_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmapprevocationreal.lib

!IF "$(DRM_MODULES_APPREVOCATION_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_APPREVOCATION_ENABLED_LIBRARY)       \
!ENDIF

############################################################################## 
#
# CERTCACHE module
#
##############################################################################

!IF "$(DRM_MODULES_CERTCACHE_DISABLE)"=="1"
!IF "$(DRM_MODULES_CERTCACHE_ENABLE)"=="1"
!ERROR DRM_MODULES_CERTCACHE_DISABLE conflicts with DRM_MODULES_CERTCACHE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_CERTCACHE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcertcachestub.lib

!IF "$(DRM_MODULES_CERTCACHE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CERTCACHE_DISABLED_LIBRARY)          \
!ENDIF

DRM_MODULES_CERTCACHE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcertcachereal.lib

!IF "$(DRM_MODULES_CERTCACHE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CERTCACHE_ENABLED_LIBRARY)           \
!ENDIF

############################################################################## 
#
# CERTPARSERCACHE module
#
##############################################################################

!IF "$(DRM_MODULES_CERTPARSERCACHE_DISABLE)"=="1"
!IF "$(DRM_MODULES_CERTPARSERCACHE_ENABLE)"=="1"
!ERROR DRM_MODULES_CERTPARSERCACHE_DISABLE conflicts with DRM_MODULES_CERTPARSERCACHE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_CERTPARSERCACHE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcertparsercachestub.lib

!IF "$(DRM_MODULES_CERTPARSERCACHE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CERTPARSERCACHE_DISABLED_LIBRARY)    \
!ENDIF

DRM_MODULES_CERTPARSERCACHE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcertparsercachereal.lib

!IF "$(DRM_MODULES_CERTPARSERCACHE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CERTPARSERCACHE_ENABLED_LIBRARY)     \
!ENDIF

############################################################################## 
#
# CLEANSTORE module
#
##############################################################################

!IF "$(DRM_MODULES_CLEANSTORE_DISABLE)"=="1"
!IF "$(DRM_MODULES_CLEANSTORE_ENABLE)"=="1"
!ERROR DRM_MODULES_CLEANSTORE_DISABLE conflicts with DRM_MODULES_CLEANSTORE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_CLEANSTORE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcleanstorestub.lib

!IF "$(DRM_MODULES_CLEANSTORE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CLEANSTORE_DISABLED_LIBRARY)         \
!ENDIF

DRM_MODULES_CLEANSTORE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcleanstorereal.lib

!IF "$(DRM_MODULES_CLEANSTORE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CLEANSTORE_ENABLED_LIBRARY)          \
!ENDIF

############################################################################## 
#
# CONTENTREVOCATION module
#
##############################################################################

!IF "$(DRM_MODULES_CONTENTREVOCATION_DISABLE)"=="1"
!IF "$(DRM_MODULES_CONTENTREVOCATION_ENABLE)"=="1"
!ERROR DRM_MODULES_CONTENTREVOCATION_DISABLE conflicts with DRM_MODULES_CONTENTREVOCATION_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_CONTENTREVOCATION_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcontentrevocationstub.lib

!IF "$(DRM_MODULES_CONTENTREVOCATION_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CONTENTREVOCATION_DISABLED_LIBRARY)  \
!ENDIF

DRM_MODULES_CONTENTREVOCATION_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcontentrevocationreal.lib

!IF "$(DRM_MODULES_CONTENTREVOCATION_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CONTENTREVOCATION_ENABLED_LIBRARY)   \
!ENDIF

############################################################################## 
#
# COPYOPL module
#
##############################################################################

!IF "$(DRM_MODULES_COPYOPL_DISABLE)"=="1"
!IF "$(DRM_MODULES_COPYOPL_ENABLE)"=="1"
!ERROR DRM_MODULES_COPYOPL_DISABLE conflicts with DRM_MODULES_COPYOPL_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_COPYOPL_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcopyoplstub.lib

!IF "$(DRM_MODULES_COPYOPL_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_COPYOPL_DISABLED_LIBRARY)            \
!ENDIF

DRM_MODULES_COPYOPL_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcopyoplreal.lib

!IF "$(DRM_MODULES_COPYOPL_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_COPYOPL_ENABLED_LIBRARY)             \
!ENDIF

############################################################################## 
#
# CONTRACT module
#
##############################################################################

!IF "$(DRM_MODULES_CONTRACT_DISABLE)"=="1"
!IF "$(DRM_MODULES_CONTRACT_ENABLE)"=="1"
!ERROR DRM_MODULES_CONTRACT_DISABLE conflicts with DRM_MODULES_CONTRACT_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_CONTRACT_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcontractstub.lib

!IF "$(DRM_MODULES_CONTRACT_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CONTRACT_DISABLED_LIBRARY)           \
!ENDIF

DRM_MODULES_CONTRACT_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcontractreal.lib

!IF "$(DRM_MODULES_CONTRACT_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CONTRACT_ENABLED_LIBRARY)            \
!ENDIF

############################################################################## 
#
# CRT module
#
##############################################################################

!IF "$(DRM_MODULES_CRT_DISABLE)"=="1"
!IF "$(DRM_MODULES_CRT_ENABLE)"=="1"
!ERROR DRM_MODULES_CRT_DISABLE conflicts with DRM_MODULES_CRT_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_CRT_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcrtstub.lib

!IF "$(DRM_MODULES_CRT_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CRT_DISABLED_LIBRARY)                \
!ENDIF

DRM_MODULES_CRT_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmcrtreal.lib

!IF "$(DRM_MODULES_CRT_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_CRT_ENABLED_LIBRARY)                 \
!ENDIF

############################################################################## 
#
# DELETEDSTORE module
#
##############################################################################

!IF "$(DRM_MODULES_DELETEDSTORE_DISABLE)"=="1"
!IF "$(DRM_MODULES_DELETEDSTORE_ENABLE)"=="1"
!ERROR DRM_MODULES_DELETEDSTORE_DISABLE conflicts with DRM_MODULES_DELETEDSTORE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_DELETEDSTORE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdeletedstorestub.lib

!IF "$(DRM_MODULES_DELETEDSTORE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DELETEDSTORE_DISABLED_LIBRARY)       \
!ENDIF

DRM_MODULES_DELETEDSTORE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdeletedstorereal.lib

!IF "$(DRM_MODULES_DELETEDSTORE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DELETEDSTORE_ENABLED_LIBRARY)        \
!ENDIF

############################################################################## 
#
# DEVCERTKEYGEN module
#
##############################################################################

!IF "$(DRM_MODULES_DEVCERTKEYGEN_DISABLE)"=="1"
!IF "$(DRM_MODULES_DEVCERTKEYGEN_ENABLE)"=="1"
!ERROR DRM_MODULES_DEVCERTKEYGEN_DISABLE conflicts with DRM_MODULES_DEVCERTKEYGEN_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_DEVCERTKEYGEN_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdevcertkeygenstub.lib

!IF "$(DRM_MODULES_DEVCERTKEYGEN_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DEVCERTKEYGEN_DISABLED_LIBRARY)      \
!ENDIF

DRM_MODULES_DEVCERTKEYGEN_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdevcertkeygenreal.lib

!IF "$(DRM_MODULES_DEVCERTKEYGEN_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DEVCERTKEYGEN_ENABLED_LIBRARY)       \
!ENDIF

############################################################################## 
#
# DEVICEREVOCATION module
#
##############################################################################

!IF "$(DRM_MODULES_DEVICEREVOCATION_DISABLE)"=="1"
!IF "$(DRM_MODULES_DEVICEREVOCATION_ENABLE)"=="1"
!ERROR DRM_MODULES_DEVICEREVOCATION_DISABLE conflicts with DRM_MODULES_DEVICEREVOCATION_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_DEVICEREVOCATION_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdevicerevocationstub.lib

!IF "$(DRM_MODULES_DEVICEREVOCATION_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DEVICEREVOCATION_DISABLED_LIBRARY)   \
!ENDIF

DRM_MODULES_DEVICEREVOCATION_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdevicerevocationreal.lib

!IF "$(DRM_MODULES_DEVICEREVOCATION_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DEVICEREVOCATION_ENABLED_LIBRARY)    \
!ENDIF

############################################################################## 
#
# DEVICESTORE module
#
##############################################################################

!IF "$(DRM_MODULES_DEVICESTORE_DISABLE)"=="1"
!IF "$(DRM_MODULES_DEVICESTORE_ENABLE)"=="1"
!ERROR DRM_MODULES_DEVICESTORE_DISABLE conflicts with DRM_MODULES_DEVICESTORE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_DEVICESTORE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdevicestorestub.lib

!IF "$(DRM_MODULES_DEVICESTORE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DEVICESTORE_DISABLED_LIBRARY)        \
!ENDIF

DRM_MODULES_DEVICESTORE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdevicestorereal.lib

!IF "$(DRM_MODULES_DEVICESTORE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DEVICESTORE_ENABLED_LIBRARY)         \
!ENDIF

############################################################################## 
#
# DLA module
#
##############################################################################

!IF "$(DRM_MODULES_DLA_DISABLE)"=="1"
!IF "$(DRM_MODULES_DLA_ENABLE)"=="1"
!ERROR DRM_MODULES_DLA_DISABLE conflicts with DRM_MODULES_DLA_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_DLA_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdlastub.lib

!IF "$(DRM_MODULES_DLA_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DLA_DISABLED_LIBRARY)                \
!ENDIF

DRM_MODULES_DLA_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdlareal.lib

!IF "$(DRM_MODULES_DLA_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DLA_ENABLED_LIBRARY)                 \
!ENDIF

############################################################################## 
#
# DOMAIN module
#
##############################################################################

!IF "$(DRM_MODULES_DOMAIN_DISABLE)"=="1"
!IF "$(DRM_MODULES_DOMAIN_ENABLE)"=="1"
!ERROR DRM_MODULES_DOMAIN_DISABLE conflicts with DRM_MODULES_DOMAIN_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_DOMAIN_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdomainstub.lib

!IF "$(DRM_MODULES_DOMAIN_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DOMAIN_DISABLED_LIBRARY)             \
!ENDIF

DRM_MODULES_DOMAIN_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdomainreal.lib

!IF "$(DRM_MODULES_DOMAIN_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_DOMAIN_ENABLED_LIBRARY)              \
!ENDIF

############################################################################## 
#
# EST module
#
##############################################################################

!IF "$(DRM_MODULES_EST_DISABLE)"=="1"
!IF "$(DRM_MODULES_EST_ENABLE)"=="1"
!ERROR DRM_MODULES_EST_DISABLE conflicts with DRM_MODULES_EST_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_EST_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmeststub.lib

!IF "$(DRM_MODULES_EST_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_EST_DISABLED_LIBRARY)                \
!ENDIF

DRM_MODULES_EST_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmestreal.lib

!IF "$(DRM_MODULES_EST_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_EST_ENABLED_LIBRARY)                 \
!ENDIF

############################################################################## 
#
# HDS module
#
##############################################################################

!IF "$(DRM_MODULES_HDS_DISABLE)"=="1"
!IF "$(DRM_MODULES_HDS_ENABLE)"=="1"
!ERROR DRM_MODULES_HDS_DISABLE conflicts with DRM_MODULES_HDS_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_HDS_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmhdsstub.lib

!IF "$(DRM_MODULES_HDS_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_HDS_DISABLED_LIBRARY)                \
!ENDIF

DRM_MODULES_HDS_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmhdsreal.lib

!IF "$(DRM_MODULES_HDS_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_HDS_ENABLED_LIBRARY)                 \
!ENDIF

############################################################################## 
#
# HDSBLOCKHEADERCACHE module
#
##############################################################################

!IF "$(DRM_MODULES_HDSBLOCKHEADERCACHE_DISABLE)"=="1"
!IF "$(DRM_MODULES_HDSBLOCKHEADERCACHE_ENABLE)"=="1"
!ERROR DRM_MODULES_HDSBLOCKHEADERCACHE_DISABLE conflicts with DRM_MODULES_HDSBLOCKHEADERCACHE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_HDSBLOCKHEADERCACHE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmhdsblockheadercachestub.lib

!IF "$(DRM_MODULES_HDSBLOCKHEADERCACHE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_HDSBLOCKHEADERCACHE_DISABLED_LIBRARY)\
!ENDIF

DRM_MODULES_HDSBLOCKHEADERCACHE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmhdsblockheadercachereal.lib

!IF "$(DRM_MODULES_HDSBLOCKHEADERCACHE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_HDSBLOCKHEADERCACHE_ENABLED_LIBRARY) \
!ENDIF

############################################################################## 
#
# LICENSE_SYNC module
#
##############################################################################

!IF "$(DRM_MODULES_LICENSE_SYNC_DISABLE)"=="1"
!IF "$(DRM_MODULES_LICENSE_SYNC_ENABLE)"=="1"
!ERROR DRM_MODULES_LICENSE_SYNC_DISABLE conflicts with DRM_MODULES_LICENSE_SYNC_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_LICENSE_SYNC_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmlicensesyncstub.lib

!IF "$(DRM_MODULES_LICENSE_SYNC_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_LICENSE_SYNC_DISABLED_LIBRARY)       \
!ENDIF

DRM_MODULES_LICENSE_SYNC_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmlicensesyncreal.lib

!IF "$(DRM_MODULES_LICENSE_SYNC_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_LICENSE_SYNC_ENABLED_LIBRARY)        \
!ENDIF

############################################################################## 
#
# METERCERTREVOCATION module
#
##############################################################################

!IF "$(DRM_MODULES_METERCERTREVOCATION_DISABLE)"=="1"
!IF "$(DRM_MODULES_METERCERTREVOCATION_ENABLE)"=="1"
!ERROR DRM_MODULES_METERCERTREVOCATION_DISABLE conflicts with DRM_MODULES_METERCERTREVOCATION_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_METERCERTREVOCATION_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmetercertrevocationstub.lib

!IF "$(DRM_MODULES_METERCERTREVOCATION_DISABLE)"=="1"
DRM_MODULES_LIBS=                                       \
    $(DRM_MODULES_LIBS)                                 \
    $(DRM_MODULES_METERCERTREVOCATION_DISABLED_LIBRARY) \
!ENDIF

DRM_MODULES_METERCERTREVOCATION_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmetercertrevocationreal.lib

!IF "$(DRM_MODULES_METERCERTREVOCATION_ENABLE)"=="1"
DRM_MODULES_LIBS=                                       \
    $(DRM_MODULES_LIBS)                                 \
    $(DRM_MODULES_METERCERTREVOCATION_ENABLED_LIBRARY)  \
!ENDIF

############################################################################## 
#
# METERING module
#
##############################################################################

!IF "$(DRM_MODULES_METERING_DISABLE)"=="1"
!IF "$(DRM_MODULES_METERING_ENABLE)"=="1"
!ERROR DRM_MODULES_METERING_DISABLE conflicts with DRM_MODULES_METERING_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_METERING_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmeteringstub.lib

!IF "$(DRM_MODULES_METERING_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_METERING_DISABLED_LIBRARY)           \
!ENDIF

DRM_MODULES_METERING_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmeteringreal.lib

!IF "$(DRM_MODULES_METERING_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_METERING_ENABLED_LIBRARY)            \
!ENDIF

############################################################################## 
#
# MODELREVOCATION module
#
##############################################################################

!IF "$(DRM_MODULES_MODELREVOCATION_DISABLE)"=="1"
!IF "$(DRM_MODULES_MODELREVOCATION_ENABLE)"=="1"
!ERROR DRM_MODULES_MODELREVOCATION_DISABLE conflicts with DRM_MODULES_MODELREVOCATION_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_MODELREVOCATION_DISABLED_LIBRARY=          \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmodelrevocationstub.lib

!IF "$(DRM_MODULES_MODELREVOCATION_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_MODELREVOCATION_DISABLED_LIBRARY)    \
!ENDIF

DRM_MODULES_MODELREVOCATION_ENABLED_LIBRARY=           \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmodelrevocationreal.lib

!IF "$(DRM_MODULES_MODELREVOCATION_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_MODELREVOCATION_ENABLED_LIBRARY)     \
!ENDIF

############################################################################## 
#
# MOVE module
#
##############################################################################

!IF "$(DRM_MODULES_MOVE_DISABLE)"=="1"
!IF "$(DRM_MODULES_MOVE_ENABLE)"=="1"
!ERROR DRM_MODULES_MOVE_DISABLE conflicts with DRM_MODULES_MOVE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_MOVE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmovestub.lib

!IF "$(DRM_MODULES_MOVE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_MOVE_DISABLED_LIBRARY)               \
!ENDIF

DRM_MODULES_MOVE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmmovereal.lib

!IF "$(DRM_MODULES_MOVE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_MOVE_ENABLED_LIBRARY)                \
!ENDIF

############################################################################## 
#
# PERFORMANCE module
#
##############################################################################

!IF "$(DRM_MODULES_PERFORMANCE_DISABLE)"=="1"
!IF "$(DRM_MODULES_PERFORMANCE_ENABLE)"=="1"
!ERROR DRM_MODULES_PERFORMANCE_DISABLE conflicts with DRM_MODULES_PERFORMANCE_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_PERFORMANCE_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmperformancestub.lib

!IF "$(DRM_MODULES_PERFORMANCE_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_PERFORMANCE_DISABLED_LIBRARY)        \
!ENDIF

DRM_MODULES_PERFORMANCE_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmperformancereal.lib

!IF "$(DRM_MODULES_PERFORMANCE_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_PERFORMANCE_ENABLED_LIBRARY)         \
!ENDIF

############################################################################## 
#
# PROFILING module
#
##############################################################################

!IF "$(DRM_MODULES_PROFILING_DISABLE)"=="1"
!IF "$(DRM_MODULES_PROFILING_ENABLE)"=="1"
!ERROR DRM_MODULES_PROFILING_DISABLE conflicts with DRM_MODULES_PROFILING_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_PROFILING_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmprofilingstub.lib

!IF "$(DRM_MODULES_PROFILING_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_PROFILING_DISABLED_LIBRARY)          \
!ENDIF

DRM_MODULES_PROFILING_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmprofilingreal.lib

!IF "$(DRM_MODULES_PROFILING_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_PROFILING_ENABLED_LIBRARY)           \
!ENDIF

############################################################################## 
#
# SECURE_CLOCK module
#
##############################################################################

!IF "$(DRM_MODULES_SECURE_CLOCK_DISABLE)"=="1"
!IF "$(DRM_MODULES_SECURE_CLOCK_ENABLE)"=="1"
!ERROR DRM_MODULES_SECURE_CLOCK_DISABLE conflicts with DRM_MODULES_SECURE_CLOCK_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_SECURE_CLOCK_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmsecureclockstub.lib

!IF "$(DRM_MODULES_SECURE_CLOCK_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_SECURE_CLOCK_DISABLED_LIBRARY)       \
!ENDIF

DRM_MODULES_SECURE_CLOCK_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmsecureclockreal.lib

!IF "$(DRM_MODULES_SECURE_CLOCK_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_SECURE_CLOCK_ENABLED_LIBRARY)        \
!ENDIF

############################################################################## 
#
# SYMOPT module
#
##############################################################################

!IF "$(DRM_MODULES_SYMOPT_DISABLE)"=="1"
!IF "$(DRM_MODULES_SYMOPT_ENABLE)"=="1"
!ERROR DRM_MODULES_SYMOPT_DISABLE conflicts with DRM_MODULES_SYMOPT_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_SYMOPT_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmsymoptstub.lib

!IF "$(DRM_MODULES_SYMOPT_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_SYMOPT_DISABLED_LIBRARY)             \
!ENDIF

DRM_MODULES_SYMOPT_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmsymoptreal.lib

!IF "$(DRM_MODULES_SYMOPT_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_SYMOPT_ENABLED_LIBRARY)              \
!ENDIF

############################################################################## 
#
# THUMBNAIL module
#
##############################################################################

!IF "$(DRM_MODULES_THUMBNAIL_DISABLE)"=="1"
!IF "$(DRM_MODULES_THUMBNAIL_ENABLE)"=="1"
!ERROR DRM_MODULES_THUMBNAIL_DISABLE conflicts with DRM_MODULES_THUMBNAIL_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_THUMBNAIL_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmthumbnailstub.lib

!IF "$(DRM_MODULES_THUMBNAIL_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_THUMBNAIL_DISABLED_LIBRARY)          \
!ENDIF

DRM_MODULES_THUMBNAIL_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmthumbnailreal.lib

!IF "$(DRM_MODULES_THUMBNAIL_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_THUMBNAIL_ENABLED_LIBRARY)           \
!ENDIF

############################################################################## 
#
# VIEWRIGHTS module
#
##############################################################################

!IF "$(DRM_MODULES_VIEWRIGHTS_DISABLE)"=="1"
!IF "$(DRM_MODULES_VIEWRIGHTS_ENABLE)"=="1"
!ERROR DRM_MODULES_VIEWRIGHTS_DISABLE conflicts with DRM_MODULES_VIEWRIGHTS_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_VIEWRIGHTS_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmviewrightsstub.lib

!IF "$(DRM_MODULES_VIEWRIGHTS_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_VIEWRIGHTS_DISABLED_LIBRARY)         \
!ENDIF

DRM_MODULES_VIEWRIGHTS_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmviewrightsreal.lib

!IF "$(DRM_MODULES_VIEWRIGHTS_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_VIEWRIGHTS_ENABLED_LIBRARY)          \
!ENDIF

############################################################################## 
#
# WMDRM module
#
##############################################################################

!IF "$(DRM_MODULES_WMDRM_DISABLE)"=="1"
!IF "$(DRM_MODULES_WMDRM_ENABLE)"=="1"
!ERROR DRM_MODULES_WMDRM_DISABLE conflicts with DRM_MODULES_WMDRM_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_WMDRM_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmwmdrmstub.lib

!IF "$(DRM_MODULES_WMDRM_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_WMDRM_DISABLED_LIBRARY)              \
!ENDIF

DRM_MODULES_WMDRM_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmwmdrmreal.lib

!IF "$(DRM_MODULES_WMDRM_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_WMDRM_ENABLED_LIBRARY)               \
!ENDIF

############################################################################## 
#
# WMDRMNET module
#
##############################################################################

!IF "$(DRM_MODULES_WMDRMNET_DISABLE)"=="1"
!IF "$(DRM_MODULES_WMDRMNET_ENABLE)"=="1"
!ERROR DRM_MODULES_WMDRMNET_DISABLE conflicts with DRM_MODULES_WMDRMNET_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_WMDRMNET_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmwmdrmnetstub.lib

!IF "$(DRM_MODULES_WMDRMNET_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_WMDRMNET_DISABLED_LIBRARY)           \
!ENDIF

DRM_MODULES_WMDRMNET_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmwmdrmnetreal.lib

!IF "$(DRM_MODULES_WMDRMNET_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_WMDRMNET_ENABLED_LIBRARY)            \
!ENDIF

############################################################################## 
#
# XMLHASH module
#
##############################################################################

!IF "$(DRM_MODULES_XMLHASH_DISABLE)"=="1"
!IF "$(DRM_MODULES_XMLHASH_ENABLE)"=="1"
!ERROR DRM_MODULES_XMLHASH_DISABLE conflicts with DRM_MODULES_XMLHASH_ENABLE
!ENDIF
!ENDIF

DRM_MODULES_XMLHASH_DISABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlhashstub.lib

!IF "$(DRM_MODULES_XMLHASH_DISABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_XMLHASH_DISABLED_LIBRARY)            \
!ENDIF

DRM_MODULES_XMLHASH_ENABLED_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlhashreal.lib

!IF "$(DRM_MODULES_XMLHASH_ENABLE)"=="1"
DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_XMLHASH_ENABLED_LIBRARY)             \
!ENDIF

############################################################################## 
#
# Modules without multiple versions
#
##############################################################################

DRM_MODULES_BASE64_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmbase64real.lib

DRM_MODULES_BYTEMANIP_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmbytemanipreal.lib

DRM_MODULES_CHKDEBUG_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmchkdebugreal.lib

DRM_MODULES_DATASTORE_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmdatastorereal.lib

DRM_MODULES_HEADERPARSER_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmheaderparserreal.lib

DRM_MODULES_LICSTORE_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmlicstorereal.lib

DRM_MODULES_NONCESTORE_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmnoncestorereal.lib

DRM_MODULES_PLAYREADYOBJ_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmplayreadyobjreal.lib

DRM_MODULES_REMOVALDATESTORE_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmremovaldatestorereal.lib

DRM_MODULES_STKALLOC_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmstkallocreal.lib

DRM_MODULES_STRSAFE_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmstrsafereal.lib

DRM_MODULES_UTF_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmutfreal.lib

DRM_MODULES_UTILITIES_GENERAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmutilitiesgeneral.lib

DRM_MODULES_UTILITIES_CRYPTO_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmutilitiescrypto.lib

DRM_MODULES_XBINARY_REAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxbinaryreal.lib

DRM_MODULES_XMLBUILDER_GENERAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlbuildergeneral.lib

DRM_MODULES_XMLBUILDER_CRYPTO_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlbuildercrypto.lib

DRM_MODULES_XMLPARSER_GENERAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlparsergeneral.lib

DRM_MODULES_XMLPARSER_CRYPTO_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlparsercrypto.lib

DRM_MODULES_XMLSIG_GENERAL_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlsiggeneral.lib

DRM_MODULES_XMLSIG_BUILDER_LIBRARY= \
    $(DRM_PK_SOURCE_LIB_PATH)\drmxmlsigbuilder.lib

DRM_MODULES_LIBS=                                      \
    $(DRM_MODULES_LIBS)                                \
    $(DRM_MODULES_BASE64_REAL_LIBRARY)                 \
    $(DRM_MODULES_BYTEMANIP_REAL_LIBRARY)              \
    $(DRM_MODULES_CHKDEBUG_REAL_LIBRARY)               \
    $(DRM_MODULES_DATASTORE_REAL_LIBRARY)              \
    $(DRM_MODULES_HEADERPARSER_REAL_LIBRARY)           \
    $(DRM_MODULES_LICSTORE_REAL_LIBRARY)               \
    $(DRM_MODULES_NONCESTORE_REAL_LIBRARY)             \
    $(DRM_MODULES_PLAYREADYOBJ_REAL_LIBRARY)           \
    $(DRM_MODULES_REMOVALDATESTORE_REAL_LIBRARY)       \
    $(DRM_MODULES_STKALLOC_REAL_LIBRARY)               \
    $(DRM_MODULES_STRSAFE_REAL_LIBRARY)                \
    $(DRM_MODULES_UTF_REAL_LIBRARY)                    \
    $(DRM_MODULES_UTILITIES_GENERAL_LIBRARY)           \
    $(DRM_MODULES_UTILITIES_CRYPTO_LIBRARY)            \
    $(DRM_MODULES_XBINARY_REAL_LIBRARY)                \
    $(DRM_MODULES_XMLBUILDER_GENERAL_LIBRARY)          \
    $(DRM_MODULES_XMLBUILDER_CRYPTO_LIBRARY)           \
    $(DRM_MODULES_XMLPARSER_GENERAL_LIBRARY)           \
    $(DRM_MODULES_XMLPARSER_CRYPTO_LIBRARY)            \
    $(DRM_MODULES_XMLSIG_GENERAL_LIBRARY)              \
    $(DRM_MODULES_XMLSIG_BUILDER_LIBRARY)              \

