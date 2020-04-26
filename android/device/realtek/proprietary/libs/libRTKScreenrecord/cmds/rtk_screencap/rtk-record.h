#ifndef __RTK_RECORD_H__
#define __RTK_RECORD_H__

class RtkRecordPrivte;

class RtkRecord {
public:
    RtkRecord();
    virtual ~RtkRecord();

    int     update(void);

    /* output format : ARGB8888 */
    void *  getVirAddr      (void);
    int     getWidth        (void);
    int     getHeight       (void);
    int     getStride       (void);
    int     getFormat       (void);
    unsigned long getSize   (void);

private:
    RtkRecordPrivte * mPrivate;
};


#endif /* End of __RTK_RECORD_H__ */
