////////////////////////////////////////////////////////////////////////////////
/// Smart pointer class 
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_AUTO_XXX_PTR__
#define __INCLUDED_AUTO_XXX_PTR__

#ifdef WIN32
#else
    #define FOR_GCC_COMPILER_WORK_AROUND
    #include <sys/types.h>
#endif

#include "os/CrossPlatformDecl.h"

namespace rtk   {
namespace tools {

template<class T> inline void DeletePtr(T* pT) { delete pT; }
template<class T> inline void DeleteArray(T* pT) { delete[] pT; }

template<class T, class Fn, Fn fnDelete> class auto_ptr_base
{
    typedef auto_ptr_base<T, Fn, fnDelete> _T;

    protected:
        T m_pPtr;

    public:
        explicit auto_ptr_base(T pPtr = NULL) throw() : m_pPtr(pPtr) { }

        auto_ptr_base(auto_ptr_base& Ptr) throw() : m_pPtr(Ptr.release()) { }

        bool operator!() { return m_pPtr == NULL; }

        operator T() const { return m_pPtr; };

        _T& operator=(_T& Ptr) throw()
        {
            reset(Ptr.release());
            return *this;
        }

        ~auto_ptr_base() 
        { 
            fnDelete(m_pPtr);
        }

        T operator->() const throw() { return m_pPtr; }

        T get() const throw() { return m_pPtr; }

        T release() throw()
        {
            T pTmp = m_pPtr;
            m_pPtr = NULL;
            return pTmp;
        }

        void reset(T pPtr = NULL) throw()
        {
            if (pPtr != m_pPtr)
            {
                fnDelete(m_pPtr);
                m_pPtr = pPtr;
            }
        }
};

template<class T> class auto_ptr: public auto_ptr_base<T*, void (*)(T*), DeletePtr>
{
    typedef auto_ptr_base<T*, void (*)(T*), DeletePtr> _TBase;
    
    public:
        explicit auto_ptr(T* pPtr) throw() : _TBase(pPtr) { }
};

#ifndef FOR_GCC_COMPILER_WORK_AROUND

// gcc cannot compile the following code :S. But in VC++ 2008, this code works well.
// It exists some bugs:
//    Error message: 
//      internal compiler error: in cp_tree_equal, at cp/tree.c:1633
//      Please submit a full bug report,
//      with preprocessed source if appropriate.
//      See <URL:http://gcc.gnu.org/bugs.html> for instructions.
//
template<class T> class auto_array_ptr: public auto_ptr_base<T*, void (*)(T*), DeleteArray>
{
    typedef auto_ptr_base<T*, void (*)(T*), DeleteArray> _TBase;
    
    public:
        explicit auto_array_ptr(T* pPtr) throw() : _TBase(pPtr) { }
        
        T& operator[](size_t i) throw() { return this->m_pPtr[i]; };

        const T& operator[](size_t i) const throw() { return this->m_pPtr[i]; };
};

#else // #ifndef FOR_GCC_COMPILER_WORK_AROUND

// I know this is ugly, but gcc exists bugs. I have no choice :(
//
template<class T> class auto_array_ptr
{
    typedef auto_array_ptr<T> _T;

    protected:
        T* m_pPtr;

    public:
        explicit auto_array_ptr(T* pPtr = NULL) throw() : m_pPtr(pPtr) { }

        auto_array_ptr(auto_array_ptr& Ptr) throw() : m_pPtr(Ptr.release()) { }

        bool operator!() { return m_pPtr == NULL; }

        operator T*() const { return m_pPtr; };

        _T& operator=(_T& Ptr) throw()
        {
            reset(Ptr.release());
            return *this;
        }

        ~auto_array_ptr() 
        { 
            delete[] m_pPtr;
        }

        T* operator->() const throw() { return m_pPtr; }

        T* get() const throw() { return m_pPtr; }

        T* release() throw()
        {
            T* pTmp = m_pPtr;
            m_pPtr = NULL;
            return pTmp;
        }

        void reset(T* pPtr = NULL) throw()
        {
            if (pPtr != m_pPtr)
            {
                delete[] m_pPtr;
                m_pPtr = pPtr;
            }
        }
        
        T& operator[](size_t i) throw() { return m_pPtr[i]; };

        const T& operator[](size_t i) const throw() { return m_pPtr[i]; };
};

#endif // #ifndef FOR_GCC_COMPILER_WORK_AROUND

}; // namespace tools
}; // namespace rtk

#endif
