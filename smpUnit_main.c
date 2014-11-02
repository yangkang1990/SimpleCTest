#include <stdio.h>

#include "smpUnit.h"
#include "smpUtil.h"

SMPSuite* SMPGetSuite();
SMPSuite* SMPStringGetSuite();

void RunAllTests(void)
{
	SMPString *output = SMPStringNew();
	SMPSuite* suite = SMPSuiteNew();

	SMPSuiteAddSuite(suite, SMPGetSuite());
	SMPSuiteAddSuite(suite, SMPStringGetSuite());

	SMPSuiteRun(suite);
	SMPSuiteSummary(suite, output);
	SMPSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
}
