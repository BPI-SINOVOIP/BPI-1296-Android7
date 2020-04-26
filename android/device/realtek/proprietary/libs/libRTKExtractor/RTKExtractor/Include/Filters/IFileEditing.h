#ifndef __I_FILE_EDITING_H__
#define __I_FILE_EDITING_H__

/**
 * @brief IFileEditing is a generic interface to edit media files
 */

class IFileEditing
{
public:

    /**
     * @brief Enumeration of the Pixel Formats Used by the Editing Functions
     */
    enum PIXEL_FORMAT
    {
        YV16,    /**< YV16 (not finalized yet) */
        YV12,    /**< YV12 (not finalized yet) */
        RGB8888, /**< RGB8888 (not finalized yet) */
        RGB555   /**< RGB555 (not finalized yet) */
    };

public:

    /**
     * @brief AB Copy
     *
     * The caller should create a General File object outside and pass its file handle as parameter GFileHandle
     * of this function. The AB segment marked by startTime and endTime will be copied and inserted as content
     * of the General File object.
     *
     * @param[in] startTime the time position which marks the beginning of AB segment which is being copied.
     * @param[in] endTime the time position which marks the end of AB segment which is being copied. (If it is
     *            0xffffffff, the end position equals to the end of source file)
     * @param[in] GFileHandle the handle to the General File object.
     *
     * @remarks The units of startTime/endTime should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks The GFileHandle should be opened (by the caller) in Read/Write mode to allow modifications.
     * @remarks Applied to: Live Pause (Playback Flow), Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    ABCopy(unsigned int startTime,
           unsigned int endTime,
           unsigned int GFileHandle) = 0;

    /**
     * @brief AB Cut
     *
     * The AB segment marked by startTime and endTime will be removed from the source object.
     *
     * @param[in] startTime the time position which marks the beginning of AB segment which is being copied to target
     *            and removed from source.
     * @param[in] endTime the time position which marks the end of AB segment which is being copied to target and
     *            removed from source. (If it is 0xffffffff, the end position equals to the end of source file)
     *
     * @remarks The units of startTime/endTime should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    ABCut(unsigned int startTime,
          unsigned int endTime) = 0;

    /**
     * @brief Insert Chapter
     *
     * Add a new chapter mark at the time position specified (the newly introduced chapter is by default a visible
     * chapter). The 0-based chapter index and the time length of the new chapter can be returned if requested.
     *
     * @param[in] time the time position where the new chapter mark should be inserted. (If it is 0xffffffff, the
     *            chapter mark should be inserted at the current position)
     * @param[out] pChapterIndex this pointer, if not NULL, should be used to return the index (0-based) of the newly
     *             created chapter mark by this function.
     * @param[out] pChapterLength this pointer, if not NULL, should be used to return the time length of the newly
     *             created chapter by this function.
     *
     * @remarks The units of time/pChapterLength should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks Applied to: Live Pause (Record Flow), Recording, Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    InsertChapter(unsigned int time,
                  unsigned int* pChapterIndex,
                  unsigned int* pChapterLength) = 0;

    /**
     * @brief Remove All Chapter
     *
     * Remove all chapter marks of the source file object.
     *
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    RemoveAllChapters() = 0;

    /**
     * @brief Remove Chapter
     *
     * Remove a chapter mark which is specified by the 0-based index. The time position of the removed chapter mark,
     * as well as the time length of the removed chapter, can be returned if requested.
     *
     * @param[in] index the index (0-based) of the chapter mark to be removed.
     * @param[out] pChapterTime this pointer, if not NULL, should be used to return the time position of the newly
     *             removed chapter mark by this function.
     * @param[out] pChapterLength this pointer, if not NULL, should be used to return the time length of the newly
     *             removed chapter by this function.
     *
     * @remarks The units of pChapterTime/pChapterLength should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    RemoveChapter(unsigned int index,
                  unsigned int* pChapterTime,
                  unsigned int* pChapterLength) = 0;

    /**
     * @brief Query Chapter Number
     *
     * Query the total number of chapters in the source file object.
     *
     * @remarks Applied to: Live Pause (Record Flow), Recording, Editing
     *
     * @return Total number of chapters in the source file object.
     */
    virtual unsigned int
    QueryChapterTotalNumber() = 0;

    /**
     * @brief Query Chapter
     *
     * Query information of a chapter specified by the 0-based index.
     *
     * @param[in] index the index (0-based) of the chapter mark being queried.
     * @param[out] pChapterTime this pointer, if not NULL, should be used to return the time position of the
     *             chapter mark being queried.
     * @param[out] pChapterLength this pointer, if not NULL, should be used to return the time length of the
     *             chapter being queried.
     * @param[out] pIsHidden this pointer, if not NULL, should be used to return whether the chapter being queried
     *             is hidden or not.
     *
     * @remarks The units of pChapterTime/pChapterLength should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks Applied to: Live Pause (Record Flow), Recording, Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    QueryChapter(unsigned int index,
                 unsigned int* pChapterTime,
                 unsigned int* pChapterLength,
                 bool* pIsHidden) = 0;

    /**
     * @brief Configure Auto Chapter
     *
     * Configure time duration that is used by the auto chapter generation mechanism (using 0xffffffff as the
     * time duration would practically disable auto chapter generation).
     *
     * @param[in] chapterDuration the time distance between auto generated chapter marks. (If it is
     *            0xffffffff, the auto chapter generation mechanism will be disabled)
     *
     * @remarks The units of chapterDuration should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks Applied to: Live Pause (Record Flow), Recording
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    ConfigureAutoChapter(unsigned int chapterDuration) = 0;

    /**
     * @brief Show or Hide Chapter
     *
     * Set a chapter, specified by the 0-based index, as visible or invisible.
     *
     * @param[in] index the index (0-based) of the chapter being shown/hidden.
     * @param[in] bShow If true, the chapter of concern is visible. If false, the chapter of concern is hidden.
     *
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    ShowChapter(unsigned int index,
                bool bShow) = 0;

    /**
     * @brief Finalize Title
     *
     * By finalization, the IFO tables of the loaded/recorded file object (title) will be persisted to permanent storage.
     *
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks The implementation of this function should have an internal “dirty flag” mechanism to prevent redundant
     *          persistence actions.
     * @remarks Applied to: Recording, Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    FinalizeTitle() = 0;

    /**
     * @brief Divide Title
     *
     * The caller should create a General File object outside and pass its file handle as parameter GFileHandle of this
     * function. The content after time position specified by time will be separated into this General File object.
     *
     * @param[in] time the time position after which the content will be separated into another file object.
     * @param[in] GFileHandle the handle to the General File object that is to receive the separated content.
     *
     * @remarks The units of time should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks The GFileHandle should be opened (by the caller) in Read/Write mode to allow modifications.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    DivideTitle(unsigned int time,
                unsigned int GFileHandle) = 0;

    /**
     * @brief Merge Title
     *
     * The caller should create a General File object outside and pass its file handle as parameter GFileHandle of this
     * function. The content of this General File object will be merged (appended to the source file object).
     *
     * @param[in] GFileHandle the handle to the General File object to be merged.
     *
     * @remarks In the Editing scenario, since this operation modifies the source file object, it is not allowed when
     *          playback or recording of the same source file object is on-going.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    MergeTitle(unsigned int GFileHandle) = 0;

    /**
     * @brief Set Key Frame
     *
     * Specify the time position where the closest decodable picture should be used as the key (representing) picture of the
     * source file object.
     *
     * @param[in] time the time position where the closest decodable picture should be used as the key (representing) picture.
     *
     * @remarks The units of time should be globally defined based on the project requirement.
     *          For example, it can be defined as 1 time unit per second.
     * @remarks We can consider setting key frame as some action that doesn’t really modify the content of title. Therefore,
     *          we would allow setting key frame wherever this function can be applied.
     * @remarks Applied to: Recording, Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    SetKeyFrame(unsigned int time) = 0;

    /**
     * @brief Get Key Frame
     *
     * The caller should prepare a memory buffer with appropriate size, and pass the buffer pointer along with the
     * parameters/attributes of the requested picture. On success, the key (representing) picture of the source file
     * object will be stored in the memory buffer when it returns.
     *
     * @param[out] pThumbnailBuffer the memory buffer (must not be NULL) used to receive decoded or stored key frame picture.
     * @param[in] width the width of the expected picture. (in pixels)
     * @param[in] pitch the pitch of the expected picture. (in bytes)
     * @param[in] height the height of the expected picture. (in pixels)
     * @param[in] bitsPerSample the number of bits used per pixel.
     * @param[in] pixelFormat the format of pixel.
     *
     * @remarks The width/pitch/height/bitsPerSample/pixelFormat will be checked against the supported key frame formats.
     *          If it is not supported, we can return E_FAIL or simply generate an assertion failure.
     * @remarks The General File object may be able to store the previously decoded key frame picture along with itself.
     *          As a result, if a previously stored picture is found, we can simply return the stored one without re-decoding it
     *          from bit-stream.
     * @remarks If the stored picture cannot be found, and the key frame time position is set. We should invoke video decoder to
     *          re-decode a key frame picture from the specified key frame time position. We may store the newly decoded key frame
     *          picture in General File object for later use.
     * @remarks If the stored picture cannot be found, and the key frame time position is not set. We should invoke video decoder to
     *          help search for a proper default key frame picture from the beginning of title bit-stream, and decode the selected picture.
     *          We should remember the key frame time position of the selected picture for later use. We may store the newly decoded key
     *          frame picture in General File object for later use too.
     * @remarks If the key frame time position setting is modified by the caller. Any previously stored key frame picture in General
     *          File object should be invalidated.
     * @remarks Applied to: Editing
     *
     * @return S_OK on success; E_FAIL on failure.
     */
    virtual HRESULT
    GetKeyFrame(void* pThumbnailBuffer,
                unsigned int width,
                unsigned int pitch,
                unsigned int height,
                unsigned int bitsPerSample,
                PIXEL_FORMAT pixelFormat) = 0;
};

#endif /*__I_FILE_EDITING_H__*/
