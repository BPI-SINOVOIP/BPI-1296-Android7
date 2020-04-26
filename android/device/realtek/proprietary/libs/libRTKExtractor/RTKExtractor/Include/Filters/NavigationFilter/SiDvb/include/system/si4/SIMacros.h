/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: SI4
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04/23
 *
 *=======================================================================*/

#ifndef _SI_MACROS_H_
#define _SI_MACROS_H_

#ifdef SI_DEBUG
// Assertion
#define SI_ASSERT(x)						if(!(x)){CF_Logger_Log(0, CF_LL_ERROR, "Assertion Failed err. %s(%d)\n", __FILE__, __LINE__);return SI_ERR_ASSERTION;}

// Logger
#define SI_LOG(mod, logLevel, fmt, args...)	CF_Logger_Log(CF_MODID_SI_##mod, logLevel, fmt, ##args);

// Memory
#define SI_MALLOC(mod, size)				CF_Memory_Alloc(CF_MODID_SI_##mod, size);
#define SI_FREE(mod, p)						CF_Memory_Free(CF_MODID_SI_##mod, (p));
#else
// Assertion
//kjw: si is always run on SI_DEBUG mode at begining. so  if assert fail, return immediatly  is safer
#define SI_ASSERT(x)						if(!(x)){ return SI_ERR_ASSERTION;}

// Logger
#ifdef WIN32
#define SI_LOG
#else
#define SI_LOG(mod, logLevel, fmt, args...)
#endif

// Memory
#define SI_MALLOC(mod, size)				malloc(size);
#define SI_FREE(mod, p)					if (p) {free(p); (p) = NULL;}
#endif

// Begin & Return
#if SI_DEBUG_TRACE_FUNCTIONCALL
#define SI_BEGIN(mod)																			\
	do {																						\
		SI_##mod##_LOG(CF_LL_DEBUG, "= Begin Function = %s(%d)\n", __FUNCTION__, __LINE__);	\
	} while (0);

#define SI_RETURN(mod, var)																		\
	do {																						\
		SI_##mod##_LOG(CF_LL_DEBUG, "= End   Function = %s(%d)\n", __FUNCTION__, __LINE__);	\
		return (var);																			\
	} while (0);
#else
#define SI_BEGIN(mod)
#define SI_RETURN(mod, var)					return (var);
#endif

// Setting return varible
#define SI_SET_RETURN_VAR(p, v)				if(p){(*p)=v;}

// langCode
#define SI_GET_LANGCODE(p)					((SI_LANG)(((*(unsigned char*)((p)))<<16)|((*(unsigned char*)((p)+1))<<8)|((*(unsigned char*)((p)+2)))))

// New & Destroy obj
#define SI_NEW(mod, obj, class)											\
	do {																\
		obj = SI_##mod##_MALLOC(sizeof(class));							\
		memset(obj, 0, sizeof(class));									\
	} while(0);

#define SI_DESTROY(mod, obj)											\
	do {																\
		SI_##mod##_FREE(obj);											\
		obj = NULL;														\
	} while(0);

#define SI_MAX(a, b)					((a)>(b)?(a):(b))
#define SI_MIN(a, b)					((a)<(b)?(a):(b))

#define SI_SLEEP(ms)													\
	do{																	\
		struct timespec delay;											\
		delay.tv_sec = ms/1000;											\
		delay.tv_nsec = ((ms%1000)*1000000);							\
		nanosleep(&delay, NULL);										\
	} while(0);

#define SI_SPEC_GET_STANDARD(sp)			((sp & SI_ENUM_STANDARD_MASK)>>SI_ENUM_STANDARD_SHIFT)
#define SI_SPEC_GET_COUNTRY(sp)				((sp & SI_ENUM_COUNTRY_MASK)>>SI_ENUM_COUNTRY_SHIFT)
//in dvb spec, region id is 6 bits
#define SI_SPEC_GET_REGION_ID(sp)			(sp & SI_ENUM_REGIONID_MASK)
//assume last 6 bits is region id ( id & 0xfff to make sure id won't exceed 12 bits)
#define SI_SPEC_SET_REGION_ID(sp, id) 		(sp = (sp&0xffffffc0)|(id&SI_ENUM_REGIONID_MASK ) )

//related macro SI_GET_TIME_OFFSET
#define SI_GET_TIME_OFFSET(si) 	((si->timeOffset) )
#define SI_SET_TIME_OFFSET(si, newOffset) do { si->timeOffset = (newOffset); } while (0);


#define SI_MUTEX						pthread_mutex_t
#define SI_MUTEX_INIT(m)				pthread_mutex_init(&(m), NULL);
#define SI_MUTEX_LOCK(m)				pthread_mutex_lock(&(m));
#define SI_MUTEX_UNLOCK(m)				pthread_mutex_unlock(&(m));
#define SI_MUTEX_DESTROY(m)				pthread_mutex_destroy(&(m));


#define SI_SET_STATE(state, bitfields) ((state) |= (bitfields))
#define SI_RESET_STATE(state, bitfields) ((state) &= ~(bitfields))
#define SI_HAS_STATE(state, bitfields) (((state) & (bitfields)) == (bitfields))
#define SI_TEST_STATE(state, bitfields) ((state) & (bitfields))
#define SI_IS_STATE(state, bitfields) ((state) == (bitfields))

#define SI_SET_FLAG(si, bitfields) ((si->flag) |= (bitfields))
#define SI_RESET_FLAG(si, bitfields) ((si->flag) &= ~(bitfields))
#define SI_HAS_FLAG(si, bitfields) (((si->flag) & (bitfields)) == (bitfields))
#define SI_TEST_FLAG(si, bitfields) ((si->flag) & (bitfields))
//#define SI_IS_FLAG(state, bitfields) ((state) == (bitfields))


#endif


