/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/

 
#include "CWCommon.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

int gCWForceMTU = 0;
int gCWRetransmitTimer = CW_RETRANSMIT_INTERVAL_MICROSECOND;	//Default value for RetransmitInterval
int gCWNeighborDeadInterval = CW_NEIGHBORDEAD_INTERVAL_DEFAULT; //Default value for NeighbourDeadInterval (no less than 2*EchoInterval and no greater than 240) 
int gCWMaxRetransmit = CW_MAX_RETRANSMIT_DEFAULT;		//Default value for MaxRetransmit 
#ifdef RTK_129X_PLATFORM
pid_t gPid = 0;
#endif

int CWTimevalSubtract(struct timeval *res, const struct timeval *x, const struct timeval *y){
	int nsec;
	struct timeval z=*y;
   
	// Perform the carry for the later subtraction by updating Y
	if (x->tv_usec < z.tv_usec) {
		nsec = (z.tv_usec - x->tv_usec) / 1000000 + 1;
		z.tv_usec -= 1000000 * nsec;
		z.tv_sec += nsec;
	}
	if (x->tv_usec - z.tv_usec > 1000000) {
		nsec = (x->tv_usec - z.tv_usec) / 1000000;
		z.tv_usec += 1000000 * nsec;
		z.tv_sec -= nsec;
	}

	// Compute the time remaining to wait. `tv_usec' is certainly positive
	if ( res != NULL){
		res->tv_sec = x->tv_sec - z.tv_sec;
		res->tv_usec = x->tv_usec - z.tv_usec;
	}

	// Return 1 if result is negative (x < y)
	return ((x->tv_sec < z.tv_sec) || ((x->tv_sec == z.tv_sec) && (x->tv_usec < z.tv_usec)));
}

/* return Epoch time in minutes */ 
unsigned int CWGetTimeInMins()
{
	struct timeval tp; 
	(void) gettimeofday(&tp,(struct timezone *)0); 

	return tp.tv_sec/60;		//convert second to min
}

/* return Epoch time in seconds */ 
unsigned int CWGetTimeInSeconds()
{
	struct timeval tp; 
	(void) gettimeofday(&tp,(struct timezone *)0); 

	return tp.tv_sec;		
}

/* return Epoch time in milliseconds */ 
unsigned long CWGetTimeInMilliSeconds()
{
	struct timeval tp; 
	(void) gettimeofday(&tp,(struct timezone *)0); 

	return (tp.tv_sec*1000 + tp.tv_usec/1000);		//convert second and micro to milli
}

int pidfile_acquire(char *pidfile)
{
	int pid_fd;

	if(pidfile == NULL)
		return -1;

	pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
	if (pid_fd < 0)
		printf("Unable to open pidfile %s\n", pidfile);
	else
		lockf(pid_fd, F_LOCK, 0);

	return pid_fd;
}

void pidfile_write_release(int pid_fd)
{
	FILE *out;

	if(pid_fd < 0)
		return;

	if((out = fdopen(pid_fd, "w")) != NULL) {
		fprintf(out, "%d\n", rtk_getpid());
		fclose(out);
	}
	lockf(pid_fd, F_UNLCK, 0);
	close(pid_fd);
}



#ifdef RTK_129X_PLATFORM
pid_t get_real_pid()
{
	struct iwreq wrq;
	int sd, ret;
	unsigned char databuf[10]={0};
	pid_t *pid_drv;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd>=0)
	{
		wrq.u.data.length = sizeof(databuf);
		wrq.u.data.pointer = (caddr_t) databuf;
		strcpy(wrq.ifr_name, "wlan0");
		ret = ioctl(sd, GETPID_IOCTL, &wrq);
		if(!ret)
		{
			pid_drv = (pid_t*)databuf;
			//printf("pid_drv=%d pid_user=%d\n", *pid_drv, getpid());
		}

		close(sd);
	}

	if(*pid_drv > 0)
		return *pid_drv;
	else
		return 0;

}
#endif


pid_t rtk_getpid()
{
#ifdef RTK_129X_PLATFORM
	if(!gPid)
		gPid = get_real_pid();

	return gPid;
#else
	return getpid();
#endif
}

