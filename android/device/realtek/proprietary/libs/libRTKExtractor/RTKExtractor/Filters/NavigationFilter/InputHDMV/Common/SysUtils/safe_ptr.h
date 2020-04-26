//-----------------------------------------------------------------------------
// safe_ptr.h
// Copyright (c) 1997 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//SR FS: Reviewed [JAW  20040912]
//SR FS: Reviewed [wwt 20040914] 

#ifndef __safe_ptr_h__
#define __safe_ptr_h__

#include <cassert>

// replacement for auto_ptr that doesn't bark on copying - in addition,
// it will assert on an attempt to dereference a null pointer

template<class T>
class safe_ptr
{

public:

	safe_ptr(T * p = 0) : m_p(p) {}
	safe_ptr(const safe_ptr & sp) : m_p(sp.release()) {}
	~safe_ptr() { delete m_p; }
	safe_ptr & operator =(const safe_ptr & sp) { reset(sp.release()); return *this; }
	T &	operator *() const { assert(m_p); return *m_p; }
	T *	operator ->() const { assert(m_p); return m_p; }
	operator const void *() const { return m_p; }
	T *	get() const { return m_p; }
	T *	release() { T * p = m_p; m_p = 0; return p; }
	void reset(T * p = 0) { if (m_p != p) { delete m_p; m_p = p; } }

private:

	T *	release() const { T * p = m_p; m_p = 0; return p; }

	mutable T * m_p;
};

template<class T>
class safe_ptr_array
{

public:

	safe_ptr_array(T * p = 0) : m_p(p) {}
	safe_ptr_array(const safe_ptr_array & spa) : m_p(spa.release()) {}
	~safe_ptr_array() { delete[] m_p; }
	safe_ptr_array & operator =(const safe_ptr_array & spa) { reset(spa.release()); return *this; }
	T & operator [](size_t i) const { assert(m_p); return m_p[i]; } 
	operator const void *() const { return m_p; }
	T *	get() const { return m_p; }
	T *	release() { T * p = m_p; m_p = 0; return p; }
	void reset(T * p = 0) { if (m_p != p) { delete [] m_p; m_p = p; } }

private:

	T *	release() const { T * p = m_p; m_p = 0; return p; }

	mutable T * m_p;
};

#endif // __safe_ptr__
