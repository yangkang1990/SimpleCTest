#ifndef __SMPSMPT_H  
#define __SMPSMPT_H  
  
#include <stdio.h>  
#include <string.h>  

  
static int gTotalCase     = 0;  
static int gSuccessedCase = 0;  
static int gFailedCase    = 0;  
  
#ifndef NULL  
#define NULL ((void*)0)  
#endif  
  
#define INTERNAL_SUCCEEDED(_a, _b, _oper) \
    printf("    Expected: \""#_a"\" "#_oper" \""#_b"\"\n      Result: Succeeded\n")  
#define INTERNAL_FAILED(_a, _b, _oper) \
    printf("    Expected: \""#_a"\" "#_oper" \""#_b"\"\n (X)  Result: Failed\n")      
#define INTERNAL_SHOWINFO() \
    printf("Case %d =====>\n", gTotalCase); \
    printf("    Location: %s:%d\n", __FILE__, __LINE__); \
  
#define SMPTEST_ERROR(_string) do { \
    printf("Error:  %s\n",_string); \
    return -1;\
} while (0)  
  
#define SMPTEST_EQUALS(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ((_a) == (_b)) { \
        INTERNAL_SUCCEEDED(_a, _b, ==); \
        gSuccessedCase++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, ==); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_NOTEQUALS(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ((_a) == (_b)) { \
        INTERNAL_FAILED(_a, _b, !=); \
        gFailedCase++; \
    } \
    else { \
        INTERNAL_SUCCEEDED(_a, _b, !=); \
        gSuccessedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_LESS_THAN(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ((_a) < (_b)) { \
        INTERNAL_SUCCEEDED(_a, _b, <); \
        gSuccessedCase ++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, <); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_LESS_THAN_EQUALS(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ((_a) <= (_b)) { \
        INTERNAL_SUCCEEDED(_a, _b, <=); \
        gSuccessedCase++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, <=); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_GREATER_THAN(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ((_a) > (_b)) { \
        INTERNAL_SUCCEEDED(_a, _b, >); \
        gSuccessedCase++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, >); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_GREATER_THAN_EQUALS(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ((_a) >= (_b)) { \
        INTERNAL_SUCCEEDED(_a, _b, >=); \
        gSuccessedCase++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, >=); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_STR_EQUALS(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ( NULL == (_a) || NULL == (_b) ) { \
        SMPTEST_ERROR("ptr should not be NULL"); \
    } \
    if ( !strcmp((_a), (_b)) ) { \
        INTERNAL_SUCCEEDED(_a, _b, ==); \
        gSuccessedCase++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, ==); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_STR_NOTEQUALS(_a, _b) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ( NULL == (_a) || NULL == (_b) ) { \
        SMPTEST_ERROR("ptr should not be NULL"); \
    } \
    if ( !strcmp((_a), (_b)) ) { \
        INTERNAL_FAILED(_a, _b, !=); \
        gFailedCase++; \
    } \
    else { \
        INTERNAL_SUCCEEDED(_a, _b, !=); \
        gSuccessedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_MEM_EQUALS(_a, _b, _len) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ( NULL == (_a) || NULL == (_b) ) { \
        SMPTEST_ERROR("ptr should not be NULL"); \
    } \
    if ( (_len) <= 0 ) { \
        SMPTEST_ERROR("mem len should be positive"); \
    } \
    if ( !memcmp((void*)(_a),(void*)(_b), (int)(_len)) ) { \
        INTERNAL_SUCCEEDED(_a, _b, ==); \
        gSuccessedCase++; \
    } \
    else { \
        INTERNAL_FAILED(_a, _b, ==); \
        gFailedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
#define SMPTEST_MEM_NOTEQUALS(_a, _b, _len) do { \
    gTotalCase++; \
    INTERNAL_SHOWINFO(); \
    if ( NULL == (_a) || NULL == (_b) ) { \
        SMPTEST_ERROR("ptr should not be NULL"); \
    } \
    if ( (_len) <= 0 ) { \
        SMPTEST_ERROR("mem len should be positive"); \
    } \
    if ( !memcmp((void*)(_a), (void*)(_b), (int)(_len)) ) { \
        INTERNAL_FAILED(_a, _b, !=); \
        gFailedCase++; \
    } \
    else { \
        INTERNAL_SUCCEEDED(_a, _b, !=); \
        gSuccessedCase++; \
    } \
    printf("\n"); \
} while (0)  
  
static void SMPtest_bar()  
{  
    printf("********************************************************\n");   
    printf("*                      Simple Test                       *\n");  
    printf("********************************************************\n");  
    printf("\n");  
}  
  
static int SMPtest_report()  
{  
    printf("\n");  
    printf("********************************************************\n");   
    printf("*                     Test Report                      *\n");  
    printf("********************************************************\n");  
      
    if (0 == gTotalCase) {  
        printf("No test case is run\n");  
        goto pass;  
    }  
    printf("    Total: %d\n", gTotalCase);  
    printf("Succeeded: %d (%d%%)\n", gSuccessedCase, gSuccessedCase * 100 / gTotalCase);  
    printf("********************************************************\n");   
    printf("\n");  
  
    if (gTotalCase != gSuccessedCase) {  
        printf("\n");  
        printf(":-( Failed\n");  
        printf("\n");  
        return -1;  
    }  
  
pass:      
    printf("\n");  
    printf(":-) Passed\n");  
    printf("\n");  
  
    return 0;  
}  
  

extern int SMPtest_main(int _argc, char *_argv[]);  
  
int main(int _argc, char *_argv[])  
{  
    SMPtest_bar();  
    SMPtest_main(_argc, _argv);  
    return SMPtest_report();  
}  

  
#endif  /* end of __SMPTEST_H */  