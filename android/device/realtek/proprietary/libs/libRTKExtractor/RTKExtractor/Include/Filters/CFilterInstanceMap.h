#ifndef __C_FILTER_INSTANCE_MAP_H__
#define __C_FILTER_INSTANCE_MAP_H__

////////////////////////////////////////////////////////////////////////////////
// CFilterInstanceMap - handle mapping from instance IDs to filter objects

class CBaseFilter;

class CFilterInstanceMap
{
public:

    CFilterInstanceMap(int mapSize);
    virtual ~CFilterInstanceMap();

    void RegisterFilter(CBaseFilter* pFilter, long instanceID);
    void UnregisterFilter(long instanceID);
    CBaseFilter* MapToFilter(long instanceID);

protected:

    struct MapEntry
    {
        CBaseFilter* pFilter;
        long         instanceID;
    };

    MapEntry* m_map;
    int       m_mapSize;
    int       m_mapItems;
};

#endif //__C_FILTER_INSTANCE_MAP_H__
