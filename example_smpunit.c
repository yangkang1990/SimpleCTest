#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "smpUnit.h"
#include "smpUtil.h"


#define SMPCompareAsserts(tc, message, expected, actual)  CompareAsserts((tc), __FILE__, __LINE__, (message), (expected), (actual))

static void CompareAsserts(SMPTest* tc, const char *file, int line, const char* message, const char* expected, const char* actual)
{
	int mismatch;
	if (expected == NULL || actual == NULL) {
		mismatch = (expected != NULL || actual != NULL);
	} else {
		const char *front = __FILE__ ":";
		const size_t frontLen = strlen(front);
		const size_t expectedLen = strlen(expected);

		const char *matchStr = actual;

		mismatch = (strncmp(matchStr, front, frontLen) != 0);
		if (!mismatch) {
			matchStr = strchr(matchStr + frontLen, ':');
			mismatch |= (matchStr == NULL || strncmp(matchStr, ": ", 2));
			if (!mismatch) {
				matchStr += 2;
				mismatch |= (strncmp(matchStr, expected, expectedLen) != 0);
			}
		}
	}

	SMPAssert_Line(tc, file, line, message, !mismatch);
}




/*-------------------------------------------------------------------------*
 * SMPString Test
 *-------------------------------------------------------------------------*/

void TestSMPStringNew(SMPTest* tc)
{
	SMPString* str = SMPStringNew();
	SMPAssertTrue(tc, 0 == str->length);
	SMPAssertTrue(tc, 0 != str->size);
	SMPAssertStrEquals(tc, "", str->buffer);
}


void TestSMPStringAppend(SMPTest* tc)
{
	SMPString* str = SMPStringNew();
	SMPStringAppend(str, "hello");
	SMPAssertIntEquals(tc, 5, str->length);
	SMPAssertStrEquals(tc, "hello", str->buffer);
	SMPStringAppend(str, " world");
	SMPAssertIntEquals(tc, 11, str->length);
	SMPAssertStrEquals(tc, "hello world", str->buffer);
}


void TestSMPStringAppendNULL(SMPTest* tc)
{
	SMPString* str = SMPStringNew();
	SMPStringAppend(str, NULL);
	SMPAssertIntEquals(tc, 4, str->length);
	SMPAssertStrEquals(tc, "NULL", str->buffer);
}


void TestSMPStringAppendChar(SMPTest* tc)
{
	SMPString* str = SMPStringNew();
	SMPStringAppendChar(str, 'a');
	SMPStringAppendChar(str, 'b');
	SMPStringAppendChar(str, 'c');
	SMPStringAppendChar(str, 'd');
	SMPAssertIntEquals(tc, 4, str->length);
	SMPAssertStrEquals(tc, "abcd", str->buffer);
}


void TestSMPStringInserts(SMPTest* tc)
{
	SMPString* str = SMPStringNew();
	SMPStringAppend(str, "world");
	SMPAssertIntEquals(tc, 5, str->length);
	SMPAssertStrEquals(tc, "world", str->buffer);
	SMPStringInsert(str, "hell", 0);
	SMPAssertIntEquals(tc, 9, str->length);
	SMPAssertStrEquals(tc, "hellworld", str->buffer);
	SMPStringInsert(str, "o ", 4);
	SMPAssertIntEquals(tc, 11, str->length);
	SMPAssertStrEquals(tc, "hello world", str->buffer);
	SMPStringInsert(str, "!", 11);
	SMPAssertIntEquals(tc, 12, str->length);
	SMPAssertStrEquals(tc, "hello world!", str->buffer);
}


void TestSMPStringResizes(SMPTest* tc)
{
	SMPString* str = SMPStringNew();
	int i;
	for(i = 0 ; i < STRING_MAX ; ++i)
	{
		SMPStringAppend(str, "aa");
	}
	SMPAssertTrue(tc, STRING_MAX * 2 == str->length);
	SMPAssertTrue(tc, STRING_MAX * 2 <= str->size);
}

SMPSuite* SMPStringGetSuite(void)
{
	SMPSuite* suite = SMPSuiteNew();

	SUITE_ADD_TEST(suite, TestSMPStringNew);
	SUITE_ADD_TEST(suite, TestSMPStringAppend);
	SUITE_ADD_TEST(suite, TestSMPStringAppendNULL);
	SUITE_ADD_TEST(suite, TestSMPStringAppendChar);
	SUITE_ADD_TEST(suite, TestSMPStringInserts);
	SUITE_ADD_TEST(suite, TestSMPStringResizes);

	return suite;
}

/*-------------------------------------------------------------------------*
 * SMPTest Test
 *-------------------------------------------------------------------------*/

void TestPasses(SMPTest* tc)
{
	SMPAssert(tc, "test should pass", 1 == 0 + 1);
}

void TestFails(SMPTest* tc)
{
	SMPAssert(tc, "test should fail", 1 == 1 + 1);
}


void TestSMPTestNew(SMPTest* tc)
{
	SMPTest* tc2 = SMPTestNew("MyTest", TestPasses);
	SMPAssertStrEquals(tc, "MyTest", tc2->m_szname);
	SMPAssertTrue(tc, !tc2->m_bfailed);
	SMPAssertTrue(tc, tc2->m_szmessage == NULL);
	SMPAssertTrue(tc, tc2->m_pfunction == TestPasses);
	SMPAssertTrue(tc, tc2->m_bran == 0);
	SMPAssertTrue(tc, tc2->m_pjumpBuf == NULL);
}


void TestSMPTestInit(SMPTest *tc)
{
	SMPTest tc2;
	SMPTestInit(&tc2, "MyTest", TestPasses);
	SMPAssertStrEquals(tc, "MyTest", tc2.m_szname);
	SMPAssertTrue(tc, !tc2.m_bfailed);
	SMPAssertTrue(tc, tc2.m_szmessage == NULL);
	SMPAssertTrue(tc, tc2.m_pfunction == TestPasses);
	SMPAssertTrue(tc, tc2.m_bran == 0);
	SMPAssertTrue(tc, tc2.m_pjumpBuf == NULL);
}

void TestSMPAssert(SMPTest* tc)
{
	SMPTest tc2;
	SMPTestInit(&tc2, "MyTest", TestPasses);

	SMPAssert(&tc2, "test 1", 5 == 4 + 1);
	SMPAssertTrue(tc, !tc2.m_bfailed);
	SMPAssertTrue(tc, tc2.m_szmessage == NULL);

	SMPAssert(&tc2, "test 2", 0);
	SMPAssertTrue(tc, tc2.m_bfailed);
	SMPCompareAsserts(tc, "SMPAssert didn't fail", "test 2", tc2.m_szmessage);

	SMPAssert(&tc2, "test 3", 1);
	SMPAssertTrue(tc, tc2.m_bfailed);
	SMPCompareAsserts(tc, "SMPAssert didn't fail", "test 2", tc2.m_szmessage);

	SMPAssert(&tc2, "test 4", 0);
	SMPAssertTrue(tc, tc2.m_bfailed);
	SMPCompareAsserts(tc, "SMPAssert didn't fail", "test 4", tc2.m_szmessage);

}

void TestSMPAssertPtrEquals_Success(SMPTest* tc)
{
	SMPTest tc2;
	int x;

	SMPTestInit(&tc2, "MyTest", TestPasses);

	/* test success case */
	SMPAssertPtrEquals(&tc2, &x, &x);
	SMPAssertTrue(tc, ! tc2.m_bfailed);
	SMPAssertTrue(tc, NULL == tc2.m_szmessage);
}

void TestSMPAssertPtrEquals_Failure(SMPTest* tc)
{
	SMPTest tc2;
	int x;
	int* nullPtr = NULL;
	char expected_message[STRING_MAX];

	SMPTestInit(&tc2, "MyTest", TestPasses);

	/* test failing case */
	sprintf(expected_message, "expected pointer <0x%p> but was <0x%p>", nullPtr, &x);
	SMPAssertPtrEquals(&tc2, NULL, &x);
	SMPAssertTrue(tc, tc2.m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertPtrEquals failed", expected_message, tc2.m_szmessage);
}

void TestSMPAssertPtrNotNull_Success(SMPTest* tc)
{
	SMPTest tc2;
	int x;

	SMPTestInit(&tc2, "MyTest", TestPasses);

	/* test success case */
	SMPAssertPtrNotNull(&tc2, &x);
	SMPAssertTrue(tc, ! tc2.m_bfailed);
	SMPAssertTrue(tc, NULL == tc2.m_szmessage);
}

void TestSMPAssertPtrNotNull_Failure(SMPTest* tc)
{
	SMPTest tc2;

	SMPTestInit(&tc2, "MyTest", TestPasses);

	/* test failing case */
	SMPAssertPtrNotNull(&tc2, NULL);
	SMPAssertTrue(tc, tc2.m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertPtrNotNull failed", "null pointer unexpected", tc2.m_szmessage);
}

void TestSMPTestRun(SMPTest* tc)
{
	SMPTest tc2;
	SMPTestInit(&tc2, "MyTest", TestFails);
	SMPTestRun(&tc2);

	SMPAssertStrEquals(tc, "MyTest", tc2.m_szname);
	SMPAssertTrue(tc, tc2.m_bfailed);
	SMPAssertTrue(tc, tc2.m_bran);
	SMPCompareAsserts(tc, "TestRun failed", "test should fail", tc2.m_szmessage);
}

/*-------------------------------------------------------------------------*
 * SMPSuite Test
 *-------------------------------------------------------------------------*/

void TestSMPSuiteInit(SMPTest* tc)
{
	SMPSuite ts;
	SMPSuiteInit(&ts);
	SMPAssertTrue(tc, ts.m_ncount == 0);
	SMPAssertTrue(tc, ts.m_nfailCount == 0);
}

void TestSMPSuiteNew(SMPTest* tc)
{
	SMPSuite* ts = SMPSuiteNew();
	SMPAssertTrue(tc, ts->m_ncount == 0);
	SMPAssertTrue(tc, ts->m_nfailCount == 0);
}

void TestSMPSuiteAddTest(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc2;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc2, "MyTest", TestFails);

	SMPSuiteAdd(&ts, &tc2);
	SMPAssertTrue(tc, ts.m_ncount == 1);

	SMPAssertStrEquals(tc, "MyTest", ts.m_alist[0]->m_szname);
}

void TestSMPSuiteAddSuite(SMPTest* tc)
{
	SMPSuite* ts1 = SMPSuiteNew();
	SMPSuite* ts2 = SMPSuiteNew();

	SMPSuiteAdd(ts1, SMPTestNew("TestFails1", TestFails));
	SMPSuiteAdd(ts1, SMPTestNew("TestFails2", TestFails));

	SMPSuiteAdd(ts2, SMPTestNew("TestFails3", TestFails));
	SMPSuiteAdd(ts2, SMPTestNew("TestFails4", TestFails));

	SMPSuiteAddSuite(ts1, ts2);
	SMPAssertIntEquals(tc, 4, ts1->m_ncount);

	SMPAssertStrEquals(tc, "TestFails1", ts1->m_alist[0]->m_szname);
	SMPAssertStrEquals(tc, "TestFails2", ts1->m_alist[1]->m_szname);
	SMPAssertStrEquals(tc, "TestFails3", ts1->m_alist[2]->m_szname);
	SMPAssertStrEquals(tc, "TestFails4", ts1->m_alist[3]->m_szname);
}

void TestSMPSuiteRun(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc1, tc2, tc3, tc4;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc1, "TestPasses", TestPasses);
	SMPTestInit(&tc2, "TestPasses", TestPasses);
	SMPTestInit(&tc3, "TestFails",  TestFails);
	SMPTestInit(&tc4, "TestFails",  TestFails);

	SMPSuiteAdd(&ts, &tc1);
	SMPSuiteAdd(&ts, &tc2);
	SMPSuiteAdd(&ts, &tc3);
	SMPSuiteAdd(&ts, &tc4);
	SMPAssertTrue(tc, ts.m_ncount == 4);

	SMPSuiteRun(&ts);
	SMPAssertTrue(tc, ts.m_ncount - ts.m_nfailCount == 2);
	SMPAssertTrue(tc, ts.m_nfailCount == 2);
}

void TestSMPSuiteSummary(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc1, tc2;
	SMPString summary;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc1, "TestPasses", TestPasses);
	SMPTestInit(&tc2, "TestFails",  TestFails);
	SMPStringInit(&summary);

	SMPSuiteAdd(&ts, &tc1);
	SMPSuiteAdd(&ts, &tc2);
	SMPSuiteRun(&ts);

	SMPSuiteSummary(&ts, &summary);

	SMPAssertTrue(tc, ts.m_ncount == 2);
	SMPAssertTrue(tc, ts.m_nfailCount == 1);
	SMPAssertStrEquals(tc, ".F\n\n", summary.buffer);
}


void TestSMPSuiteDetails_SingleFail(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc1, tc2;
	SMPString details;
	const char* front;
	const char* back;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc1, "TestPasses", TestPasses);
	SMPTestInit(&tc2, "TestFails",  TestFails);
	SMPStringInit(&details);

	SMPSuiteAdd(&ts, &tc1);
	SMPSuiteAdd(&ts, &tc2);
	SMPSuiteRun(&ts);

	SMPSuiteDetails(&ts, &details);

	SMPAssertTrue(tc, ts.m_ncount == 2);
	SMPAssertTrue(tc, ts.m_nfailCount == 1);

	front = "There was 1 failure:\n"
		"1) TestFails: ";
	back =  "test should fail\n"
		"\n!!!FAILURES!!!\n"
		"Runs: 2 Passes: 1 Fails: 1\n";

	SMPAssertStrEquals(tc, back, details.buffer + strlen(details.buffer) - strlen(back));
	details.buffer[strlen(front)] = 0;
	SMPAssertStrEquals(tc, front, details.buffer);
}


void TestSMPSuiteDetails_SinglePass(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc1;
	SMPString details;
	const char* expected;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc1, "TestPasses", TestPasses);
	SMPStringInit(&details);

	SMPSuiteAdd(&ts, &tc1);
	SMPSuiteRun(&ts);

	SMPSuiteDetails(&ts, &details);

	SMPAssertTrue(tc, ts.m_ncount == 1);
	SMPAssertTrue(tc, ts.m_nfailCount == 0);

	expected =
		"OK (1 test)\n";

	SMPAssertStrEquals(tc, expected, details.buffer);
}

void TestSMPSuiteDetails_MultiplePasses(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc1, tc2;
	SMPString details;
	const char* expected;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc1, "TestPasses", TestPasses);
	SMPTestInit(&tc2, "TestPasses", TestPasses);
	SMPStringInit(&details);

	SMPSuiteAdd(&ts, &tc1);
	SMPSuiteAdd(&ts, &tc2);
	SMPSuiteRun(&ts);

	SMPSuiteDetails(&ts, &details);

	SMPAssertTrue(tc, ts.m_ncount == 2);
	SMPAssertTrue(tc, ts.m_nfailCount == 0);

	expected =
		"OK (2 tests)\n";

	SMPAssertStrEquals(tc, expected, details.buffer);
}

void TestSMPSuiteDetails_MultipleFails(SMPTest* tc)
{
	SMPSuite ts;
	SMPTest tc1, tc2;
	SMPString details;
	const char* front;
	const char* mid;
	const char* back;

	SMPSuiteInit(&ts);
	SMPTestInit(&tc1, "TestFails1", TestFails);
	SMPTestInit(&tc2, "TestFails2", TestFails);
	SMPStringInit(&details);

	SMPSuiteAdd(&ts, &tc1);
	SMPSuiteAdd(&ts, &tc2);
	SMPSuiteRun(&ts);

	SMPSuiteDetails(&ts, &details);

	SMPAssertTrue(tc, ts.m_ncount == 2);
	SMPAssertTrue(tc, ts.m_nfailCount == 2);

	front =
		"There were 2 failures:\n"
		"1) TestFails1: ";
	mid =   "test should fail\n"
		"2) TestFails2: ";
	back =  "test should fail\n"
		"\n!!!FAILURES!!!\n"
		"Runs: 2 Passes: 0 Fails: 2\n";

	SMPAssertStrEquals(tc, back, details.buffer + strlen(details.buffer) - strlen(back));
	SMPAssert(tc, "Couldn't find middle", strstr(details.buffer, mid) != NULL);
	details.buffer[strlen(front)] = 0;
	SMPAssertStrEquals(tc, front, details.buffer);
}


/*-------------------------------------------------------------------------*
 * Misc Test
 *-------------------------------------------------------------------------*/

void TestSMPStrCopy(SMPTest* tc)
{
	const char* old = "hello world";
	const char* newStr = SMPStrCopy(old);
	SMPAssert(tc, "old is new", strcmp(old, newStr) == 0);
}


void TestSMPStringAppendFormat(SMPTest* tc)
{
	int i;
	char* text = SMPStrAlloc(301);		/* long string */
	SMPString* str = SMPStringNew();
	for (i = 0 ; i < 300 ; ++i)
		text[i] = 'a';
	text[300] = '\0';
	SMPStringAppendFormat(str, "%s", text);

	/* buffer limit raised to HUGE_STRING_LEN so no overflow */

	SMPAssert(tc, "length of str->buffer is 300", 300 == strlen(str->buffer));
}

void TestFail(SMPTest* tc)
{
	jmp_buf buf;
	int pointReached = 0;
	SMPTest* tc2 = SMPTestNew("TestFails", TestFails);
	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPFail(tc2, "hello world");
		pointReached = 1;
	}
	SMPAssert(tc, "point was not reached", pointReached == 0);
}

void TestAssertStrEquals(SMPTest* tc)
{
	jmp_buf buf;
	SMPTest *tc2 = SMPTestNew("TestAssertStrEquals", TestFails);

	const char* expected = "expected <hello> but was <world>";
	const char *expectedMsg = "some text: expected <hello> but was <world>";

	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals(tc2, "hello", "world");
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals failed", expected, tc2->m_szmessage);
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals_Msg(tc2, "some text", "hello", "world");
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals failed", expectedMsg, tc2->m_szmessage);
}

void TestAssertStrEquals_NULL(SMPTest* tc)
{
	jmp_buf buf;
	SMPTest *tc2 = SMPTestNew("TestAssertStrEquals_NULL", TestFails);

	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals(tc2, NULL, NULL);
	}
	SMPAssertTrue(tc, !tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals_NULL failed", NULL, tc2->m_szmessage);
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals_Msg(tc2, "some text", NULL, NULL);
	}
	SMPAssertTrue(tc, !tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals_NULL failed", NULL, tc2->m_szmessage);
}

void TestAssertStrEquals_FailNULLStr(SMPTest* tc)
{
	jmp_buf buf;
	SMPTest *tc2 = SMPTestNew("TestAssertStrEquals_FailNULLStr", TestFails);

	const char* expected = "expected <hello> but was <NULL>";
	const char *expectedMsg = "some text: expected <hello> but was <NULL>";

	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals(tc2, "hello", NULL);
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals_FailNULLStr failed", expected, tc2->m_szmessage);
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals_Msg(tc2, "some text", "hello", NULL);
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals_FailNULLStr failed", expectedMsg, tc2->m_szmessage);
}

void TestAssertStrEquals_FailStrNULL(SMPTest* tc)
{
	jmp_buf buf;
	SMPTest *tc2 = SMPTestNew("TestAssertStrEquals_FailStrNULL", TestFails);

	const char* expected = "expected <NULL> but was <hello>";
	const char *expectedMsg = "some text: expected <NULL> but was <hello>";

	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals(tc2, NULL, "hello");
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals_FailStrNULL failed", expected, tc2->m_szmessage);
	if (setjmp(buf) == 0)
	{
		SMPAssertStrEquals_Msg(tc2, "some text", NULL, "hello");
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals_FailStrNULL failed", expectedMsg, tc2->m_szmessage);
}

void TestAssertIntEquals(SMPTest* tc)
{
	jmp_buf buf;
	SMPTest *tc2 = SMPTestNew("TestAssertIntEquals", TestFails);
	const char* expected = "expected <42> but was <32>";
	const char* expectedMsg = "some text: expected <42> but was <32>";
	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertIntEquals(tc2, 42, 32);
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertIntEquals failed", expected, tc2->m_szmessage);
	if (setjmp(buf) == 0)
	{
		SMPAssertIntEquals_Msg(tc2, "some text", 42, 32);
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertStrEquals failed", expectedMsg, tc2->m_szmessage);
}

void TestAssertDblEquals(SMPTest* tc)
{
	jmp_buf buf;
	double x = 3.33;
	double y = 10.0 / 3.0;
	SMPTest *tc2 = SMPTestNew("TestAssertDblEquals", TestFails);
	char expected[STRING_MAX];
	char expectedMsg[STRING_MAX];
	sprintf(expected, "expected <%lf> but was <%lf>", x, y);
	sprintf(expectedMsg, "some text: expected <%lf> but was <%lf>", x, y);

	SMPTestInit(tc2, "TestAssertDblEquals", TestPasses);

	SMPAssertDblEquals(tc2, x, x, 0.0);
	SMPAssertTrue(tc, ! tc2->m_bfailed);
	SMPAssertTrue(tc, NULL == tc2->m_szmessage);

	SMPAssertDblEquals(tc2, x, y, 0.01);
	SMPAssertTrue(tc, ! tc2->m_bfailed);
	SMPAssertTrue(tc, NULL == tc2->m_szmessage);

	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertDblEquals(tc2, x, y, 0.001);
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertDblEquals failed", expected, tc2->m_szmessage);
	tc2->m_pjumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		SMPAssertDblEquals_Msg(tc2, "some text", x, y, 0.001);
	}
	SMPAssertTrue(tc, tc2->m_bfailed);
	SMPCompareAsserts(tc, "SMPAssertDblEquals failed", expectedMsg, tc2->m_szmessage);
}

/*-------------------------------------------------------------------------*
 * main
 *-------------------------------------------------------------------------*/

SMPSuite* SMPGetSuite(void)
{
	SMPSuite* suite = SMPSuiteNew();

	SUITE_ADD_TEST(suite, TestSMPStringAppendFormat);
	SUITE_ADD_TEST(suite, TestSMPStrCopy);
	SUITE_ADD_TEST(suite, TestFail);
	SUITE_ADD_TEST(suite, TestAssertStrEquals);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_NULL);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_FailStrNULL);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_FailNULLStr);
	SUITE_ADD_TEST(suite, TestAssertIntEquals);
	SUITE_ADD_TEST(suite, TestAssertDblEquals);

	SUITE_ADD_TEST(suite, TestSMPTestNew);
	SUITE_ADD_TEST(suite, TestSMPTestInit);
	SUITE_ADD_TEST(suite, TestSMPAssert);
	SUITE_ADD_TEST(suite, TestSMPAssertPtrEquals_Success);
	SUITE_ADD_TEST(suite, TestSMPAssertPtrEquals_Failure);
	SUITE_ADD_TEST(suite, TestSMPAssertPtrNotNull_Success);
	SUITE_ADD_TEST(suite, TestSMPAssertPtrNotNull_Failure);
	SUITE_ADD_TEST(suite, TestSMPTestRun);

	SUITE_ADD_TEST(suite, TestSMPSuiteInit);
	SUITE_ADD_TEST(suite, TestSMPSuiteNew);
	SUITE_ADD_TEST(suite, TestSMPSuiteAddTest);
	SUITE_ADD_TEST(suite, TestSMPSuiteAddSuite);
	SUITE_ADD_TEST(suite, TestSMPSuiteRun);
	SUITE_ADD_TEST(suite, TestSMPSuiteSummary);
	SUITE_ADD_TEST(suite, TestSMPSuiteDetails_SingleFail);
	SUITE_ADD_TEST(suite, TestSMPSuiteDetails_SinglePass);
	SUITE_ADD_TEST(suite, TestSMPSuiteDetails_MultiplePasses);
	SUITE_ADD_TEST(suite, TestSMPSuiteDetails_MultipleFails);

	return suite;
}
