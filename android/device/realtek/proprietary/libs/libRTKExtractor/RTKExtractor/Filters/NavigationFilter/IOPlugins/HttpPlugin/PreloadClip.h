////////////////////////////////////////////////////////////////////////////////
// Pre-loading clips.
//
// This class is for HLS to pre-load clips during playback. 
//
// Author: zackchiu@realtek.com
//
// Created: 11/22/2010
////////////////////////////////////////////////////////////////////////////////

#ifndef __PRE_LOAD_CLIP__
#define __PRE_LOAD_CLIP__

#include "HLSPlayList.h"
#include <Filters/NavigationFilter/NavPlugins.h>
#include <map>
#include <list>
#include "CommandQueue.h"
#include "CommandProcessor.h"
#include "OSAL.h"

class PreloadCmdProcessor;
class HLSClip;

////////////////////////////////////////////////////////////////////////////////
// This class is for pre-loading HLS clips.
//
class PreloadClip
{
    public:
        PreloadClip(IOPLUGIN& IoPlugin, const char* pUserAgent);
        ~PreloadClip();
        
        // Pre-load clip
        //
        bool Preload(rtk::parser::PlayListItem* pItem);
        
        // Find and get pre-loaded clip
        //
        HLSClip* GetPreloadItem(rtk::parser::PlayListItem* pItem, bool bRemoveFromList = false);
        
        // Clear all of pre-loaded clips
        //
        // pop out the first item of playlist
		HLSClip* GetFirstPreloadItem();
	   // clear all playlist item, but clip queue is still existed in case of future use.
	   void InvalidateItemQueue();
        
        void ClearPreloadedItem();
        
        // Check if this item is pre-loading.
        //
        //  Return:
        //      true --> This item is pre-loading
        //
        bool IsPreLoading(rtk::parser::PlayListItem* pItem);
        
        // Wait for the specified item pre-loaded
        //
        //  Return:
        //      true --> OK.
        //      false--> Failed and time out.
        //
        bool WaitForPreload(rtk::parser::PlayListItem* pItem);
        bool ItemQueueFull();
        bool ItemQueueEmpty();
    private:
        rtk::tools::CommandQueue<rtk::parser::PlayListItem*>* m_pPreloadCmdList;      
        PreloadCmdProcessor* m_pPreloadCmdProcessor;
		rtk::tools::SafeQueue<rtk::parser::PlayListItem*> *m_ItemQueue;
};

////////////////////////////////////////////////////////////////////////////////
//  This class presents a HLS clip.
//
class HLSClip
{
    public:
        HLSClip(IOPLUGIN& IoPlugin, 
                rtk::parser::PlayListItem* pItem, 
                const char* pUserAgent);

        // Please call this function before using this class.
        // After calling this, it ensures that the instance must be valid.
        //
        UINT AddRef();
        
        // Please call this function to release instance.
        // Don't use delete operator.
        //
        void Release();
        
        // Get the file handle returned by IO plugin's open()
        //
        void* GetHandle();
        
        // Get the original play list item
        //
        rtk::parser::PlayListItem* GetItem();
        
        // Set the file pointer to the begin.
        //
        void SeekToBegin();
        
        // Get the item URL.
        //
        const char* GetURL() const { return m_URL.c_str(); };
        
    private:
        // delete operator is not allowed!!
        // Please release this object by Release();
        //
        ~HLSClip();
        
    private:
        IOPLUGIN& m_IoPlugin;
        void* m_pFileHandle;
        rtk::parser::PlayListItem* m_pItem;
        UINT m_iRefCounter;
        CCritSec m_CritSec;
        std::string m_URL;
};

////////////////////////////////////////////////////////////////////////////////
// Pre-load commands procdessor.
//
class PreloadCmdProcessor: public rtk::tools::CommandProcessor<rtk::parser::PlayListItem*>
{
    public:
        PreloadCmdProcessor(IOPLUGIN& IoPlugin, const char* pUserAgent);
        ~PreloadCmdProcessor();
        
        // Find and get the pre-loaded clip
        //
        //  Parameters:
        //      1. pItem: The play list item for this clip.
        //      2. bRemoveFromList: Remove this item from list when found it.
        //
        HLSClip* Find(rtk::parser::PlayListItem* pItem, bool bRemoveFromList);
        // Find and get the pre-loaded clip after pItem in the queue
        //
        //  Parameters:
        //      1. pItem: The play list item for this clip. If NULL, return the top item.
        //      2. bRemoveFromList: Remove this item from list when found it.
        //

        HLSClip* FindNext(rtk::parser::PlayListItem* pItem, bool bRemoveFromList);

        
        void ClearAllCommands();
        
        // Set sync command. After this item is pre-loaded, the sunc event will
        // be set.
        //
        bool SetSyncComamnd(rtk::parser::PlayListItem* pItem, CEvent* pSyncEvent);
        
    public:
        bool Execute(rtk::parser::PlayListItem* pItem);   
        
    private:
        void ReleaseTopItemFromList();
        
    private:   
        IOPLUGIN& m_IoPlugin;
        std::list<HLSClip*> m_PreloadList;
        std::map<rtk::parser::PlayListItem*, CEvent*> m_CmdSyncList;
        CCritSec m_CritSec;
        std::string m_strUserAgent;
};

class CachedClipList
{
	public:
		CachedClipList(UINT sizeCache);
		~CachedClipList();
		bool AppendItem(HLSClip *pClip, bool noAutoPopItem = false); 
		//void RemoveItem(HLSClip *pClip); 
		// append pClip to end of list if not existed. pop the first item if reaches maxSize
        HLSClip* GetItem(UINT iIndex);		
		void ClearCachedList(); 
		UINT Size() {return m_ClipList.size();}
		// invalidate all cache
	private:
		UINT maxSize;
        std::vector<HLSClip *> m_ClipList;
};

////////////////////////////////////////////////////////////////////////////////
#endif
