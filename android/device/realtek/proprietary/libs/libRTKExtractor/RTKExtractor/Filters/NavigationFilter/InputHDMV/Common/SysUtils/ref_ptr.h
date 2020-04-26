//-----------------------------------------------------------------------------
// ref_ptr.h
// Copyright (c) 1998 - 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __ref_ptr_h__
#define __ref_ptr_h__

#include "SonicCounter.h"
#include <cassert>

class ref_ptr_base
{

public:

	SInt32 count() const { return m_counter ? m_counter->Count() : 0; }

protected:

	ref_ptr_base() : m_counter(0) {}
	ref_ptr_base(const ref_ptr_base & rpb) : m_counter(rpb.m_counter) {}
	~ref_ptr_base() { if (m_counter) delete m_counter; }
	ref_ptr_base & operator =(const ref_ptr_base & rpb) { assert(m_counter == 0); m_counter = rpb.m_counter; return *this; }
	void inc() { if (m_counter) m_counter->Increment(); }
	void init() { assert(m_counter == 0); m_counter = new SonicCounter(1); }
	SInt32 release()
	{
		if (m_counter)
		{
			SInt32 count = m_counter->Decrement();
			if (count == 0) delete m_counter;
			m_counter = 0;
			return count;
		}

		return 0;
	}

private:

	SonicCounter * m_counter;
};

template<class T>
class ref_ptr : public ref_ptr_base
{

public:

	ref_ptr() : ref_ptr_base(), m_p(0) {}
	ref_ptr(const ref_ptr<T> & rp) : ref_ptr_base(rp), m_p(rp.m_p) { inc(); }

	ref_ptr(T * p) : ref_ptr_base(), m_p(0)
	{
		try
		{
			if (p)
			{
				init();
				m_p = p;
			}
		} 
		catch (...)
		{
			delete p;
			throw;
		}
	}

	~ref_ptr() { release(); }
	ref_ptr & operator =(T * p) { set(p); return *this; }

	ref_ptr & operator =(const ref_ptr & rp)
	{
		if (this != &rp)
		{
			release();
			ref_ptr_base::operator =(rp);
			m_p = rp.m_p;
			inc();
		}

		return *this;
	}

	template<class X>
	ref_ptr(const ref_ptr<X> & rp) : ref_ptr_base(rp)
	{
		m_p = dynamic_cast<T *>(rp.get());
		inc();
	}

	template<class X>
	ref_ptr & operator =(const ref_ptr<X> & rp)
	{
		// cast is necessary to avoid compiler errors when <T> != <X>
		if (this != reinterpret_cast<void const *>(&rp))
		{
			release();
			m_p = dynamic_cast<T *>(rp.get());
			ref_ptr_base::operator =(rp);
			inc();
		}

		return *this;
	}

	operator const void *() const { return m_p; }
	T & operator *() const { assert(m_p); return *m_p; }
	T * operator ->() const { assert(m_p); return m_p; }
	T *	get() const { return m_p; }
	void set(T * p) { reset(p); }
	void reset(T * p = 0)
	{
		try
		{
			release();
			if (p)
			{
				init();
				m_p = p;
			}
		} 
		catch (...)
		{
			delete p;
			throw;
		}
	}

protected:

	void release()
	{
		if (ref_ptr_base::release() == 0) delete m_p;
		m_p = 0;
	}

private:

	T * m_p;
};

template<class T>
class ref_ptr_array : public ref_ptr_base
{

public:

	ref_ptr_array() : ref_ptr_base(), m_p(0) {}
	ref_ptr_array(const ref_ptr_array & rpa) : ref_ptr_base(rpa), m_p(rpa.m_p) { inc(); }
	ref_ptr_array(T * p) : ref_ptr_base(), m_p(0)
	{
		try
		{
			if (p)
			{
				init();
				m_p = p;
			}
		} 
		catch (...)
		{
			delete[] p;
			throw;
		}
	}

	~ref_ptr_array() { release(); }
	ref_ptr_array & operator =(const ref_ptr_array & rpa)
	{
		if (this != &rpa)
		{
			release();
			ref_ptr_base::operator =(rpa);
			m_p = rpa.m_p;
			inc();
		}

		return *this;
	}

	operator const void *() const { return m_p; }
	T & operator [] (int i) const { assert(m_p); return m_p[i]; }
	T * get() const { return m_p; }
	void set(T * p) { reset(p); }
	void reset(T * p = 0)
	{
		try
		{
			release();
			if (p)
			{
				init();
				m_p = p;
			}
		} 
		catch (...)
		{
			delete[] p;
			throw;
		}
	}

private:

	void release()
	{
		if (ref_ptr_base::release() == 0) delete[] m_p;
		m_p = 0;
	};

	T * m_p;
};

#endif // __ref_ptr_h__
