#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

_DRM_SOURCE_PATH=./source/
_DRM_ROOT_PATH=
_DMP_ROOT_PATH=../../../../../
include ./DrmSetEnvironment.mk

#some compile flags has to be defined first
include ./DrmProfileCompileTimeOptions.mk



rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= $(_DRM_LIB_PATH)libplayready20.a

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) $(_DRM_C_DEFINES) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)

OBJS = source/blackbox/blackbox.o
OBJS += source/certs/drmbcertbuilder.o source/certs/drmbcertparser.o
OBJS += source/cmdlnpars/cmdlnpars.o
OBJS += source/constants/drmactivationconstants.o 
OBJS += source/constants/drmantirollbackclockconstants.o
OBJS += source/constants/drmconstants.o
OBJS += source/constants/drmcontentrevocationconstants.o
OBJS += source/constants/drmdevcertconstants.o
OBJS += source/constants/drmdlaconstants.o
OBJS += source/constants/drmdomainconstants.o
OBJS += source/constants/drmhdrbuilderconstants.o
OBJS += source/constants/drmheaderparserconstants.o
OBJS += source/constants/drmlicacqv3constants.o
OBJS += source/constants/drmlicensesyncconstants.o
OBJS += source/constants/drmmanagerconstants.o
OBJS += source/constants/drmmeterconstants.o
OBJS += source/constants/drmmoveconstants.o
OBJS += source/constants/drmndcertconstants.o
OBJS += source/constants/drmplayreadyobjconstants.o
OBJS += source/constants/drmpubkeydeviceexclusion.o
OBJS += source/constants/drmrevocationconstants.o
OBJS += source/constants/drmsecureclockconstants.o
OBJS += source/constants/drmsoapxmlconstants.o
OBJS += source/constants/drmsymoptconstants.o
OBJS += source/constants/drmtoolsconstants.o
OBJS += source/constants/drmversionconstants.o
OBJS += source/constants/drmviewrightsconstants.o
OBJS += source/constants/drmwmdrmconstants.o
OBJS += source/constants/drmwmdrmnetconstants.o
OBJS += source/constants/drmxmlbuilderconstants.o
OBJS += source/constants/drmxmlparserconstants.o
OBJS += source/constants/drmxmlsigconstants.o
OBJS += source/constants/ndtconstants.o
OBJS += source/core/drmbcrlparser.o
OBJS += source/core/drmblobcache.o
OBJS += source/core/drmchain.o
OBJS += source/core/drmcleanstore.o
OBJS += source/core/drmdebug.o
OBJS += source/core/drmembedding.o
OBJS += source/core/drmhdrbuilder.o
OBJS += source/core/drmint64.o
OBJS += source/core/drmkeygen.o
OBJS += source/core/drmlicacqresp.o
OBJS += source/core/drmlicacqv3.o
OBJS += source/core/drmliceval.o
OBJS += source/core/drmrevocation.o
OBJS += source/core/drmrevocationstore.o
OBJS += source/core/drmsecstore.o
OBJS += source/core/drmsoapxmlutility.o

OBJS += source/devicedevcert/devicedevcert.o
OBJS += source/drmmanager/real/drmmanager.o source/drmmanager/real/drmpolicystate.o
OBJS += source/initiators/drminitparser.o
OBJS += source/keyfile/drmkeyfile.o
OBJS += source/keyfileformat/drmkeyfileformat.o


OBJS += source/modules/activation/stub/drmactivationimpstub.o source/modules/activation/stub/drmactivationutilstub.o
OBJS += source/modules/antirollbackclock/real/antirollbackclockimplreal.o source/modules/antirollbackclock/real/antirollbackclockutilreal.o
OBJS += source/modules/antirollbackclock/stub/antirollbackclockimplstub.o source/modules/antirollbackclock/stub/antirollbackclockutilstub.o
OBJS += source/modules/apppolicy/stub/apppolicyimplstub.o source/modules/apppolicy/stub/apppolicyutilstub.o
OBJS += source/modules/apprevocation/real/apprevocationimplreal.o source/modules/apprevocation/real/apprevocationutilreal.o
OBJS += source/modules/apprevocation/stub/apprevocationimplstub.o source/modules/apprevocation/stub/apprevocationutilstub.o
OBJS += source/modules/base64/real/drmbase64implreal.o
OBJS += source/modules/bytemanip/real/drmbytemanipimplreal.o
OBJS += source/modules/certcache/real/certcachereal.o source/modules/certcache/real/certcacheutilreal.o
OBJS += source/modules/certcache/stub/certcachestub.o source/modules/certcache/stub/certcacheutilstub.o
OBJS += source/modules/certparsercache/real/certparsercacheimplreal.o source/modules/certparsercache/real/certparsercacheutilreal.o
OBJS += source/modules/certparsercache/stub/certparsercacheimplstub.o source/modules/certparsercache/stub/certparsercacheutilstub.o
OBJS += source/modules/chkdebug/real/drmchkdebug.o
OBJS += source/modules/cleanstore/real/cleanstorereal.o source/modules/cleanstore/real/cleanstoreutilreal.o
OBJS += source/modules/cleanstore/stub/cleanstorestub.o source/modules/cleanstore/stub/cleanstoreutilstub.o
OBJS += source/modules/contentrevocation/real/contentrevocationlevlreal.o source/modules/contentrevocation/real/contentrevocationlicreal.o source/modules/contentrevocation/real/contentrevocationutilreal.o
OBJS += source/modules/contentrevocation/stub/contentrevocationlevlstub.o source/modules/contentrevocation/stub/contentrevocationlicstub.o source/modules/contentrevocation/stub/contentrevocationutilstub.o
OBJS += source/modules/contract/real/contractutilreal.o source/modules/contract/real/drmcontract.o
OBJS += source/modules/contract/stub/contractutilstub.o source/modules/contract/stub/drmcontractstub.o
OBJS += source/modules/copyopl/stub/copyoplimplstub.o source/modules/copyopl/stub/copyoplutilstub.o
OBJS += source/modules/crt/drmcrtcommon.o
OBJS += source/modules/crt/real/crtimplreal.o source/modules/crt/real/crtutilreal.o
OBJS += source/modules/crt/stub/crtimplstub.o source/modules/crt/stub/crtutilstub.o
OBJS += source/modules/datastore/real/drmdatastore.o
OBJS += source/modules/deletedstore/real/deletedstoreimplreal.o source/modules/deletedstore/real/deletedstoreutilreal.o
OBJS += source/modules/deletedstore/stub/deletedstoreimplstub.o source/modules/deletedstore/stub/deletedstoreutilstub.o
OBJS += source/modules/devcertkeygen/real/devcertkeygenimplreal.o
OBJS += source/modules/devicerevocation/real/devicerevocationimplreal.o source/modules/devicerevocation/real/devicerevocationutilreal.o
OBJS += source/modules/devicerevocation/stub/devicerevocationimplstub.o source/modules/devicerevocation/stub/devicerevocationutilstub.o
OBJS += source/modules/devicestore/real/devicestoreutilreal.o source/modules/devicestore/real/drmdevicestore.o source/modules/devicestore/real/drmdevstore.o
OBJS += source/modules/devicestore/stub/devicestoreutilstub.o source/modules/devicestore/stub/drmdevicestorestub.o source/modules/devicestore/stub/drmdevstorestub.o
OBJS += source/modules/dla/real/dlautilreal.o source/modules/dla/real/drmdlaimplreal.o
OBJS += source/modules/dla/stub/dlautilstub.o source/modules/dla/stub/drmdlaimplstub.o
OBJS += source/modules/domain/real/drmdomainapi.o source/modules/domain/real/drmdomainimp.o source/modules/domain/real/drmdomainkeyxmrparser.o source/modules/domain/real/drmdomainstore.o
OBJS += source/modules/domain/stub/drmdomainstub.o
OBJS += source/modules/est/real/drmestimplreal.o
OBJS += source/modules/est/stub/drmestimplstub.o
OBJS += source/modules/hds/real/drmhdsimplreal.o source/modules/hds/real/drmhdsutilreal.o
OBJS += source/modules/hds/stub/drmhdsimplstub.o source/modules/hds/stub/drmhdsutilstub.o
OBJS += source/modules/hdsblockheadercache/real/hdsblockheadercacheutilreal.o
OBJS += source/modules/hdsblockheadercache/stub/hdsblockheadercacheutilstub.o
OBJS += source/modules/headerparser/real/drmheaderparser.o
OBJS += source/modules/licensesync/real/drmgensyncchallenge.o source/modules/licensesync/real/drmsyncstore.o source/modules/licensesync/real/licensesyncutilreal.o
OBJS += source/modules/licensesync/stub/drmgensyncchallengestub.o  source/modules/licensesync/stub/drmsyncstorestub.o source/modules/licensesync/stub/licensesyncutilstub.o
OBJS += source/modules/licstore/real/drmlicstore.o
OBJS += source/modules/metercertrevocation/real/metercertrevocationutilreal.o
OBJS += source/modules/metercertrevocation/stub/metercertrevocationutilstub.o
OBJS += source/modules/metering/real/drmmeterapi.o source/modules/metering/real/drmmetercertstore.o source/modules/metering/real/drmmeterimp.o source/modules/metering/real/drmmeterimpv1.o source/modules/metering/real/meteringutilreal.o
OBJS += source/modules/metering/stub/drmmeterapistub.o source/modules/metering/stub/drmmetercertstorestub.o source/modules/metering/stub/drmmeterimpstub.o source/modules/metering/stub/meteringutilstub.o
OBJS += source/modules/modelrevocation/stub/modelrevocationimplstub.o source/modules/modelrevocation/stub/modelrevocationutilstub.o
OBJS += source/modules/move/stub/drmmoveimplstub.o source/modules/move/stub/moveutilstub.o
OBJS += source/modules/noncestore/real/drmnoncestore.o
OBJS += source/modules/performance/real/drmperformancereal.o source/modules/performance/stub/drmperformancestub.o
OBJS += source/modules/playreadyobj/real/drmplayreadyobj.o
OBJS += source/modules/profiling/real/drmprofilingimplreal.o source/modules/profiling/stub/drmprofilingimplstub.o
OBJS += source/modules/removaldatestore/real/drmremovaldatestore.o
OBJS += source/modules/secure_clock/real/secureclockmanager.o source/modules/secure_clock/real/secureclockrequest.o source/modules/secure_clock/real/secureclockresponse.o source/modules/secure_clock/real/secureclockutil.o
OBJS += source/modules/secure_clock/stub/secureclockmanagerstub.o source/modules/secure_clock/stub/secureclockrequeststub.o source/modules/secure_clock/stub/secureclockresponsestub.o source/modules/secure_clock/stub/secureclockutilstub.o
OBJS += source/modules/stkalloc/real/drmstkallocimplreal.o
OBJS += source/modules/strsafe/real/drmstrsafeimplreal.o
OBJS += source/modules/symopt/real/symoptdevicedevcertreal.o source/modules/symopt/real/symoptlacqreal.o source/modules/symopt/real/symoptlicreal.o source/modules/symopt/real/symoptutilreal.o
OBJS += source/modules/symopt/stub/symoptdevicedevcertstub.o source/modules/symopt/stub/symoptlacqstub.o source/modules/symopt/stub/symoptlicstub.o source/modules/symopt/stub/symoptutilstub.o
OBJS += source/modules/thumbnail/stub/thumbnailutilstub.o
OBJS += source/modules/utf/real/drmutf.o
OBJS += source/modules/utilities/crypto/drmutilitiescrypto.o
OBJS += source/modules/utilities/general/drmutilitiesgeneral.o
OBJS += source/modules/viewrights/real/drmviewrightsimplreal.o source/modules/viewrights/real/drmviewrightsutilreal.o
OBJS += source/modules/viewrights/stub/drmviewrightsimplstub.o source/modules/viewrights/stub/drmviewrightsutilstub.o
OBJS += source/modules/wmdrm/real/drmdevcert.o source/modules/wmdrm/real/drmwmdrmexprevalreal.o source/modules/wmdrm/real/drmwmdrmexprfuncreal.o source/modules/wmdrm/real/drmwmdrmexprvariablereal.o source/modules/wmdrm/real/drmwmdrmlicacqrespreal.o source/modules/wmdrm/real/drmwmdrmlicchainreal.o source/modules/wmdrm/real/drmwmdrmlicenseparserreal.o source/modules/wmdrm/real/drmwmdrmlicevalreal.o source/modules/wmdrm/real/drmwmdrmoplreal.o source/modules/wmdrm/real/drmwmdrmutilreal.o
OBJS += source/modules/wmdrm/stub/drmdevcertstub.o source/modules/wmdrm/stub/drmwmdrmexprevalstub.o source/modules/wmdrm/stub/drmwmdrmexprfuncstub.o source/modules/wmdrm/stub/drmwmdrmexprvariablestub.o source/modules/wmdrm/stub/drmwmdrmlicacqrespstub.o source/modules/wmdrm/stub/drmwmdrmlicchainstub.o source/modules/wmdrm/stub/drmwmdrmlicenseparserstub.o source/modules/wmdrm/stub/drmwmdrmlicevalstub.o source/modules/wmdrm/stub/drmwmdrmoplstub.o source/modules/wmdrm/stub/drmwmdrmutilstub.o
OBJS += source/modules/wmdrmnet/real/drmndcertbuilder.o source/modules/wmdrmnet/real/drmnddevicecert.o source/modules/wmdrmnet/real/drmxmlrsakeys.o source/modules/wmdrmnet/real/wmdrmnetimplreal.o source/modules/wmdrmnet/real/wmdrmnetutilreal.o
OBJS += source/modules/wmdrmnet/stub/wmdrmnetimplstub.o source/modules/wmdrmnet/stub/wmdrmnetutilstub.o
OBJS += source/modules/xbinary/real/drmxb.o source/modules/xbinary/real/drmxbbuilder.o source/modules/xbinary/real/drmxbparser.o
OBJS += source/modules/xmlbuilder/crypto/drmxmlbuildercrypto.o
OBJS += source/modules/xmlbuilder/general/drmxmlbuildergeneralansi.o source/modules/xmlbuilder/general/drmxmlbuildergeneralunicode.o
OBJS += source/modules/xmlhash/real/xmlhashimplreal.o source/modules/xmlhash/real/xmlhashutilreal.o
OBJS += source/modules/xmlhash/stub/xmlhashimplstub.o source/modules/xmlhash/stub/xmlhashutilstub.o
OBJS += source/modules/xmlparser/crypto/drmxmlparsercrypto.o
OBJS += source/modules/xmlparser/general/drmxmlparsergeneral.o source/modules/xmlparser/general/drmxmlutilities.o
OBJS += source/modules/xmlsig/builder/drmxmlsigbuilder.o
OBJS += source/modules/xmlsig/general/drmxmlsiggeneral.o		






OBJS += source/ndr/drmndr.o #source/ndr/ndrexample.o
OBJS += source/ndt/drmndtmanager.o source/ndt/drmpfd.o source/ndt/drmpfdmanager.o source/ndt/drmptrlist.o
OBJS += source/ndtasf/asfmediabuffer.o source/ndtasf/asfsample.o source/ndtasf/drmndtasf.o source/ndtasf/drmorderedlist.o source/ndtasf/leakybucket.o source/ndtasf/mux.o source/ndtasf/muxstream.o source/ndtasf/ndtasf_internal.o source/ndtasf/ndtasfdrm.o source/ndtasf/ndtasfguids.o source/ndtasf/packetwriter.o source/ndtasf/sendwindow.o
OBJS += source/ndtmgr/ndtencrypt.o source/ndtmgr/ndtlicense.o source/ndtmgr/ndtmanager_internal.o source/ndtmgr/ndtnetmsg.o source/ndtmgr/ndtreg.o
OBJS += source/oem/ansi/oemcriticalsection.o source/oem/ansi/oemdebug.o source/oem/ansi/oemfileio.o source/oem/ansi/oemimpl.o source/oem/ansi/oeminfo.o source/oem/ansi/oemprofile.o source/oem/ansi/oemtime.o
OBJS += source/oem/ansi/trace/oemtrace.o
OBJS += source/oem/common/AES/aes128.o  source/oem/common/AES/aesbox.o  source/oem/common/AES/oemaes.o  source/oem/common/AES/oemaesmulti.o
OBJS += source/oem/common/bignum/bignum.o source/oem/common/bignum/bignumnoinline.o source/oem/common/bignum/digitogcd.o source/oem/common/bignum/divide.o source/oem/common/bignum/divisibility.o source/oem/common/bignum/ecex2001.o source/oem/common/bignum/ecexpon.o source/oem/common/bignum/ecppq.o source/oem/common/bignum/ecurve.o source/oem/common/bignum/field.o source/oem/common/bignum/kdiv.o source/oem/common/bignum/kinitpr.o source/oem/common/bignum/kinvert.o source/oem/common/bignum/kmuladd.o source/oem/common/bignum/lucas.o source/oem/common/bignum/modexp.o source/oem/common/bignum/modmulch1.o source/oem/common/bignum/modsqrt.o source/oem/common/bignum/modular.o source/oem/common/bignum/mpaddsubcmp.o source/oem/common/bignum/mpalloc.o source/oem/common/bignum/mpgcdex.o source/oem/common/bignum/mpmul22.o source/oem/common/bignum/mprand.o source/oem/common/bignum/multlow.o
OBJS += source/oem/common/cbc/lib/oemcbc.o 

#oemcbcimpl.c is included in oemcbc.c
#OBJS += source/oem/common/cbc/oemcbcimpl.o

OBJS += source/oem/common/cipher/oemcipher.o
OBJS += source/oem/common/des/deskey.o
OBJS += source/oem/common/DevStore/DVS/oemdevstore.o source/oem/common/DevStore/InMemory/oemdevstore.o
OBJS += source/oem/common/ecc/base/oemeccp160.o source/oem/common/ecc/base/oemeccp256.o
OBJS += source/oem/common/ecc/baseimpl/oemeccp160impl.o  source/oem/common/ecc/baseimpl/oemeccp256impl.o
OBJS += source/oem/common/keyfile/oemkeyfile.o
OBJS += source/oem/common/lib/oemimpl_common.o  source/oem/common/lib/oeminfo_common.o  source/oem/common/lib/oemndt.o  source/oem/common/lib/oemxmlhash.o
OBJS += source/oem/common/md5/oemmd5.o
OBJS += source/oem/common/pkcrypto/oempkcrypto.o
OBJS += source/oem/common/profile/oemprofile_common.o
OBJS += source/oem/common/rc4/lib/rc4.o
OBJS += source/oem/common/RSA/base/oemrsa.o source/oem/common/RSA/base/oemrsakeygen.o
OBJS += source/oem/common/RSA/oaeppss/oemrsaoaeppss.o
OBJS += source/oem/common/RSA/oaeppssimpl/oemrsaoaeppssimpl.o
OBJS += source/oem/common/sha/hash/oemshahash.o
OBJS += source/oem/common/sha/hmac/oemshahmac.o
OBJS += source/oem/common/sha/sha1/oemsha1.o
OBJS += source/oem/common/sha/sha256/oemsha256.o



OBJS += source/results/drmerror.o source/results/drmresults.o source/results/drmresultsinc.o

OBJS += source/wmdrmcert/certparser/drmcertparser.o
OBJS += source/xmr/drmxmr.o source/xmr/drmxmrbuilder.o source/xmr/drmxmrparser.o
.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -O -c $< -o $@ 


all:$(LIBNAME)


ifeq ($(__NAV_USE_DLL_PLUGIN__), YES)
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(CC) -shared -o $(LIBNAME) $(OBJS)
else
$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@
endif

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
