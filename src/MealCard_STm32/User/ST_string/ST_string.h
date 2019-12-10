#ifndef _ST_STRING_H_
#define _ST_STRING_H_



signed int st_strlen(unsigned char* str);
signed int st_strcmp(unsigned char *s,unsigned char *t);
unsigned char *st_strcpy(unsigned char *t,unsigned char *s);
signed int st_strncmp(unsigned char *dest,unsigned char *src,unsigned int Len);
signed char st_toupper(unsigned char c);
unsigned char *st_strcat(unsigned char *s,unsigned char *t);
signed int st_memcpy(unsigned char *dest, unsigned char *src, unsigned int Len);
signed int st_memset(unsigned char *dest,unsigned char byte, unsigned int Len);
signed int st_memcmp(unsigned char* dest, unsigned char* src, unsigned int Len);
int str_toint(unsigned char buf[]);
void strto_int(unsigned char buf[],int a[6]);

void Save_Alarm(unsigned char w_year,unsigned char w_month,unsigned char w_date,unsigned char hour,unsigned char min,unsigned char sec);



#endif
