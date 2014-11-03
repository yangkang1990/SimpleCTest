#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "smpUnit.h"
#include "smpUtil.h"


/*-------------------------------------------------------------------------*
 * SMPTest
 *-------------------------------------------------------------------------*/

void SMPTestInit(SMPTest* t, const char* name, TestFunction function)
{
	t->m_szname =SMPStrCopy(name);
	t->m_bfailed = 0;
	t->m_bran = 0;
	t->m_szmessage = NULL;
	t->m_pfunction = function;
	t->m_pjumpBuf = NULL;
}

SMPTest* SMPTestNew(const char* name, TestFunction function)
{
	SMPTest* tc = SMP_ALLOC(SMPTest);
	SMPTestInit(tc, name, function);
	return tc;
}

void SMPTestDelete(SMPTest *t)
{
        if (!t) return;
        free(t->m_szname);
        free(t);
}

void SMPTestRun(SMPTest* tc)
{
	jmp_buf buf;
	tc->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		tc->m_bran = 1;
		(tc->m_pfunction)(tc);
	}
	tc->m_pjumpBuf = 0;
}

static void SMPFailInternal(SMPTest* tc, const char* file, int line, SMPString* string)
{
	char buf[HUGE_STRING_LEN];

	sprintf(buf, "%s:%d: ", file, line);
	SMPStringInsert(string, buf, 0);

	tc->m_bfailed = 1;
	tc->m_szmessage = string->buffer;
	if (tc->m_pjumpBuf != 0) longjmp(*(tc->m_pjumpBuf), 0);
}

void SMPFail_Line(SMPTest* tc, const char* file, int line, const char* message2, const char* message)
{
	SMPString string;

	SMPStringInit(&string);
	if (message2 != NULL) 
	{
		SMPStringAppend(&string, message2);
		SMPStringAppend(&string, ": ");
	}
	SMPStringAppend(&string, message);
	SMPFailInternal(tc, file, line, &string);
}

void SMPAssert_Line(SMPTest* tc, const char* file, int line, const char* message, int condition)
{
	if (condition) return;
	SMPFail_Line(tc, file, line, NULL, message);
}

void SMPAssertStrEquals_LineMsg(SMPTest* tc, const char* file, int line, const char* message, 
	const char* expected, const char* actual)
{
	SMPString string;
	if ((expected == NULL && actual == NULL) ||
	    (expected != NULL && actual != NULL &&
	     strcmp(expected, actual) == 0))
	{
		return;
	}

	SMPStringInit(&string);
	if (message != NULL) 
	{
		SMPStringAppend(&string, message);
		SMPStringAppend(&string, ": ");
	}
	SMPStringAppend(&string, "expected <");
	SMPStringAppend(&string, expected);
	SMPStringAppend(&string, "> but was <");
	SMPStringAppend(&string, actual);
	SMPStringAppend(&string, ">");
	SMPFailInternal(tc, file, line, &string);
}

void SMPAssertIntEquals_LineMsg(SMPTest* tc, const char* file, int line, const char* message, 
	int expected, int actual)
{
	char buf[STRING_MAX];
	if (expected == actual) return;
	sprintf(buf, "expected <%d> but was <%d>", expected, actual);
	SMPFail_Line(tc, file, line, message, buf);
}

void SMPAssertDblEquals_LineMsg(SMPTest* tc, const char* file, int line, const char* message, 
	double expected, double actual, double delta)
{
	char buf[STRING_MAX];
	if (fabs(expected - actual) <= delta) return;
	sprintf(buf, "expected <%f> but was <%f>", expected, actual); 

	SMPFail_Line(tc, file, line, message, buf);
}

void SMPAssertPtrEquals_LineMsg(SMPTest* tc, const char* file, int line, const char* message, 
	void* expected, void* actual)
{
	char buf[STRING_MAX];
	if (expected == actual) return;
	sprintf(buf, "expected pointer <0x%p> but was <0x%p>", expected, actual);
	SMPFail_Line(tc, file, line, message, buf);
}


/*-------------------------------------------------------------------------*
 * SMPSuite
 *-------------------------------------------------------------------------*/

void SMPSuiteInit(SMPSuite* testSuite)
{
	testSuite->m_ncount = 0;
	testSuite->m_nfailCount = 0;
        memset(testSuite->m_alist, 0, sizeof(testSuite->m_alist));
}

SMPSuite* SMPSuiteNew(void)
{
	SMPSuite* testSuite = SMP_ALLOC(SMPSuite);
	SMPSuiteInit(testSuite);
	return testSuite;
}

void SMPSuiteDelete(SMPSuite *testSuite)
{
        unsigned int n;
        for (n=0; n < MAX_TEST_CASES; n++)
        {
                if (testSuite->m_alist[n])
                {
                        SMPTestDelete(testSuite->m_alist[n]);
                }
        }
        free(testSuite);

}

void SMPSuiteAdd(SMPSuite* testSuite, SMPTest *testCase)
{
	assert(testSuite->m_ncount < MAX_TEST_CASES);
	testSuite->m_alist[testSuite->m_ncount] = testCase;
	testSuite->m_ncount++;
}

void SMPSuiteAddSuite(SMPSuite* testSuite, SMPSuite* testSuite2)
{
	int i;
	for (i = 0 ; i < testSuite2->m_ncount ; ++i)
	{
		SMPTest* testCase = testSuite2->m_alist[i];
		SMPSuiteAdd(testSuite, testCase);
	}
}

void SMPSuiteRun(SMPSuite* testSuite)
{
	int i;
	for (i = 0 ; i < testSuite->m_ncount ; ++i)
	{
		SMPTest* testCase = testSuite->m_alist[i];
		SMPTestRun(testCase);
		if (testCase->m_bfailed) { testSuite->m_nfailCount += 1; }
	}
}

void SMPSuiteSummary(SMPSuite* testSuite, SMPString* summary)
{
	int i;
	for (i = 0 ; i < testSuite->m_ncount ; ++i)
	{
		SMPTest* testCase = testSuite->m_alist[i];
		SMPStringAppend(summary, testCase->m_bfailed ? "F" : ".");
	}
	SMPStringAppend(summary, "\n\n");
}

void SMPSuiteDetails(SMPSuite* testSuite, SMPString* details)
{
	int i;
	int failCount = 0;

	if (testSuite->m_nfailCount == 0)
	{
		int passCount = testSuite->m_ncount - testSuite->m_nfailCount;
		const char* testWord = passCount == 1 ? "test" : "tests";
		SMPStringAppendFormat(details, "OK (%d %s)\n", passCount, testWord);
	}
	else
	{
		if (testSuite->m_nfailCount == 1)
			SMPStringAppend(details, "There was 1 failure:\n");
		else
			SMPStringAppendFormat(details, "There were %d failures:\n", testSuite->m_nfailCount);

		for (i = 0 ; i < testSuite->m_ncount ; ++i)
		{
			SMPTest* testCase = testSuite->m_alist[i];
			if (testCase->m_bfailed)
			{
				failCount++;
				SMPStringAppendFormat(details, "%d) %s: %s\n",
					failCount, testCase->m_szname, testCase->m_szmessage);
			}
		}
		SMPStringAppend(details, "\n!!!FAILURES!!!\n");

		SMPStringAppendFormat(details, "Runs: %d ",   testSuite->m_ncount);
		SMPStringAppendFormat(details, "Passes: %d ", testSuite->m_ncount - testSuite->m_nfailCount);
		SMPStringAppendFormat(details, "Fails: %d\n",  testSuite->m_nfailCount);
	}
}
