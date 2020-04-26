
/*
  internal api function 
*/

#ifdef SI_DVB_ENABLE_SSU


void *SI_Callback_ScanSsu(SI *si, void *arg);
void *SI_Callback_DownloadStartSsu(SI *si, void *arg);
void *SI_Callback_DownloadErrorSsu(SI *si, void *arg);
void *SI_Callback_DownloadProgressUpdateSsu(SI *si, void *arg);
void *SI_Callback_VerifyProgressUpdateSsu(SI *si, void *arg);
void *SI_Callback_DownloadOkSsu(SI *si, void *arg);
#endif//SI_DVB_ENABLE_SSU


/////////////////////////////////////////////////////////////
// Callbacks
/////////////////////////////////////////////////////////////

/**
	@brief used by agent
*/
void *SI_Callback_ScanChannel(SI *si, void *arg);
void *SI_Callback_PresentEITChange(SI *si, void *arg);
void *SI_Callback_FollowingEITChange(SI *si, void *arg);
void *SI_Callback_ScheduleEITChange(SI *si, void *arg);
void *SI_Callback_PMTChange(SI *si, void *arg);
void *SI_Callback_StreamChangeBegin(SI *si, void *arg);
void *SI_Callback_StreamChangeEnd(SI *si, void *arg);
void *SI_Callback_PATChange(SI *si, void *arg);
void *SI_Callback_ANITChange(SI *si, void *arg);	// bug 26588, Evora iSuite v2.3, GBR/394
void *SI_Callback_ResetFilePosition(SI *si, void *arg);
void *SI_Callback_SDTChange(SI *si, void *arg);


/////////////////////////////////////////////////////////////
// Function for PVR integration
/////////////////////////////////////////////////////////////

/**
	@brief reset file pointer to start
*/
ErrCode SI_PVRInterface_ResetFilePosition(SI *si);


/**
	@brief used by navigation filter
*/
void SI_PVRInterface_Flush(unsigned int si);



