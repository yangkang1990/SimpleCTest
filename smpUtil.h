#ifndef _SMPUTIL_H_
#define	_SMPUTIL_H_


typedef unsigned char	uchar;
typedef unsigned short ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;




/* String */

char* SMPStrAlloc(int size);
char* SMPStrCopy(const char* old);

#define SMP_ALLOC(TYPE)		((TYPE*) malloc(sizeof(TYPE)))

#define HUGE_STRING_LEN	8192
#define STRING_MAX			256
#define STRING_INC			256

typedef struct
{
	int length;
	int size;
	char* buffer;
} SMPString;

void SMPStringInit(SMPString* str);
SMPString* SMPStringNew(void);
void SMPStringRead(SMPString* str, const char* path);
void SMPStringAppend(SMPString* str, const char* text);
void SMPStringAppendChar(SMPString* str, char ch);
void SMPStringAppendFormat(SMPString* str, const char* format, ...);
void SMPStringInsert(SMPString* str, const char* text, int pos);
void SMPStringResize(SMPString* str, int newSize);
void SMPStringDelete(SMPString* str);


#endif