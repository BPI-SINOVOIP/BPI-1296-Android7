//-----------------------------------------------------------------------------
// IDFBBase.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef IDFBBASE_H
#define IDFBBASE_H

//#define PROFILE_DFB // enable profiling calls in all IDFBXyz classes

#include <directfb_version.h>
#include <dfb_types.h>
#include <directfb_keyboard.h>
#include <directfb.h>

#define DFBRESULT(cond)			do { DFBResult __res__=(cond); if (__res__ != DFB_OK) return __res__; } while (0)
#define DFBASSERT(cond, res)	do {if (!(cond)) return res;} while (0)

#if defined(_DEBUG) && 0
#define	TRACE_DFB(x)	x
#else
#define	TRACE_DFB(x)
#endif

template <class T>
class IDFBBase
{
	public:
		virtual ~IDFBBase() 
		{
			TRACE_DFB( printf("IDFBBase::~IDFBBase this=%p, dfbif=%p\n", this, dfbif); )
			if (dfbif)
			{
				dfbif->Release( dfbif ); dfbif = 0;
			}
		}

		inline operator T*() const	{ return dynamic_cast<T*>(this); }
		inline operator T*()		{ return dynamic_cast<T*>(this); }
		inline operator bool()		{ return dfbif != 0; }

		inline T	*GetInterface() const	{ return dfbif; }
		inline T	*GetInterface()			{ return dfbif; }

	protected:
		IDFBBase(): dfbif (0) {}

		IDFBBase(const T &dfbif)  
        {
            TRACE_DFB( printf("IDFBBase::IDFBBase(const T &dfbif) this=%p, dfbif=%p, dfbif.dfbif=%p\n", this, dfbif, dfbif.dfbif); )

            if (dfbif.dfbif) 
				dfbif.dfbif->AddRef( dfbif.dfbif );
			this->dfbif = dfbif.dfbif;
		}
		
		inline IDFBBase(T *dfbif) 	
        {   
            TRACE_DFB( printf("IDFBBase::IDFBBase(T *dfbif) this=%p, dfbif=%p\n", this, dfbif); )

            this->dfbif = dfbif;
			if (this->dfbif)
				this->dfbif->AddRef( this->dfbif );
		}
		
		inline T &operator = (const T &dfbif) 
		{
            TRACE_DFB( printf("IDFBBase::&Operator =(const T &dfbif) this=%p, dfbif=%p, dfbif.dfbif=%p\n", this, dfbif, dfbif.dfbif); )

            if (dfbif.dfbif)
                dfbif.dfbif->AddRef( dfbif.dfbif );
			if (this->dfbif)
				this->dfbif->Release( this->dfbif );
			this->dfbif = dfbif.dfbif;

            return dynamic_cast<T&>(*this);
		}

	protected:
		T	*dfbif;
};


#endif
