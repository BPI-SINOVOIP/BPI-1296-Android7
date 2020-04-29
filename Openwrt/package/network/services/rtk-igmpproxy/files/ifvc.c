/*
**  smcroute - static multicast routing control 
**  Copyright (C) 2001 Carsten Schill <carsten@cschill.de>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**  $Id: ifvc.c,v 1.1.1.1 2007-08-06 10:04:43 root Exp $	
**
**  This module manages an interface vector of the machine
**
*/

#include "mclab.h"
#include <linux/sockios.h>
#ifdef CONFIG_ISP_IGMPPROXY_MULTIWAN
#include "igmpproxy.h"
#include <signal.h>
#endif

struct IfDesc IfDescVc[ MAX_IF ], *IfDescEp = IfDescVc;
#ifdef CONFIG_ISP_IGMPPROXY_MULTIWAN
int VifSock = -1;
extern char igmp_if_sock_old[MAX_IF];
extern int signo;
sigset_t newmask, oldmask, pendmask;
#endif



#ifdef CONFIG_ISP_IGMPPROXY_MULTIWAN
//add name if necessary, update Flags, pif_index, local addr, 
int getIfFromKernel(const char *IfName, struct IfDesc * Ifd)
{
	struct ifreq IfReq;
	
	if(VifSock < 0)
	{
		//first open socket
		if( (VifSock = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
		{
			log( LOG_ERR, errno, "RAW socket open" );
			return 0;
		}
	}

	if(strcmp(Ifd->Name, "")==0)
		memcpy(Ifd->Name, IfName, sizeof(Ifd->Name));

	if(strcmp(Ifd->Name, IfName)==0)
		memcpy(IfReq.ifr_name, IfName, sizeof( IfReq.ifr_name ));
	else
		return 0;

	
	if( ioctl(VifSock, SIOCGIFFLAGS, &IfReq ) < 0 )
	{
		log( LOG_ERR, errno, "ioctl SIOCGIFFLAGS" );
		return 0;
	}
	
	Ifd->Flags = IfReq.ifr_flags;

	
	if( ioctl(VifSock, SIOCGIFINDEX, &IfReq ) < 0 )
	{
		log( LOG_ERR, errno, "ioctl SIOCGIFINDEX" );
		return 0;
	}
	
	Ifd->pif_idx = IfReq.ifr_ifindex;

	if( ioctl(VifSock, SIOCGIFADDR, &IfReq ) < 0 )
	{
		log( LOG_ERR, errno, "ioctl SIOCGIFADDR" );
		Ifd->InAdr.s_addr = 0;  /* mark as non-IP interface */
	}
	else
	{
		if( IfReq.ifr_addr.sa_family != AF_INET ) {
			Ifd->InAdr.s_addr = 0;  /* mark as non-IP interface */
		}
			
		Ifd->InAdr = ((struct sockaddr_in *)&IfReq.ifr_addr)->sin_addr;
	}

#if 0
	printf("if = %s, flag = %d, ifindex = %d, inaddr = %x, [%s:%d]\n", 
		Ifd->Name, Ifd->Flags, Ifd->pif_idx, Ifd->InAdr.s_addr, __FUNCTION__, __LINE__);
#endif

	return 1;
	
}

void buildIfVc()
/*
** sync IfDescVc with igmp_down_if_name and igmp_up_if name
*/
{
	struct IfDesc *Ddp = NULL, *Dup = NULL;
	struct IfDesc *Dp = NULL;
	int vcindex;
	int ifindex;

	//block signal
	sigemptyset(&newmask);
	sigaddset(&newmask, signo);
	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask)<0)
		printf("signal block error!\n");

	//down if
	Ddp = getIfByName(igmp_down_if_name);
	if(Ddp)
	{
		//already exist, update it	
	}
	else
	{
		//not exist, find empty IfDesc
		for(vcindex=0, Dp=IfDescVc; vcindex<MAX_IF; vcindex++, Dp++)
		{
		  	if(strcmp(Dp->Name, "") == 0)
		  	{
				Ddp = Dp;
				break;
		  	}
		}
	}

	if(Ddp)
	{	
		getIfFromKernel(igmp_down_if_name, Ddp);
	}
	else
	{
		printf("too much interface!\n");
	}
	//fixed vif index
	Ddp->vif_idx = 0;

	//up if
	for(ifindex=0;ifindex<MAXWAN;ifindex++)
 	{
		if(strcmp(igmp_up_if_name[ifindex], "") == 0)
			continue;

		Dup = getIfByName(igmp_up_if_name[ifindex]);
		if(Dup)
		{
			//already exist, update it
		}
		else
		{
			for(vcindex=0, Dp=IfDescVc; vcindex<MAX_IF; vcindex++, Dp++)
			{
				if(strcmp(Dp->Name, "") == 0)
				{
					Dup = Dp;
					break;
				}
			}
		}
		
		if(Dup)
		{	
			getIfFromKernel(igmp_up_if_name[ifindex], Dup);
		}
		else
		{
			printf("too much interface!\n");
		}

		/*fixed vif index, eth1.x ->2x-1, pppx->2x*/
		if(strncmp(igmp_up_if_name[ifindex], "eth1", 4)==0)
			Dup->vif_idx = 2*(igmp_up_if_name[ifindex][5]-'0')-1;
		else if(strncmp(igmp_up_if_name[ifindex], "ppp", 3)==0)
				Dup->vif_idx = 2*(igmp_up_if_name[ifindex][3]-'0');
		else
			Dup->vif_idx = -1;
		

	}

	/*delete interface in IfDescVc but not up/down inf
	**sock not changed here, record old sock
	*/
	
	for(vcindex=0, Dp=IfDescVc; vcindex<MAX_IF; vcindex++, Dp++)
	{
		if(strcmp(Dp->Name, "") == 0)
		{
			igmp_if_sock_old[vcindex] = 0;
			continue;
		}

		igmp_if_sock_old[vcindex] = Dp->sock;
		for(ifindex=0; ifindex<MAXWAN; ifindex++)
		{
			if(strcmp(Dp->Name, igmp_up_if_name[ifindex])==0)
					break;
		}

		if(ifindex==MAXWAN && strcmp(Dp->Name, igmp_down_if_name)!=0)
		{
			close(Dp->sock);
			memset(Dp, 0, sizeof(struct IfDesc));
		}
				
	}

	//unblock signal
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL)<0)
		printf("sig set mask error!\n");
}
struct IfDesc *getIfByName( const char *IfName )
/*
** Returns a pointer to the IfDesc of the interface 'IfName'
**
** returns: - pointer to the IfDesc of the requested interface
**          - NULL if no interface 'IfName' exists
**          
*/
{
  	struct IfDesc *Dp;
  	int i;

  	for( i = 0, Dp = IfDescVc; i < MAX_IF; i++, Dp++ ) 
    	if( strcmp(IfName, "")!=0 && strcmp(IfName, Dp->Name)==0 ) 
      		return Dp;

  	return NULL;
}
struct IfDesc *getIfByIx( unsigned Ix )
/*
** Returns a pointer to the IfDesc of the interface 'Ix'
**
** returns: - pointer to the IfDesc of the requested interface
**          - NULL if no interface 'Ix' exists
**          
*/
{
  	struct IfDesc *Dp = &IfDescVc[ Ix ];
	return Dp;
}
int chk_local( __u32 addr )
/*
** Returns a pointer to the IfDesc of the interface 'IfName'
**
** returns: - pointer to the IfDesc of the requested interface
**          - NULL if no interface 'IfName' exists
**          
*/
{
	struct IfDesc *Dp;
	int i;
	for( i = 0, Dp = IfDescVc; i < MAX_IF; i++, Dp++ ) 
		if(strcmp(Dp->Name, "")!=0 && Dp->InAdr.s_addr == addr) 
			return 1;

  	return 0;
}

unsigned long getAddrByVifIx(int ix)
{
	int i;
	struct IfDesc *Dp;
	for( i = 0, Dp = IfDescVc; i < MAX_IF; i++, Dp++ ) 
		if(strcmp(Dp->Name, "")!=0 && Dp->vif_idx == ix) 
			return Dp->InAdr.s_addr;

	return 0;
}
#else
void buildIfVc()
/*
** Builds up a vector with the interface of the machine. Calls to the other functions of 
** the module will fail if they are called before the vector is build.
**          
*/
{
  struct ifreq IfVc[ sizeof( IfDescVc ) / sizeof( IfDescVc[ 0 ] )  ];
  struct ifreq *IfEp;

  int Sock;

	memset(IfDescVc, 0, sizeof( IfDescVc ) );
	IfDescEp = IfDescVc;
	
  if( (Sock = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
    log( LOG_ERR, errno, "RAW socket open" );

  /* get If vector
   */
  {
    struct ifconf IoCtlReq;

    IoCtlReq.ifc_buf = (void *)IfVc;
    IoCtlReq.ifc_len = sizeof( IfVc );

    if( ioctl( Sock, SIOCGIFCONF, &IoCtlReq ) < 0 )
      log( LOG_ERR, errno, "ioctl SIOCGIFCONF" );

    IfEp = (void *)((char *)IfVc + IoCtlReq.ifc_len);
  }

  /* loop over interfaces and copy interface info to IfDescVc
   */
  {
    struct ifreq  *IfPt;
    struct IfDesc *IfDp;

    for( IfPt = IfVc; IfPt < IfEp; IfPt++ ) {
      char FmtBu[ 32 ];
  
      strncpy( IfDescEp->Name, IfPt->ifr_name, sizeof( IfDescEp->Name ) );

      /* don't retrieve more info for non-IP interfaces
       */
      if( IfPt->ifr_addr.sa_family != AF_INET ) {
	IfDescEp->InAdr.s_addr = 0;  /* mark as non-IP interface */
	IfDescEp++;
	continue;
      }

      IfDescEp->InAdr = ((struct sockaddr_in *)&IfPt->ifr_addr)->sin_addr;

      /* get if flags
      **
      ** typical flags:
      ** lo    0x0049 -> Running, Loopback, Up
      ** ethx  0x1043 -> Multicast, Running, Broadcast, Up
      ** ipppx 0x0091 -> NoArp, PointToPoint, Up 
      ** grex  0x00C1 -> NoArp, Running, Up
      ** ipipx 0x00C1 -> NoArp, Running, Up
      */
      {
	struct ifreq IfReq;

	memcpy( IfReq.ifr_name, IfDescEp->Name, sizeof( IfReq.ifr_name ) );

	if( ioctl( Sock, SIOCGIFFLAGS, &IfReq ) < 0 )
	  log( LOG_ERR, errno, "ioctl SIOCGIFFLAGS" );

	IfDescEp->Flags = IfReq.ifr_flags;
      }

      log( LOG_DEBUG, 0, "buildIfVc: Interface %s Addr: %s, Flags: 0x%04x",
	   IfDescEp->Name,
	   fmtInAdr( FmtBu, IfDescEp->InAdr ),
	   IfDescEp->Flags );

      IfDescEp++; 
    } 
  }

  close( Sock );
}


struct IfDesc *getIfByName( const char *IfName )
/*
** Returns a pointer to the IfDesc of the interface 'IfName'
**
** returns: - pointer to the IfDesc of the requested interface
**          - NULL if no interface 'IfName' exists
**          
*/
{
  struct IfDesc *Dp;

  for( Dp = IfDescVc; Dp < IfDescEp; Dp++ ) 
    if( ! strcmp( IfName, Dp->Name ) ) 
      return Dp;

  return NULL;
}

struct IfDesc *getIfByIx( unsigned Ix )
/*
** Returns a pointer to the IfDesc of the interface 'Ix'
**
** returns: - pointer to the IfDesc of the requested interface
**          - NULL if no interface 'Ix' exists
**          
*/
{
  struct IfDesc *Dp = &IfDescVc[ Ix ];
  return Dp < IfDescEp ? Dp : NULL;
}



int chk_local( __u32 addr )
/*
** Returns a pointer to the IfDesc of the interface 'IfName'
**
** returns: - pointer to the IfDesc of the requested interface
**          - NULL if no interface 'IfName' exists
**          
*/
{
	struct IfDesc *Dp;

	for( Dp = IfDescVc; Dp < IfDescEp; Dp++ ) 
  		if( Dp->InAdr.s_addr == addr )
  			return 1;

  return 0;
}
#endif

