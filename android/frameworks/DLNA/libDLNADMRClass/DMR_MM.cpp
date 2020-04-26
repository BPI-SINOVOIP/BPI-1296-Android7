#include "DLNA_DMR.h"
#include "ILibThreadPool.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define MINIMUM_SEEK_TIME	3

mmRenderer::mmRenderer(SUBRENDERER_TYPE renderType)
{
	m_protocolInfo = NULL;
	m_lInitSeekPosition = -1; // Init the begin play position
	m_subRenderer_type = renderType;
	m_pLoadURI = NULL;
	m_lTotalTime = -1;
	m_Rate = 0;
}

mmRenderer::~mmRenderer()
{
	if(m_protocolInfo)
	{
		free(m_protocolInfo);
		m_protocolInfo = NULL;
	}

    if (m_pLoadURI) {
        free(m_pLoadURI);
        m_pLoadURI = NULL;
    }
}

int mmRenderer::loadMedia(char *filename)
{
    if (m_pLoadURI) {
         free(m_pLoadURI); m_pLoadURI=NULL;
    }

    m_pLoadURI = strdup(filename);

    ALOGI("DMR : mmRenderer::loadMedia(%s)", filename);

	m_lInitSeekPosition = -1; // Init the begin play position
	m_Rate = 0;
	return RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_LOADMEDIA);
}

int mmRenderer::preParse(char *filename, unsigned int *NumberOfTracks, char ***MediaTrackURI, SUBRENDERER_TYPE **MediaType, long *TotalTime,char ***ProtocalInfo)
{
	*TotalTime = m_lTotalTime;
	*NumberOfTracks = 1;

	*MediaTrackURI = new char*[*NumberOfTracks];	// NumberOfTracks = 1, temporary;
	if( filename != NULL)
		*MediaTrackURI[0] = strdup(filename);
	else
		*MediaTrackURI[0] = NULL;

	*MediaType = new SUBRENDERER_TYPE[*NumberOfTracks];
	for(unsigned int i = 0; i < *NumberOfTracks; i++)
		*MediaType[i] = m_subRenderer_type;

	return 0;
}

int mmRenderer::Play(char *filename, int speed)
{
	ALOGI("[DMR subRenderer][Audio/Video] speed %d, [%s]", speed , filename);

	if(loadMedia(filename) == 0)
	{
		if (m_lInitSeekPosition > 0) {
            // Todo: Seek first?;
		}
		RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_PLAY);

		if (speed != 1)
		{
			m_Rate = speed/256;
		}
		return 1; // S_FALSE
	}
	return -1;
}

int mmRenderer::Pause(bool Pause)
{
	ALOGI("[DMR subRenderer][Audio/Video] Pause()\n");
	//return (m_pPlayControl->Pause());
	if(Pause == true)
		RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_PAUSE);
	else
		RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_PLAY);

	return 0;
}

void mmRenderer::Stop()
{
	ALOGI("[DMR subRenderer][Audio/Video] Stop\n");
	RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_STOP);
}

int mmRenderer::SeekMediaPosition(int title, long position)
{
	long playTime = position;
	m_lInitSeekPosition = position;
	ALOGI("SeekMediaPosition time=%ld, TotalTime:%ld", playTime, m_lTotalTime);

    if (m_lTotalTime > 0 && ((m_lTotalTime - position) < MINIMUM_SEEK_TIME))
    {
        ULOGE("[DMR avRenderer] The PLAYATTIME is failed (%ld/%ld)", position, m_lTotalTime);
        return E_FAIL;
    }

	RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_SEEK, &m_lInitSeekPosition);

	DMR_StateChange_AbsoluteTimePosition(RTK_DLNA_DMR::m_pDMR_MicroStack, m_lInitSeekPosition*1000);
	DMR_StateChange_RelativeTimePosition(RTK_DLNA_DMR::m_pDMR_MicroStack, m_lInitSeekPosition*1000);

	return 0;
}

int mmRenderer::SetRate(int rate)
{
	return RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_SETRATE, &rate);
}

void mmRenderer::SetProtocolInfo(char *pinfo)
{
	if(m_protocolInfo)
	{
		free(m_protocolInfo);
		m_protocolInfo = NULL;
	}

	if(pinfo)
		m_protocolInfo = strdup(pinfo);
}

void mmRenderer::UpdateStatus(DMR_UPDATE_STATUS_TYPE type, long value)
{
    switch (type)
    {
    case DMR_UPDATE_STATUS_DURATION:
        m_lTotalTime = value / 1000;
        break;
	case DMR_UPDATE_STATUS_TRANSPORTSTATE:
		if (value == DMR_PS_Playing && m_Rate != 0)
		{
			SetRate(m_Rate);
			m_Rate = 0;
		}
        break;
    }
}

