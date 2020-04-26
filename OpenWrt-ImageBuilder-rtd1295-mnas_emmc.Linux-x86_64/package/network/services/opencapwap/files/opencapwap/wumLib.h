#include <stdio.h>
#include "ACAppsProtocol.h"

#define SUCCESS	0
#define ERROR	1

#define FRAGMENT_SIZE 4000

struct WTPInfo {
	int wtpId;
	CWStateTransition currentState;
	unsigned int echoRequestCounter;
	time_t lastEchoRequestTime;
	char *name;
};

struct version_info {
	char major;
	char minor;
	char revision;
	int size;
};

int ACServerConnect(char *address, int port);
void ACServerDisconnect(int acserver);
struct WTPInfo *ACServerWTPList(int acserver, int *nWTPs);	
CWRtkStationValues *ACServerStationsList(int acserver, int wtp_id, int *nStations);
void freeWTPList(struct WTPInfo *wtpList, int nWTPs);


int WUMGetWTPVersion(int acserver, int wtpId, struct version_info *);
int WUMReadCupVersion(char *cup_pathname, struct version_info *update_v);
int WUMUpdate(int acserver, int wtpId, void *cup_buf, struct version_info update_v);
int WUMSendCancelRequest(int acserver, int wtpId);
int WUMSendUpdateRequest(int acserver, int wtpId, struct version_info update_v);

int WUMChangeImage(int acserver, int wtpId, const char *image_version);
int WUMKickStation(int acserver, int wtpId, const unsigned char *mac);
CWRtkSiteSurveyValues *WUMSiteSurvey(int acserver, int wtpId, int *nSite, char *msgBuf);
int wumUpdatePower(int acserver, int wtpId, unsigned char radioId, unsigned char power);
int wumUpdateChannel(int acserver, int wtpId, unsigned char radioId, unsigned char channel);


