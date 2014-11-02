#ifndef _SMPUNIT_H_
#define _SMPUNIT_H_

#include <setjmp.h>
#include <stdarg.h>
#include "smpUtil.h"

#define SMPEST_VERSION  "SMPTest 1.5"

/* SMPUnit Test struc and function */

typedef struct SMPTest SMPTest;

typedef void (*TestFunction)(SMPTest *);

struct SMPTest
{
	char* name;
	TestFunction function;
	int failed;
	int ran;
	const char* message;
	jmp_buf *jumpBuf;
};

void SMPTestInit(SMPTest* t, const char* name, TestFunction function);
SMPTest* SMPTestNew(const char* name, TestFunction function);
void SMPTestRun(SMPTest* tc);
void SMPTestDelete(SMPTest *t);

/* Internal versions of assert functions -- use the public versions */
void SMPFail_Line(SMPTest* tc, const char* file, int line, const char* message2, const char* message);
void SMPAssert_Line(SMPTest* tc, const char* file, int line, const char* message, int condition);
void SMPAssertStrEquals_LineMsg(SMPTest* tc, 
	const char* file, int line, const char* message, 
	const char* expected, const char* actual);
void SMPAssertIntEquals_LineMsg(SMPTest* tc, 
	const char* file, int line, const char* message, 
	int expected, int actual);
void SMPAssertDblEquals_LineMsg(SMPTest* tc, 
	const char* file, int line, const char* message, 
	double expected, double actual, double delta);
void SMPAssertPtrEquals_LineMsg(SMPTest* tc, 
	const char* file, int line, const char* message, 
	void* expected, void* actual);




/* common assert functions , you can add if you want*/

#define SMPFail(tc, ms)                        SMPFail_Line(  (tc), __FILE__, __LINE__, NULL, (ms))
#define SMPAssert(tc, ms, cond)                SMPAssert_Line((tc), __FILE__, __LINE__, (ms), (cond))
#define SMPAssertTrue(tc, cond)                SMPAssert_Line((tc), __FILE__, __LINE__, "assert failed", (cond))

#define SMPAssertStrEquals(tc,ex,ac)           SMPAssertStrEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define SMPAssertStrEquals_Msg(tc,ms,ex,ac)    SMPAssertStrEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define SMPAssertIntEquals(tc,ex,ac)           SMPAssertIntEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define SMPAssertIntEquals_Msg(tc,ms,ex,ac)    SMPAssertIntEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define SMPAssertDblEquals(tc,ex,ac,dl)        SMPAssertDblEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac),(dl))
#define SMPAssertDblEquals_Msg(tc,ms,ex,ac,dl) SMPAssertDblEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac),(dl))
#define SMPAssertPtrEquals(tc,ex,ac)           SMPAssertPtrEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define SMPAssertPtrEquals_Msg(tc,ms,ex,ac)    SMPAssertPtrEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))

#define SMPAssertPtrNotNull(tc,p)        SMPAssert_Line((tc),__FILE__,__LINE__,"null pointer unexpected",(p != NULL))
#define SMPAssertPtrNotNullMsg(tc,msg,p) SMPAssert_Line((tc),__FILE__,__LINE__,(msg),(p != NULL))

/* SMPSuite */

#define MAX_TEST_CASES	1024

#define SUITE_ADD_TEST(SUITE,TEST)	SMPSuiteAdd(SUITE, SMPTestNew(#TEST, TEST))

typedef struct
{
	int count;
	SMPTest* list[MAX_TEST_CASES];
	int failCount;

} SMPSuite;


void SMPSuiteInit(SMPSuite* testSuite);
SMPSuite* SMPSuiteNew(void);
void SMPSuiteDelete(SMPSuite *testSuite);
void SMPSuiteAdd(SMPSuite* testSuite, SMPTest *testCase);
void SMPSuiteAddSuite(SMPSuite* testSuite, SMPSuite* testSuite2);
void SMPSuiteRun(SMPSuite* testSuite);
void SMPSuiteSummary(SMPSuite* testSuite, SMPString* summary);
void SMPSuiteDetails(SMPSuite* testSuite, SMPString* details);

#endif /* CU_TEST_H */
