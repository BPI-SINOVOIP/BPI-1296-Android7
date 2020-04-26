#ifndef __EDITING_IFO_H__
#define __EDITING_IFO_H__


class CPosTable
{
public:
	CPosTable(int tableSize);
	virtual ~CPosTable();
	
	int 	m_tableSize;
	int 	m_tailIdx;	
	int 	m_headIdx;
	int 	m_currIdx;
	
	unsigned int* m_pPosTable;
	
	bool 	Insert(unsigned int pos);
	bool 	GetTableIdx(int index, int* pIndex);
	bool    RemoveUnusedEntry(unsigned int pos);
	bool 	Divide(void* pTable, unsigned int pos);
	
	int		MoveCurrIdx(unsigned int pos, bool* pSegChanged);
	int 	QueryTotalNumber();
	int 	GetIdxByPos(unsigned int pos);
	
	
protected:
	virtual bool RemoveInfoEntry(int index);
};

class CCGMSInfo : public CPosTable
{
public:
	const static int CGMS_TABLE_SIZE 	  = (256);
	
	CCGMSInfo(int tableSize);
	~CCGMSInfo();
	
	CGeneralIFO::CGMS_TYPE* m_pTypeTable;
	
	bool	Insert(unsigned int pos, CGeneralIFO::CGMS_TYPE type);
	bool	ABCopy(CCGMSInfo* pCGMSInfo, unsigned int startPos, unsigned int endPos, unsigned int appendPos);
	bool	ABCut(unsigned int startPos, unsigned int endPos);
};

class CChapterInfo : public CPosTable
{
public:
    const static int CHAPTER_TABLE_SIZE   = (256);

	CChapterInfo(int tableSize);
	~CChapterInfo();
	
	struct CHAPTER_INFO {
		unsigned char 	bIsHidden;
		unsigned char	bSeamless;	// if false, there may be a pts gap at pos. The following content should be a new segment.
		unsigned int	time;		// time * IFOContext->TMAPTimeUnit = N seconds
	}* m_pChapterTable;

	int 	Insert(unsigned int time, unsigned int pos);
	bool 	ABCopy(CChapterInfo* pChapterInfo, unsigned int startPos, unsigned int endPos, unsigned int appendPos, unsigned int appendTime);
	bool 	ABCut(unsigned int startPos, unsigned int endPos, int shiftTime = 0);
	void 	ShiftEntries(int start, int totalEntry, int direction);
};

class CStateMap : public CPosTable
{
public:
	const static unsigned int MAX_STATE_MAP_ENTRIES           = 512;

	CStateMap(int tableSize);
	~CStateMap();
	
	struct STATE_INFO{ 
		CGeneralIFO::STATE_TYPE type; 
		void* state; 
		int bytes; 
	}* m_pStateMap;

	bool	Insert(unsigned int  pos, CGeneralIFO::STATE_TYPE type, void* state, int bytes);
	bool 	ABCopy(CStateMap* pStateTable, unsigned int startPos, unsigned int endPos, unsigned int appendPos);
	bool 	ABCut(unsigned int startPos, unsigned int endPos);
	
protected:
	bool RemoveInfoEntry(int index);
};
#endif /*__EDITING_IFO_H__*/
