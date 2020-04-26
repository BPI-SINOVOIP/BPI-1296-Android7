//-----------------------------------------------------------
// ulListSingle.h - single link list class
//
//		ulCLinkedListSingle< int, sizeof(int) >	IntList;
//
//------------------------------------------------------------

#ifndef _ULLIST_SINGLE_H
#define _ULLIST_SINGLE_H

#include <string.h>
#include <stdlib.h>
//#include <OSAL.h>
//#include <Util.h>

//
// node class
//
template <class T, int i> 
class ulCLinkedListSingleNode
{
public:
	ulCLinkedListSingleNode< T, i >	*pNext;
	T				Data;
public:
	ulCLinkedListSingleNode();
	~ulCLinkedListSingleNode();
	bool	IsEqual(T&);
        void * operator new (size_t size)
        {
            void * ptr;
            ptr = (void *)malloc(size);
            if (!ptr) return ptr;
            memset(ptr, '\0', size);
            return ptr;
        }

        void  operator delete(void * obj)
        {
            free(obj);
        }
};

//
// list class
//
template <class T, int i> 
class ulCLinkedListSingle
{
	unsigned int 		Size;

public:
	ulCLinkedListSingleNode< T, i >	*pHead;

public:
	ulCLinkedListSingle();
	~ulCLinkedListSingle();

	bool Initialize();
	bool Free();
	
	bool Add(T&);
	bool Remove(T&);
	bool Remove(unsigned int);

	bool Insert(unsigned int, T&);
	unsigned int GetSize(void);
};


//////////////////////////////////////////////////////////////////////////////////////////
//
// implementation of node
//

//
// construction/destruction
//
template <class T, int i> 
ulCLinkedListSingleNode< T, i >::ulCLinkedListSingleNode()
{ 
	pNext	= NULL; 
}

template <class T, int i> 
ulCLinkedListSingleNode< T, i >::~ulCLinkedListSingleNode()			
{ 
	pNext	= NULL; 
}

template <class T, int i> 
bool ulCLinkedListSingleNode< T, i >::IsEqual(T& Data)
{
	for( unsigned int  Index( 0 ); Index < i; Index++ )
		if( ((char*)&(this->Data))[ Index ] != ((char*)&Data)[ Index ] )
			//
			// not equal
			//
			return false;

	//
	// the data is equal
	//
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// implementation of list
//

//
// construction/destruction
//
template <class T, int i> 
ulCLinkedListSingle< T, i >::ulCLinkedListSingle()
{
	pHead	= NULL;
	Size	= 0;
}

template <class T, int i> 
ulCLinkedListSingle< T, i >::~ulCLinkedListSingle()
{
	pHead	= NULL;
	Size	= 0;
}

//
// initialization
//
template<typename T, int i>
bool ulCLinkedListSingle<T,i>::Initialize(void)
{
	//
	// if the list is not empty then free it
	//
	if( ( 0 != (int)Size ) ||
		( NULL != (void *)pHead ) )
	{
		Free();
	}

	pHead	= NULL;
	Size	= 0;

	return true;
}

//
// release
//
template<typename T, int i>
bool ulCLinkedListSingle<T,i>::Free(void)
{

	//
	// start from head and delete each node
	//

	for( ; NULL != pHead; )
	{
		ulCLinkedListSingleNode<T,i>	*pSave = pHead->pNext;
		//
		// delete the current head
		//
		if( NULL != pHead )
			delete (ulCLinkedListSingleNode<T,i>*)pHead;

		//
		// move head to next node
		//
		pHead	= pSave;
	}

	pHead	= NULL;
	Size	= 0;

	return true;	
}

//
// add node
//
template<typename T, int i>
bool ulCLinkedListSingle<T,i>::Add(T& Data)
{

	
		//
		// allocate memory for the new node
		//
		ulCLinkedListSingleNode<T,i>	*pNew;

		pNew	= new ulCLinkedListSingleNode<T,i>();
		if( NULL == pNew )
		{
			//
			// cannot allocate memory... so, we return false
			//			
			return false;
		}

		//
		// copy data
		//
		////CopyMemory( &(pNew->Data), &Data, (SIZE_T)i );
                if(&Data)
		    memcpy( &(pNew->Data), &Data, i );

		//
		// add node to list
		//
		pNew->pNext	= pHead;
		pHead		= pNew;

		Size++;
		
		return true;	
}



//
// remove node by Data
//
template<typename T, int i>
bool ulCLinkedListSingle<T,i>::Remove(T& Data)
{
	{
		if( pHead->IsEqual( Data ) )
		{
			//
			// delete the head
			//
			ulCLinkedListSingleNode<T,i>	*pSave = pHead->pNext;

			if( NULL != pHead )
				delete (ulCLinkedListSingleNode<T,i>*)pHead;

			pHead		= pSave;

			Size--;

			//
			// the node was found and removed
			//	
			return true;
		}


		//
		// if it's not the head, then search for it
		//
		ulCLinkedListSingleNode<T,i>	*pPrevious = pHead;

		for( ulCLinkedListSingleNode<T,i> *pCursor = pHead->pNext; NULL != pCursor; pCursor = pCursor->pNext )
		{
			if( pCursor->IsEqual( Data ) )
			{
				ulCLinkedListSingleNode<T,i>	*pSave = pCursor->pNext;

				if( NULL != pPrevious )		// it might be the head of the list
					pPrevious->pNext	= NULL;
				pCursor->pNext		= NULL;

				if( NULL != pCursor )
					delete (ulCLinkedListSingleNode<T,i>*)pCursor;


				pCursor				= pSave;
				if( NULL != pPrevious )		// it might be the head of the list
					pPrevious->pNext	= pCursor;

				Size--;

				//
				// the node was found and removed
				//				
				return true;
			}

			pPrevious	= pCursor;
		}

		//
		// the node was not found
		//		
	}

	return false;
}

//
// remove node by Position
//
template<typename T, int i>
bool ulCLinkedListSingle<T,i>::Remove(unsigned int Position)
{		
	{
		if( Position > Size )
		{		
			return false;
		}


		if( 0 == Position )
		{
			//
			// delete the head
			//
			ulCLinkedListSingleNode<T,i>	*pSave = pHead->pNext;

			if( NULL != pHead )
				delete (ulCLinkedListSingleNode<T,i>*)pHead;

			pHead		= pSave;

			Size--;

			//
			// the node was found and removed
			//			
			return true;
		}



		//
		// if it's not the head, then search for it
		//
		ulCLinkedListSingleNode<T,i>	*pPrevious = pHead;
		unsigned int						Index( 1 );

		for( ulCLinkedListSingleNode<T,i> *pCursor = pHead->pNext; NULL != pCursor; pCursor = pCursor->pNext, Index++ )
		{
			if( Index == Position )
			{
				ulCLinkedListSingleNode<T,i>	*pSave = pCursor->pNext;

				if( NULL != pCursor )
					delete (ulCLinkedListSingleNode<T,i>*)pCursor;

				pCursor				= pSave;
				pPrevious->pNext	= pCursor;

				Size--;

				//
				// the node was found and removed
				//				
				return true;
			}

			pPrevious	= pCursor;
		}

		//
		// the node was not found
		//		
	}

	return false;
}





//
// insert node at specified position
//
template<typename T, int i>
bool ulCLinkedListSingle<T,i>::Insert(unsigned int Position, T& Data)
{	
	{
		if( Position > Size )
		{		
			return false;
		}


		ulCLinkedListSingleNode<T,i>	*pPrevious = NULL;
		unsigned int						Index( 0 );

		for( ulCLinkedListSingleNode<T,i> *pCursor = pHead; NULL != pCursor; pCursor = pCursor->pNext, Index++ )
		{
			if( Index == Position )
			{
				//
				// insert the new node
				//

				//
				// allocate memory for the new node
				//
				ulCLinkedListSingleNode<T,i>	*pNew = NULL;

				pNew	= new ulCLinkedListSingleNode<T,i>();
				if( NULL == pNew )
				{					
					return false;
				}

				//
				// copy data
				//
				//CopyMemory( &(pNew->Data), &Data, i );
                                if(&Data)
				    memcpy(&(pNew->Data), &Data, i );


				if( 0 == Index )
				{
					//
					// if Index == 0 then this is the new head
					//
					pNew->pNext			= pHead;
					pHead				= pNew;
				}
				else
				{
					pNew->pNext			= pCursor;
					pPrevious->pNext	= pNew;
				}

				Size++;

				//
				// the node was found and removed
				//				
				return true;
			}

			pPrevious	= pCursor;
		}

		//
		// if we got here it means that Position == Size
		// and this should be inserted after the last node
		//


		//
		// allocate memory for the new node
		//
		ulCLinkedListSingleNode<T,i>	*pNew = NULL;

		pNew	= new ulCLinkedListSingleNode<T,i>();
		if( NULL == pNew )
		{			
			return false;
		}

		//
		// copy data
		//
		//CopyMemory( &(pNew->Data), &Data, i );
		memcpy( &(pNew->Data), &Data, i );

		//
		// link it to tail
		//
		pNew->pNext			= NULL;
		pPrevious->pNext	= pNew;

		Size++;


		//
		// we should never get here
		//		
	}

	return false;
}


//
// get the size of list
//
template<typename T, int i>
unsigned int ulCLinkedListSingle<T,i>::GetSize(void)
{
	return Size;
}

#endif // _ULLIST_SINGLE_H

