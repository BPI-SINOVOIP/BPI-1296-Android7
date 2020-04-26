/*!
   @file
   @verbatim

LICENSE

Copyright (c) 2003 Asim Jalis

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in
a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not
be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

**/

/*!
	@verbatim
	
Additional modifications made to original file by DivX Inc. 2007, 
in accordance with the above license.

**/


#ifndef CU_TEST_H
#define CU_TEST_H

#include <setjmp.h>
#include <stdarg.h>

/* CuString */

char* CuStrAlloc(int size);
char* CuStrCopy(const char* old);

#define CU_ALLOC(TYPE)		((TYPE*) malloc(sizeof(TYPE)))

#define HUGE_STRING_LEN	8192
#define STRING_MAX		256
#define STRING_INC		256

typedef struct
{
	int length;
	int size;
	char* buffer;
} CuString;

void CuStringInit(CuString* str);
CuString* CuStringNew(void);
void CuStringRead(CuString* str, const char* path);
void CuStringAppend(CuString* str, const char* text);
void CuStringAppendChar(CuString* str, char ch);
void CuStringAppendFormat(CuString* str, const char* format, ...);
void CuStringInsert(CuString* str, const char* text, int pos);
void CuStringResize(CuString* str, int newSize);

/* CuTest */

typedef struct CuTest CuTest;
typedef struct CuSuite CuSuite;
typedef struct CuSuiteRegistry CuSuiteRegistry;
typedef void (*TestFunction)(CuTest *);

struct CuTest
{
	const char* name;
	TestFunction function;
	int failed;
	int ran;
	const char* message;
	jmp_buf *jumpBuf;
	void (*setUpFunction)();
	void (*tearDownFunction)();
	CuSuite *pSuite;
};

void CuTestInit(CuTest* t, const char* name, TestFunction function);
CuTest* CuTestNew(const char* name, TestFunction function);
void CuTestRun(CuTest* tc);

/* Internal versions of assert functions -- use the public versions */
void CuFail_Line(CuTest* tc, const char* file, int line, const char* message2, const char* message);
void CuAssert_Line(CuTest* tc, const char* file, int line, const char* message, int condition);
void CuAssertStrEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	const char* expected, const char* actual);
void CuAssertIntEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	int expected, int actual);
void CuAssertDblEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	double expected, double actual, double delta);
void CuAssertPtrEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	void* expected, void* actual);

/* public assert functions */

#define CuFail(tc, ms)                        CuFail_Line(  (tc), __FILE__, __LINE__, NULL, (ms))
#define CuAssert(tc, ms, cond)                CuAssert_Line((tc), __FILE__, __LINE__, (ms), (cond))
#define CuAssertTrue(tc, cond)                CuAssert_Line((tc), __FILE__, __LINE__, "assert failed", (cond))

#define CuAssertStrEquals(tc,ex,ac)           CuAssertStrEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define CuAssertStrEquals_Msg(tc,ms,ex,ac)    CuAssertStrEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define CuAssertIntEquals(tc,ex,ac)           CuAssertIntEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define CuAssertIntEquals_Msg(tc,ms,ex,ac)    CuAssertIntEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define CuAssertDblEquals(tc,ex,ac,dl)        CuAssertDblEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac),(dl))
#define CuAssertDblEquals_Msg(tc,ms,ex,ac,dl) CuAssertDblEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac),(dl))
#define CuAssertPtrEquals(tc,ex,ac)           CuAssertPtrEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define CuAssertPtrEquals_Msg(tc,ms,ex,ac)    CuAssertPtrEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))

#define CuAssertPtrNotNull(tc,p)        CuAssert_Line((tc),__FILE__,__LINE__,"null pointer unexpected",(p != NULL))
#define CuAssertPtrNotNullMsg(tc,msg,p) CuAssert_Line((tc),__FILE__,__LINE__,(msg),(p != NULL))

/* CuSuite */

#define MAX_TEST_CASES	1024

#define SUITE_ADD_TEST(SUITE,TEST)	CuSuiteAdd(SUITE, CuTestNew(#TEST, TEST))

/* Modified original structure to give it a name and add setup and teardown function */
struct CuSuite
{
	const char* name;
	int count;
	CuTest* list[MAX_TEST_CASES];
	int runCount;
	int failCount;
	void (*setUpFunction)();
	void (*tearDownFunction)();
	CuSuiteRegistry *pRegistry;
};

struct CuSuiteRegistry
{
	int count;
	int verbose;
	CuSuite* suites;
};

/* 
Global pointer to test case, so CPPUNIT compatible macros can be called in functions called by test cases without having to 
to pass the test case pointer to each function 
*/
extern CuTest   *gTc;
extern int      g_CuTestErrors;

void CuSuiteInit(CuSuite* testSuite);
CuSuite* CuSuiteNew(void);
void CuSuiteAdd(CuSuite* testSuite, CuTest *testCase);
void CuSuiteAddSuite(CuSuite* testSuite, CuSuite* testSuite2);
void CuSuiteRun(CuSuite* testSuite);
void CuSuiteSummary(CuSuite* testSuite, CuString* summary);
void CuSuiteDetails(CuSuite* testSuite, CuString* details);

/* Addition of MACROS to make it code look more like CPPUNIT tests */
CuSuite* CuSuiteNamedNew(const char *name);
/* Global registration suite */
CuSuiteRegistry registry,*pRegistry;

/*Local suite to source file this is included into */
static CuSuite* pCuSuite;

/* Must provide setUp and tearDown functions, even if only empty */
static void setUp(CuTest *tc);
static void tearDown(CuTest *tc);

void CuRegistryAddSuite(CuSuiteRegistry* registry, CuSuite* testSuite);
void CuSuiteRegistrySummary(const CuSuiteRegistry* registry, CuString* summary);
void CuSuiteRegistryDetails(const CuSuiteRegistry* registry, CuString* details);
void CuRegistryRun(const CuSuiteRegistry* registry);
CuTest *CuFindTest(const CuSuiteRegistry* pReg, const char *testName);
void CuRegistryRunTests(const CuSuiteRegistry* pReg, const char *testName);

/* CPPUNIT like MACROS */
#define TEST_SUITE(SUITENAME) void SUITENAME(){ \
	pCuSuite = CuSuiteNamedNew(#SUITENAME);\
	pCuSuite->setUpFunction = (void *)0;\
	pCuSuite->tearDownFunction=(void *)0;\

#define ADD_TEST(TESTNAME)      SUITE_ADD_TEST(pCuSuite,TESTNAME)
#define ADD_SETUP(SETUP) 	    (pCuSuite->setUpFunction = SETUP)
#define ADD_TEARDOWN(TEARDOWN) 	(pCuSuite->tearDownFunction = TEARDOWN)

#define TEST_SUITE_END() CuRegistryAddSuite(pRegistry,pCuSuite);}

/* Must create the global registry only once */
#define CREATE_REGISTRY() registry.count=0;registry.verbose=0;registry.suites=(CuSuite*)0; pRegistry = &registry; gTc =(void*)0
/* Must register the suites created in a function to get them to run */
#define TEST_SUITE_REGISTRATION(SUITENAME) SUITENAME();
#define VERBOSE_OUTPUT() (registry.verbose=1)

/* Assert MACROS to reproduce CPPUNIT versions */
#define ASSERT(cond) CuAssertTrue(gTc,cond)
#define ASSERT_MESSAGE(ms, cond) CuAssert(gTc, ms, cond)
#define ASSERT_EQUALS_STR(ex,ac) CuAssertStrEquals(gTc,ex,ac)
#define ASSERT_EQUALS_INT(ex,ac) CuAssertIntEquals(gTc,ex,ac)
#define ASSERT_EQUALS_DBL(ex,ac) CuAssertDblEquals(gTc,ex,ac)
#define ASSERT_EQUALS_PTR(ex,ac) CuAssertPtrEquals(gTc,ex,ac)

#define CPPUNIT_ASSERT(cond) ASSERT(cond)
#define CPPUNIT_ASSERT_MESSAGE(ms, cond) CuAssert(gTc, ms, cond)
#define CPPUNIT_ASSERT_EQUALS_STR(ex,ac) CuAssertStrEquals(gTc,ex,ac)
#define CPPUNIT_ASSERT_EQUALS_INT(ex,ac) CuAssertIntEquals(gTc,ex,ac)
#define CPPUNIT_ASSERT_EQUALS_DBL(ex,ac) CuAssertDblEquals(gTc,ex,ac)
#define CPPUNIT_ASSERT_EQUALS_PTR(ex,ac) CuAssertPtrEquals(gTc,ex,ac)

#define CPPUNIT_TEST_SUITE_REGISTRATION(SUITENAME) TEST_SUITE_REGISTRATION(SUITENAME)

#define CPPUNIT_TEST_SUITE(SUITENAME) TEST_SUITE(SUITENAME)
#define CPPUNIT_TEST(TESTNAME) ADD_TEST(TESTNAME)
#define CPPUNIT_TEST_SUITE_END() TEST_SUITE_END() 
#endif /* CU_TEST_H */
