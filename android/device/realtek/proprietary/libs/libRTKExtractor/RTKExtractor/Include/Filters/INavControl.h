#ifndef __I_NAV_CONTROL_H__
#define __I_NAV_CONTROL_H__

//
//  INavControl interface
//  -----------------------
//  A generic interface to send commands to navigation filter.
//

#include "NavigationFilter/NavPlugins.h"

#include <utils/RefBase.h>

namespace android {

class INavControl
 :
 public RefBase
{
public:

    //
    // LoadMedia - initialize playback target.
    //
    //     @path - path to the playback target
    //
    virtual HRESULT LoadMedia(
        char* path,
        NAVMEDIAINFO* pMediaInfo=NULL,
        ENUM_MEDIA_TYPE preDeterminedType=MEDIATYPE_None,
        bool* pbContinue=NULL,
        bool bForcedIdentification = false,
        NAVLOADFAILURE* pFailure = NULL,
        fpGetAuthInfo getAuthInfo = NULL,
        bool bRetryIOPlugin = false)=0;

    // UnloadMedia - uninitialize playback target.
    virtual HRESULT UnloadMedia()=0;

    //
    // SetGetProperty - generic interface for property set/get
    //
    virtual HRESULT SetGetProperty(
        NAV_PROP_ID id,
        void* inData,
        unsigned int inDataSize,
        void* outData,
        unsigned int outDataSize,
        unsigned int* returnedSize)=0;

    //
    // GetPlaybackStatus - get current playback status (location, speed, ... etc)
    //
    virtual HRESULT GetPlaybackStatus(
        NAVPLAYBACKSTATUS* status)=0;

    //
    // GetVideoStatus - get video status (available streams, attributes, ... etc)
    //
    virtual HRESULT GetVideoStatus(
        NAVVIDEOSTATUS* status)=0;

    //
    // GetAudioStatus - get audio status (available streams, attributes, ... etc)
    //
    virtual HRESULT GetAudioStatus(
        NAVAUDIOSTATUS* status)=0;

    //
    // GetSubpictureStatus - get subpicture status (available streams, attributes, ... etc)
    //
    virtual HRESULT GetSubpictureStatus(
        NAVSPICSTATUS* status)=0;

    //
    // GetMenuStatus
    //
    virtual HRESULT GetMenuStatus(
        NAVMENUSTATUS* status)=0;

    //
    // GetDiscStatus
    //
    virtual HRESULT GetDiscStatus(
        NAVDISCSTATUS* status)=0;

    //
    // GetTitleStatus
    //
    virtual HRESULT GetTitleStatus(
        int titleNum,
        NAVTITLESTATUS* status)=0;

    //
    // GetLatestCmdStatus
    //
    virtual HRESULT GetLatestCmdStatus(
        NAVCMDSTATUS* status)=0;

    //
    // GetNavState - get a snapshot of current navigator state
    //
    //     @state - buffer for returning snapshot of state, format defined by individual navigators
    //     @size - size of buffer
    //     @returnedSize - the actual size of snapshot returned
    //
    virtual HRESULT GetNavState(
        void* state,
        unsigned int size,
        unsigned int* returnedSize)=0;

    //
    // SetNavState - loading navigator state from a specific snapshot.
    //
    //     @state - snapshot of state, format defined by individual navigators
    //     @size - size of snapshot.
    //
    virtual HRESULT SetNavState(
        void* state,
        unsigned int size,
        bool bForced=false)=0;

    //
    // PlayTitle - play from beginning of a title.
    //
    //     @titleNum - title number (-1: replay current title, or 1~N)
    //                 (N = number of titles on disc)
    //     @cmdID - command identifier (used to check completion)
    //              (all other cmdID in INavControl has the same meaning)
    //
    virtual HRESULT PlayTitle(
        int titleNum,
        unsigned int* cmdID)=0;

    //
    // PlayChapter - play from beginning of a chapter.
    //
    //     @titleNum - title number (-1: current title, or 1~N)
    //                 (N = number of titles on disc)
    //     @chapterNum - chapter number (-1: replay current chapter, or 1~N)
    //                   (N = number of chapters in title)
    //
    virtual HRESULT PlayChapter(
        int titleNum,
        int chapterNum,
        unsigned int* cmdID)=0;

    // PlayNextChapter, PlayPrevChapter - play the neighbor chapters.
    virtual HRESULT PlayNextChapter(
        unsigned int* cmdID)=0;

    virtual HRESULT PlayPrevChapter(
        unsigned int* cmdID)=0;

    //
    // PlayAtTime - play from a position specified by time.
    //
    //     @titleNum - title number (-1: current title, or 1~N)
    //                 (N = number of titles on disc)
    //     @startSeconds,startFrameIdx - play start time in seconds/frameIndex
    //
    virtual HRESULT PlayAtTime(
        int titleNum,
        unsigned int startSeconds,
        unsigned int startFrameIdx,
        unsigned int* cmdID)=0;

	 virtual HRESULT PlayAutoChapter(
        int titleNum,
        int chapterNum,
        unsigned int* cmdID)=0;

    //
    // PlayAtPos - play from a position specified by offset. for MPEG2 PS/TS only
    //
    //     @titleNum - title number (-1: current title, or 1~N)
    //                 (N = number of titles on disc)
    //     @pos - play start position 
    //
#ifndef DISABLEPLAYATPOS
    virtual HRESULT PlayAtPos(
        int titleNum,
        long long pos,
        unsigned int* cmdID)=0;
#endif

    //
    // PlaySegment - play a segment within a title.
    //
    //     @titleNum - title number (-1: current title, or 1~N)
    //                 (N = number of titles on disc)
    //     @startSeconds,startFrameIdx - start play position of segment
    //     @endSeconds,endFrameIdx - end play position of segment
    //     @bRepeat - specify true to enter AB repeat mode
    //
    virtual HRESULT PlaySegment(
        int titleNum,
        unsigned int startSeconds,
        unsigned int startFrameIdx,
        unsigned int endSeconds,
        unsigned int endFrameIdx,
        bool bRepeat,
        unsigned int* cmdID)=0;

    //
    // MenuShow - show menu specified by ID.
    //
    //     @menuID - menu identifier defined by navigator
    //
    virtual HRESULT MenuShow(
        unsigned int menuID,
        unsigned int* cmdID)=0;

    // MenuEscape - escape from menu and resume previous presentation.
    virtual HRESULT MenuEscape(
        unsigned int* cmdID)=0;

    //
    // ButtonSelectNumeric - select button identified by unique number.
    //
    //     @buttonNum - button number (1~N)
    //
    virtual HRESULT ButtonSelectNumeric(
        int buttonNum,
        unsigned int* cmdID)=0;

    //
    // ButtonSelectPoint - select button identified by a pointing device.
    //
    //     @x,y - coordinate pointed.
    //
    virtual HRESULT ButtonSelectPoint(
        unsigned int x,
        unsigned int y,
        unsigned int* cmdID)=0;

    // ButtonActivateNumeric - activate button identified by unique number.
    virtual HRESULT ButtonActivateNumeric(
        int buttonNum,
        unsigned int* cmdID)=0;

    // ButtonActivatePoint - activate button identified by a pointing device.
    virtual HRESULT ButtonActivatePoint(
        unsigned int x,
        unsigned int y,
        unsigned int* cmdID)=0;

    // ButtonActivate - activate the current selected button.
    virtual HRESULT ButtonActivate(
        unsigned int* cmdID)=0;

    //
    // ButtonMoveSelection - move current button selection following direction.
    //
    //     @direction - identify button movement direction.
    //
    virtual HRESULT ButtonMoveSelection(
        NAV_BTNDIR_ID direction,
        unsigned int* cmdID)=0;

    // GoUp - go to the upper (parent) menu or playback unit, if any.
    virtual HRESULT GoUp(
        unsigned int* cmdID)=0;

    // StillEscape - escape from navigator-triggered still modes.
    virtual HRESULT StillEscape(
        unsigned int* cmdID)=0;

    //
    // SetAudioStream - set audio stream by number.
    //
    //     @streamNum - valid range defined elsewhere per individual navigators
    //
    virtual HRESULT SetAudioStream(
        int streamNum,
        unsigned int* cmdID)=0;

    //
    // SetSubpictureStream - set subpicture stream by number, turn on/off.
    //
    //     @streamNum - valid range defined elsewhere per individual navigators
    //                  (-1 means subpicture stream number unchanged)
    //     @displayState - show/hide subpicture, or keep the state unchanged.
    //
    virtual HRESULT SetSubpictureStream(
        int streamNum,
        NAV_DISPLAY_STATE displayState,
        bool bDummyStream,
        unsigned int* cmdID)=0;

    //
    // SetAngle - set viewing angle by number.
    //
    //     @angleNum - angle number (1~N)
    //                 (1 indicates the original angle)
    //                 (N is the number of angles available)
    //
    virtual HRESULT SetAngle(
        int angleNum,
        unsigned int* cmdID)=0;

    //
    // SetGetProperty_Async - generic interface for property set/get (asynchronous version)
    //
    virtual HRESULT SetGetProperty_Async(
        NAV_PROP_ID id,
        void* inData,
        unsigned int inDataSize,
        void* outData,
        unsigned int outDataSize,
        unsigned int* returnedSize,
        unsigned int* cmdID)=0;

    //
    // SendNavCommand - generic interface for sending nav commands
    //
    //     @id - command id declared in NavDef.h
    //     @cmdData - command arguments (can be NULL if there is no arguments)
    //     @cmdDataSize - total size of command arguments in bytes
    //
    virtual HRESULT SendNavCommand(
        NAV_CMD_ID id,
        void* cmdData,
        unsigned int cmdDataSize,
        unsigned int* cmdID)=0;
    
    //
    // GetBufferFullness - generic interface for querying data size in buffer
    //
    //     @pFullness - used to return data size
    //     @pinID - which buffer you are interested in, video or audio.
    //     @channelIndex - when dual video/audio decoders are running, which decoder you are interested in.
    //
	virtual HRESULT GetBufferFullness(
		int* pFullness, 
		FILTER_TYPE filterType = FILTER_DECODE_VIDEO, 
		int channelIndex = 0)=0;    

    //
    // GetIOBufferFullness - generic interface for querying data size in IO buffer
    //
    //     @pFullness - used to return data size
    //
    virtual HRESULT GetIOBufferFullness(int *pFullness, int *pFlag) = 0;   
	virtual bool	getEosFlag() = 0;
};

}  // namespace android


#endif /*__I_NAV_CONTROL_H__*/
