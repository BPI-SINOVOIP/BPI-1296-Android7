/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HEIF_EXTRACTOR_H_

#define HEIF_EXTRACTOR_H_

#include <utils/Errors.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaExtractor.h>
#include <map>

namespace android
{

struct AMessage;
class DataSource;
class String8;

typedef uint32_t ItemId;
typedef uint32_t PropertyId;

struct IINFInfo
{
    uint32_t itemType; ///< Item type from Item Information box
};

struct ILOCInfo
{
    uint16_t constructionMethod;
    uint64_t base_offset;
    uint64_t extent_offset;
    uint64_t extent_length;
};

struct GridItem
{
    uint8_t rows;
    uint8_t columns;
    uint32_t outputWidth;
    uint32_t outputHeight;
    Vector<uint32_t> itemIds;
};

struct ItemPropertyInfo
{
    uint32_t type;
    uint64_t size;
    union
    {
        struct
        {
            uint32_t width;      ///< Width of the image from ispe property
            uint32_t height;     ///< Height of the image from ispe property
        } iSPE;

        struct
        {
            uint8_t rotationDegrees;
        } iROT;

        struct
        {
            off64_t offset;
        } offset;

    } value;
};

struct IPMAInfo
{
    Vector<uint32_t> associationPropertyIds;
};

struct IDATInfo
{
    off64_t offset;
    uint64_t size;
};

struct HEIFInfo
{
    std::map<ItemId, IINFInfo> itemInfoMap;
    std::map<ItemId, ILOCInfo> itemLocationMap;
    std::map<ItemId, GridItem> gridItemMap;     ///< Data of image grid items
    std::map<PropertyId, ItemPropertyInfo> itemPropertyMap;
    std::map<ItemId, IPMAInfo> itemAssociationMap;
    Vector<uint32_t> thmbItemIds;
    Vector<uint32_t> exifItemIds;
    IDATInfo idatInfo;
    uint32_t primaryItemID;
};

void parseImageGrid();

class HEIFExtractor : public MediaExtractor
{
public:
    // Extractor assumes ownership of "source".
    HEIFExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);
    virtual sp<MetaData> getMetaData();
    virtual status_t parseChunk(off64_t *offset, int depth);
    virtual void getMetaByItemId(uint32_t itemId, sp<MetaData> &mMeta);
    virtual uint32_t getPrimaryItemId();
    virtual uint32_t getItemIdByType(uint32_t itemType);
    virtual uint32_t getGridItem(uint32_t index);
    virtual uint32_t getGridItemSize();
    virtual void parseImageGrid();


    /* start of heif used*/
    HEIFInfo mHEIFInfo;
    off64_t readOffset;
    /* end of heif used*/

private:
    status_t mInitCheck;
    sp<DataSource> mDataSource;
    sp<MetaData> mMeta;

    /* start of heif used*/
    enum
    {
        // max track header chunk to return
        kMaxTrackHeaderSize = 32,

        // maximum size of an atom. Some atoms can be bigger according to the spec,
        // but we only allow up to this size.
        kMaxAtomSize = 64 * 1024 * 1024,
    };

    struct PathAdder
    {
        PathAdder(Vector<uint32_t> *path, uint32_t chunkType)
            : mPath(path)
        {
            mPath->push(chunkType);
        }

        ~PathAdder()
        {
            mPath->pop();
        }

    private:
        Vector<uint32_t> *mPath;

        PathAdder(const PathAdder &);
        PathAdder &operator=(const PathAdder &);
    };

    Vector<uint32_t> mPath;
    /* end of heif used*/

    HEIFExtractor(const HEIFExtractor &);
    HEIFExtractor &operator=(const HEIFExtractor &);
};

}  // namespace android

#endif  // HEIF_EXTRACTOR_H_
