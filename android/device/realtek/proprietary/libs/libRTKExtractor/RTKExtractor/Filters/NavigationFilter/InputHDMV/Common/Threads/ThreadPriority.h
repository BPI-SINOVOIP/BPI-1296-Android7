#ifndef __THREAD_PRIORITY_h__
#define __THREAD_PRIORITY_h__

enum Priority
	{
		Priority_Lowest		= 2,
		Priority_Low		= 1,
		Priority_Normal		= 0,
		Priority_High		= -1,
		Priority_Highest	= -2,
		Priority_RealTime	= -3
	};


#endif //__THREAD_PRIORITY_h__
