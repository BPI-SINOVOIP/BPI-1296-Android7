#ifndef _TOGO_SOURCE_STATUS_H_
#define _TOGO_SOURCE_STATUS_H_



typedef struct _TOGO_SOURCE_STATUS_T
{
	int id;
	int isAvailable;
	char *pName;
}TOGO_SOURCE_STATUS_T;

class ToGoSourceStatus
{
public:
	ToGoSourceStatus();
	~ToGoSourceStatus();
	void setCount(int count);
	int getCount();
	int setName(int index, char* pName, int nameLen);
	char* getName(int index);
	int setId(int index, int id);
	int getId(int index);
	int setAvailable(int index, int available);
	int getAvailable(int index);
	void clear();

private:
	int mCount;
	TOGO_SOURCE_STATUS_T *mSources;
};






#endif