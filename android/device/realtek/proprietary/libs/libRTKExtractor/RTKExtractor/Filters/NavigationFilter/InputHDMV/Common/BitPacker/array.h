//------------------------------------------------------------
// array.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//------------------------------------------------------------

#ifndef _array_h_
#define _array_h_
#include "ScalarBitField.h"
#include "BitFieldArray.h"
#include "rt_stream.h"

#include <vector>
#include <algorithm>

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4355)
#endif

namespace bitpacker {

//	A BitField object isn't fully constructed unless its
//	layout or size is set (by the layout() function).
//	To set the layout/size for items within arrays, I
//	substitute a custom allocator.
template <class K>
class arrayAllocator : public std::allocator<K> {
public:
	// convert an arrayAllocator<K> to an arrayAllocator <_Other>
	template<class _Other>
	struct rebind {
		typedef arrayAllocator<_Other> other;
	};

	arrayAllocator() : mItemSize(0) {}
	arrayAllocator(const arrayAllocator& a) : std::allocator<K>(a), mItemSize(a.mItemSize) {}
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
	arrayAllocator(const size_t& itemSize) : mItemSize(itemSize) {}
// **CodeWizzard** - Possible Severe Violation:  Effective C++ item 16  - Assign to all data members in operator
	arrayAllocator& operator = (const arrayAllocator& a) { 
		if (this != &a){
			std::allocator<K>::operator=(a);
			mItemSize = a.mItemSize;
		}
		return *this; 
	}

	void construct(typename bitpacker::arrayAllocator<K>::pointer p, const K& val)
	{ 
		static_cast<std::allocator<K>&>(*this).construct(p, val);
		//	Just in case the item doesn't know its own size,
		//	tell it here.  This applies only to scalar fields.
		//	Arrays and structures know their sizes.
		if (dynamic_cast<const ScalarBitField*>(p)
			&& mItemSize != p->bitSize())
			p->layout(mItemSize);
	}

private:
	const size_t& mItemSize;
};

template <class T>
class array : 
	public BitFieldArray
{
public:
	typedef typename std::vector<T,arrayAllocator<T> > BaseVector;
   typedef typename BaseVector::iterator iterator;
   typedef typename BaseVector::const_iterator const_iterator;
	typedef typename BaseVector::size_type size_type;

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4355)	//	"this" used in init list
#endif
	
	array()
	: mItemSize(0),
      mArrayAllocator(mItemSize),
	  mVector(static_cast<arrayAllocator<T>&>(mArrayAllocator))
		
	{
	}

	array(const array &a)
	: 
      BitFieldArray(a),
		mVector(static_cast<arrayAllocator<T>&>(mArrayAllocator)),
		mItemSize(a.mItemSize), mArrayAllocator(a.mArrayAllocator)
	{
		mVector.resize(a.size());
		std::copy(a.begin(), a.end(), begin());
	}
	
#if defined(_WIN32)	
#pragma warning(pop)
#endif

	array& 
	operator=(const array &a){ 
		if (this != &a) {
			std::vector<T,arrayAllocator<T> >::operator=(a);
			BitFieldArray::operator=(a);
			mItemSize = a.mItemSize;
         mArrayAllocator = a.mArrayAllocator;
		}
		return *this; 
	}

	array& 
	operator=(const BaseVector &a){ 
		mItemSize(a.mItemSize);
		resize(a.size());
		std::copy(a.begin(), a.end(), begin());
	}

	struct accumulator {
		accumulator() : mSize(0) {}
		accumulator(size_t& offset) : mSize(&offset) {}
		void operator()(const T& t){ t.addOffset(*mSize); }
		size_t* mSize;
	};

	virtual void 
	addOffset(size_t& offset) const{
		BitPtr::byteAlign(offset);
		if (itemSize())
			offset += size()*itemSize();
		else
			std::for_each(begin(), end(), accumulator(offset));
	}

	virtual bool 
	bitOffset(size_t& offset, const BitField* field) const {
		if (this == field) 
			return true;
		BitPtr::byteAlign(offset);
		for (typename BaseVector::const_iterator it = begin(); it != end(); ++it)
			if (it->bitOffset(offset, field)) 
				return true;
		return false;
	}

	struct binder {
		binder() : mPtr(0) {}
		binder(ConstBitPtr& field) : mPtr(&field) {}
		void operator()(T& t){ t.bind(*mPtr); }
		ConstBitPtr* mPtr;
	};

	virtual void 
	bind(ConstBitPtr& p){
		mIsFixed = true;
		//	Round out to a byte boundary
		p.round();
		//	Bind all the elements
		std::for_each(begin(), end(), binder(p));
	}

	void 
	layoutArray(size_t base, size_t max, bool isFixed){
		BitFieldArray::layoutArray(base, max, isFixed);
		//	If we have a max, peg the size at that value.
		if (max && isFixed)
      {
         if (max < size())
            mVector.resize(max);
         else
         {
            while (size() < max)
            {
               T item;
               if (
//		dynamic_cast<const ScalarBitField*>(&item) &&
                  itemSize() != item.bitSize())
                  item.layout(itemSize());

               push_back(item);
            }
         }
      }
	}

	struct packer {
		packer() : mPtr(0) {}
		packer(BitPtr& field) : mPtr(&field) {}
		void operator()(const T& t){ t.pack(*mPtr); }
		BitPtr* mPtr;
	};

	virtual void 
	pack(BitPtr& p) const {
		//	Fill out to a byte boundary
		p.fill();
		//	pack all the elements
		std::for_each(begin(), end(), packer(p));
	}

	struct reader {
		reader() : mIs(0), mOffset(0) {}
		reader(rt_stream& is, size_t& offset)
			: mIs(&is), mOffset(&offset) {}
		void operator()(T& t){ t.read(*mIs, *mOffset); }
		rt_stream* 	mIs;
		size_t*			mOffset;
	};

	void 
	read(rt_stream& is, size_t& offset){
		//	Skip the stream to the next byte boundary
		if ((8-offset%8)%8 != 0)
			is.advance_read_pos(1);
		std::for_each(begin(), end(), reader(is, offset));
	}

	virtual void 
	print(rt_stream& os, int indent) const {
		leader(os, indent-1, "{") << newline;
		for (typename BaseVector::const_iterator it = begin(); it != end(); ++it)
			it->print(os, indent);
		leader(os, indent-1, "}") << newline;
	}

	void itemSize(size_t s) { mItemSize = s; }
	size_t itemSize() const { return mItemSize; }

	//	vector method overrides
	void resize(size_type newSize){
		checkResize(size(), newSize);
      if (newSize < size())
         mVector.resize(newSize);
      else
      {
         while (size() < newSize)
         {
            T item;
            if (
//	      dynamic_cast<const ScalarBitField*>(&item) &&
                itemSize() != item.bitSize())
               item.layout(itemSize());

            push_back(item);
         }
      }
		resizeEvent(newSize);
	}
	void resize(size_type newSize, const T& value){
		checkResize(size(), newSize);
		mVector.resize(newSize, value);
		resizeEvent(newSize);
	}
	const T& operator[](size_type index) const
		{ return mVector.operator[](mapIndex(index)); }
	T&       operator[](size_type index)
		{ return mVector.operator[](mapIndex(index)); }
	const T& at(size_type index) const
		{ return mVector.at(mapIndex(index)); }
	T&       at(size_type index)
		{ return mVector.at(mapIndex(index)); }

   // vector iterator accessor methods
   size_type size() const {return mVector.size();}
   iterator begin() {return mVector.begin();}
   
   const_iterator begin() const {return mVector.begin();}
   iterator end() {return mVector.end();}
   const_iterator end() const {return mVector.end();}
   void push_back(const T& val) {return mVector.push_back(val);}
   iterator erase(iterator where){return mVector.erase(where);}
   iterator erase(iterator first,iterator last){return mVector.erase(first,last);}
   iterator insert(iterator where,const T&val) {return mVector.insert(where,val);}

private:
	size_t			   mItemSize;
   arrayAllocator<T> mArrayAllocator;
   BaseVector        mVector;
};

}
#if defined(_WIN32)
#pragma warning(pop)
#endif

#endif // _array_h_


