#ifndef __INFO_HEADER__
#define __INFO_HEADER__


struct IFO_MAIN_HEADER
{
    char    ID1[4];
    int     headerBytes;
    int64_t ifoOffset;
    int     ifoBytes;
    int64_t thumbnailOffset;
    int     thumbnailBytes;
    char    ID2[4];

    IFO_MAIN_HEADER()
    {
        ID1[0] = 'R';
        ID1[1] = 'T';
        ID1[2] = 'K';
        ID1[3] = 'I';

        headerBytes = sizeof(*this);
        ifoOffset = thumbnailOffset = 0;
        ifoBytes = thumbnailBytes = 0;

        ID2[0] = 'r';
        ID2[1] = 't';
        ID2[2] = 'k';
        ID2[3] = 'i';
    }

    bool IsValid()
    {
        return (ID1[0] == 'R' &&
                ID1[1] == 'T' &&
                ID1[2] == 'K' &&
                ID1[3] == 'I' &&
                ID2[0] == 'r' &&
                ID2[1] == 't' &&
                ID2[2] == 'k' &&
                ID2[3] == 'i' && headerBytes == sizeof(*this));
    }
};


#endif /*__INFO_HEADER__*/
