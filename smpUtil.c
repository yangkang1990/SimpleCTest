#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smpUnit.h"

/*-------------------------------------------------------------------------*
 * Str function
 *-------------------------------------------------------------------------*/

char* SMPStrAlloc(int size)
{
	char* newStr = (char*) malloc( sizeof(char) * (size) );
	return newStr;
}

void  SMPStrFree(char* str)
{
	if(!str)
	   free(str);
}




char* SMPStrCopy(const char* old)
{
	int len = strlen(old);
	char* newStr = SMPStrAlloc(len + 1);
	strcpy(newStr, old);
	return newStr;
}

/*-------------------------------------------------------------------------*
 *  String
 *-------------------------------------------------------------------------*/

void SMPStringInit(SMPString* str)
{
	str->length = 0;
	str->size = STRING_MAX;
	str->buffer = (char*) malloc(sizeof(char) * str->size);
	str->buffer[0] = '\0';
}

SMPString* SMPStringNew(void)
{
	SMPString* str = (SMPString*) malloc(sizeof(SMPString));
	str->length = 0;
	str->size = STRING_MAX;
	str->buffer = (char*) malloc(sizeof(char) * str->size);
	str->buffer[0] = '\0';
	return str;
}

void SMPStringDelete(SMPString *str)
{
        if (!str) return;
        free(str->buffer);
        free(str);
}

void SMPStringResize(SMPString* str, int newSize)
{
	str->buffer = (char*) realloc(str->buffer, sizeof(char) * newSize);
	str->size = newSize;
}

void SMPStringAppend(SMPString* str, const char* text)
{
	int length;

	if (text == NULL) {
		text = "NULL";
	}

	length = strlen(text);
	if (str->length + length + 1 >= str->size)
		SMPStringResize(str, str->length + length + 1 + STRING_INC);
	str->length += length;
	strcat(str->buffer, text);
}

void SMPStringAppendChar(SMPString* str, char ch)
{
	char text[2];
	text[0] = ch;
	text[1] = '\0';
	SMPStringAppend(str, text);
}

void SMPStringAppendFormat(SMPString* str, const char* format, ...)
{
	va_list argp;
	char buf[HUGE_STRING_LEN];
	va_start(argp, format);
	vsprintf(buf, format, argp);
	va_end(argp);
	SMPStringAppend(str, buf);
}

void SMPStringInsert(SMPString* str, const char* text, int pos)
{
	int length = strlen(text);
	if (pos > str->length)
		pos = str->length;
	if (str->length + length + 1 >= str->size)
		SMPStringResize(str, str->length + length + 1 + STRING_INC);
	memmove(str->buffer + pos + length, str->buffer + pos, (str->length - pos) + 1);
	str->length += length;
	memcpy(str->buffer + pos, text, length);
}



