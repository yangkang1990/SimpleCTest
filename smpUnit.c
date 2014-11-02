#include <stdlib.h>  
#include "smpUnit.h"  
 
  
#define UNUSED(param) ((param) = (param))  
  
int SMPtest_main (int argc, char *argv[])  
{   
    char *str1 = "Hello";  
    char *str2 = "World";  
    char *str3 = "WORLD";  
    void *p1   = NULL;  
    void *p2   = NULL;  
  
    UNUSED(argc);  
    UNUSED(argv);  
      
    SMPTEST_EQUALS(87, 87);  
  
    SMPTEST_NOTEQUALS(10, 13);  
  
    SMPTEST_LESS_THAN(41, 70);  
  
    SMPTEST_LESS_THAN_EQUALS(8, 8);  
    SMPTEST_LESS_THAN_EQUALS(7, 8);  
  
    SMPTEST_GREATER_THAN(70, 41);  
  
    SMPTEST_GREATER_THAN_EQUALS(8, 8);  
    SMPTEST_GREATER_THAN_EQUALS(8, 7);  

    SMPTEST_STR_EQUALS(str2, str3);  
  
    SMPTEST_STR_NOTEQUALS(str1, str2);  
      
    p1 = malloc(10);  
    memset(p1, 2, 10);  
  
    p2 = malloc(10);  
    memset(p2, 2, 10);  
  
    SMPTEST_MEM_EQUALS(p1, p2, 10);  
  
    memset(p2, 5, 10);  
    SMPTEST_MEM_NOTEQUALS(p1, p2, 10);  

    free(p1);  
    p1 = NULL;  
  
    free(p2);  
    p2 = NULL;  
  
    return 0;  
}  


















