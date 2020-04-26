///////////////////////////////////////////////////////////////////////////////
// BDROMNavigationCache.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#ifndef _BDROM_NAVIGATIONCACHE_H_
#define _BDROM_NAVIGATIONCACHE_H_

#include <list>

#include "BDROMIndex.h"
#if !SKIP_NAVCACHE_PLAYLIST
#include "BDROMPlaylist.h"
#endif
#if !SKIP_NAVCACHE_CLIPINFO
#include "BDROMClipInfo.h"
#endif
#include "BDROMSound.h"

#include "BD_FS_Layer.h"
#include "BD_FS_rt_stream.h"

#include "SonicCriticalSection.h"

namespace BDROM {

class NavigationCacheHandle;
typedef ref_ptr<NavigationCacheHandle> NavigationCacheHandleRP;
typedef std::list<NavigationCacheHandleRP> NavigationCacheHandleRPList;

///////////////////////////////////////////////////////////////

class BDROM_NavigationCache
{
public:
  typedef enum
  {
    UNKNOWN, INDEX, PLAYLIST,CLIPINFO, SOUND, SOUNDSTREAM,
     MOVIEOBJECT, BDJO, STREAM, JAR, FONTINDEX, ID, FONTFILE, OTHER
  } NavigationCacheType;

private:
  ref_ptr<FilePath> mBDROMDir;
  ref_ptr<BD_FS_Layer> mBDROMFSLayer;
  BD_FS_DeviceHandle mBDROMDeviceHandle;

  // quotas
  UInt32 mMenuFilesQuota;
  UInt32 mMarkFilesQuota;
  UInt32 mPlayListFilesQuota;
  UInt32 mClipInfoFilesQuota;
  UInt32 mBDJOFilesQuota;
  UInt32 mJarFilesQuota;
  UInt32 mFontIndexFileQuota;
  UInt32 mIdFileQuota;
  UInt32 mFontFileQuota;

  // preload buffer to speed up BPT parsing
  UInt8 * mpPreloadBuffer;

  //NavigationCacheHandleRPList	mCache;	// moved for Descent->Lxg fixing
  SonicCriticalSection			m_cs;

protected:
  BDROM_NavigationCache() {};
  void _Clone (const BDROM_NavigationCache &_a);

  bool _CheckQuota ( NavigationCacheType Type );

public:
  BDROM_NavigationCache(ref_ptr<FilePath> BDROMDir, ref_ptr<BD_FS_Layer> BDROMFSLayer, BD_FS_DeviceHandle BDROMDeviceHandle);
  BDROM_NavigationCache(const BDROM_NavigationCache &_a);
  BDROM_NavigationCache & operator= (const BDROM_NavigationCache &_a);
  ~BDROM_NavigationCache();

  void GetBD_FS_LayerInfo(ref_ptr<BD_FS_Layer> &, BD_FS_DeviceHandle &, FilePath &);
  void QuotaLimits (UInt32 MenuFiles, UInt32 MarkFiles, UInt32 PlayListFiles, UInt32 ClipInfoFiles, UInt32 BDJOFiles,
					UInt32 JarFiles, UInt32 FontIndex, UInt32 IdFile, UInt32 FontFile);

  ref_ptr<NavigationCacheHandle> GetBDROMFile (const unicode::string8 & Filename, NavigationCacheType FileType, bool CreateFile, bool MustSucceed, bool bCantCache = false);
  ref_ptr<FilePath> GetFilePath ( NavigationCacheType FileType, const unicode::string8 & Filename );
  ref_ptr<FilePath> GetStreamFilePath (const unicode::string8 & Filename) {return GetFilePath(STREAM, Filename);}
  void GetAVCHD8dot3Filename(const unicode::string8 & Filename, unicode::string8 & FilenameAvchd83);

  void Clear(); // in the case of a VFS update, cached handles may be obsolete. 

  NavigationCacheHandleRPList	mCache;	// added for Descent->Lxg fixing
};

/////////////////////////////////////////////////////////////////////
class NavigationCacheHandle
{
private:
  BDROM_NavigationCache::NavigationCacheType mFiletype;
  ref_ptr<FilePath> mFile;
  ref_ptr<BD_FS_Layer> mLayer;
  BD_FS_rt_stream *mStream;
  
  ref_ptr<index_bdmv>		mIndex;
#if !SKIP_NAVCACHE_PLAYLIST
  ref_ptr<BDROM::PlayList_mpls>	mPlayList;
#endif
#if !SKIP_NAVCACHE_CLIPINFO
  ref_ptr<ClipInfo_clpi>	mClipInfo;
#endif
  ref_ptr<MovieObject_bdmv>	mMovieObject;
  ref_ptr<BDJObject_bdjo>	mBDJO;
  ref_ptr<sound_bdmv>		mSound;
  ref_ptr<BD_FS_rt_stream>	mAVStream;
  ref_ptr<BD_FS_rt_stream>	mJar;
  ref_ptr<BD_FS_rt_stream>  mFontIndex;
  ref_ptr<BD_FS_rt_stream>	mId;
  ref_ptr<BD_FS_rt_stream>	mFontFile;

  UInt32 mModified;
  BD_FS_DeviceHandle mDeviceHandle;

  bool						mbValid;

  char*						m_pDataBuffer;
  UInt32					m_uiDataBufferSize;

protected:
  NavigationCacheHandle() : mFiletype (BDROM_NavigationCache::UNKNOWN) {};
  void _Clone (const NavigationCacheHandle &_a);

public:
  
  NavigationCacheHandle( BDROM_NavigationCache::NavigationCacheType FileType, ref_ptr<FilePath> File,
		ref_ptr<BD_FS_Layer> BDROMFSLayer, BD_FS_DeviceHandle BDROMDeviceHandle, UInt8 * pPreloadBuffer, UInt32 BlockSize, UInt32 NoBlocks);
  ~NavigationCacheHandle();

  NavigationCacheHandle(const NavigationCacheHandle &_a);
  NavigationCacheHandle & operator= (const NavigationCacheHandle &_a);

  ref_ptr<index_bdmv>		GetIndex();
#if !SKIP_NAVCACHE_PLAYLIST
  ref_ptr<BDROM::PlayList_mpls>	GetPlayList();
#endif
#if !SKIP_NAVCACHE_CLIPINFO
  ref_ptr<ClipInfo_clpi>	GetClipInfo();
#endif
  ref_ptr<MovieObject_bdmv>	GetMovieObject();
  ref_ptr<BDJObject_bdjo>   GetBDJObject();
  ref_ptr<sound_bdmv>		GetSound();
  ref_ptr<BD_FS_rt_stream>	GetClipAVStream();
  ref_ptr<BD_FS_rt_stream>	GetJar();
  ref_ptr<BD_FS_rt_stream>	GetFontIndex();
  ref_ptr<BD_FS_rt_stream>	GetId();
  ref_ptr<BD_FS_rt_stream>	GetFontFile();

  UInt32 SetModified();
  UInt32 GetModified();
  void Flush (bool Discard);
  BDROM_NavigationCache::NavigationCacheType GetFileType();

  bool IsEqual ( const ref_ptr<FilePath> & File);
  bool IsValid(void)								{ return mbValid; }

  bool BufferData(bool bForceReBuffer = false);
  UInt32 GetFileSize(void);
  bool Read(char* pDst, UInt32 uiOffset, UInt32 uiBytes, UInt32 & uiActBytes);
};

} // namespace

#endif
