#ifndef _TOGO_CLIENT_STATUS_H_
#define _TOGO_CLIENT_STATUS_H_

typedef struct _TOGO_SESSION_T
{
	int clientId;
	int sourceType;
	int state;
}TOGO_SESSION;

class ToGoClientStatus
{
public:
	ToGoClientStatus();
	~ToGoClientStatus();
	void clear();
	void setCount(int count);
	int getCount();
	int setClientMax(int max);
	int getClientMax();
	int setId(int index, int id);
	int getId(int index);
	int setSourceType(int index, int type);
	int getSourceType(int index);
	int setState(int index, int state);
	int getState(int index);

private:
	int mSessionMax;
	int mCount;
	TOGO_SESSION *mSessions;
};






#endif