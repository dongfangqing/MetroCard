/*********************************iBoxV300*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "ST_string.h"
#include <stdio.h>

/**************************************
**�������ܣ���ȡ�ַ�������
***************************************/
signed int st_strlen(unsigned char* str)
{
	int i = 0;
	if(str != NULL)
	{
		while(*str++)
		{
			i++;
		}
	}
	
	return i;
}

/**************************************************
**�������ܣ��Ƚ������ַ�����С
**************************************************/
signed int st_strcmp(unsigned char *s,unsigned char *t)
{
	while(*s == *t)
	{
		if(*s == 0)
		{
			return(0);
		}
	    ++s;
	    ++t;
	}
	
	return (*s - *t);
}


/***************************************************
**�������ܣ��ַ�������
****************************************************/
unsigned char *st_strcpy(unsigned char *t,unsigned char *s)
{
	unsigned char *d;

	d = t;
	while(*s)
	{
	   *t++ = *s++;
	}
	*t = 0x0;			// Add string terminator

	return(d);
}


/***********************************************************************
**�������ܣ��ɱ䳤���ַ�����С�Ƚ�
************************************************************************/
signed int st_strncmp(unsigned char *dest,unsigned char *src,unsigned int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++) 
	{
		if(dest[i] != src[i])
		{
		    if(dest[i] > src[i])
			{
		        return 1;
		    }
			else
			{
		        return -1;
		    }
	    }
	}

	return 0;

}

/*****************************************************
**�������ܣ��ж��ַ���Сд
******************************************************/
signed char st_toupper(unsigned char c)
{
	if(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))
	{
		c &= 0xDF;
	}
	
	return c;
}


/*********************************************************
**�������ܣ��ַ�������
**********************************************************/
unsigned char *st_strcat(unsigned char *s, unsigned char *t)
{
	unsigned char *d;
	
	d = s;
	--s;
	while(*++s);

	do 
	{
		*s = *t++;
	}
	while(*s++);

	return(d);
}


/*********************************************************************
**�������ܣ��ɱ䳤���ڴ濽��
**********************************************************************/
signed int st_memcpy(unsigned char *dest, unsigned char *src, unsigned int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++)
	{
		dest[i] = src[i];
	}

	return(Len);
}


/***********************************************************************
**�������ܣ����ó���ΪLen���ڴ��Ϊbyte
************************************************************************/
signed int st_memset(unsigned char *dest,unsigned char byte, unsigned int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++)
	{
		dest[i] = byte;
	}

	return(Len);
}



/***********************************************************************
**�������ܣ��ڴ�������ݱȽ�
************************************************************************/
signed int st_memcmp(unsigned char* dest, unsigned char* src, unsigned int Len)
{
	unsigned int i;

	for(i = 0; i < Len; i++)
	{
		if(dest[i] != src[i])
		{
		    if(dest[i] > src[i])
			{
		        return 1;
		    }
			else
			{
		        return -1;
		    }
	    }
	}
	
	return 0;
}
/***********************************************************************
**�������ܣ��ַ���ת����
************************************************************************/

int str_toint(unsigned char buf[])
{
	int sum = 0;
	if(st_strlen(buf) == 3)
	{
		sum = sum + (buf[0]-'0')*100;
		sum = sum + (buf[1]-'0')*10;
		sum = sum + (buf[2]-'0');	
	}
	
	if(st_strlen(buf) == 2)
	{
		if(buf[0] == '0')
		{
			sum = sum + (buf[1]-'0');
		}
		else
		{
			sum = sum + (buf[0]-'0')*10;
			sum = sum + (buf[1]-'0')*1;
		}
	}
	
	if(st_strlen(buf) == 4)
	{
		sum = sum + (buf[0]-'0')*1000;
		sum = sum + (buf[1]-'0')*100;
		sum = sum + (buf[2]-'0')*10;
		sum = sum + (buf[3]-'0');	}
	

	return sum;
}


#if 0
void strto_int(uint8_t buf[],int a[6])
{
	int i = 0;
	int sum = 0;
	sum = (buf[0]-'0')*1000;
	sum = sum + (buf[1]-'0')*100;
	sum = sum + (buf[2]-'0')*10;
	sum = sum + (buf[3]-'0');	
	a[0] = sum;		//year
	
	sum = 0;
	sum = sum + (buf[5]-'0')*10;
	sum = sum + (buf[6]-'0');
	a[1] = sum;	//month
	
	sum = 0;
	sum = sum + (buf[8]-'0')*10;
	sum = sum + (buf[9]-'0');
	a[2] = sum;	//day
	
	sum = 0;
	sum = sum + (buf[11]-'0')*10;
	sum = sum + (buf[12]-'0');
	a[3] = sum;	//hour
	
	sum = 0;
	sum = sum + (buf[14]-'0')*10;
	sum = sum + (buf[15]-'0');
	a[4] = sum;	//min
	
	sum = 0;
	sum = sum + (buf[17]-'0')*10;
	sum = sum + (buf[18]-'0');
	a[5] = sum;	//sec
	sum = 0;
	
	
	
}
void Save_Alarm(unsigned char w_year,unsigned char w_month,unsigned char w_date,unsigned char hour,unsigned char min,unsigned char sec)
{
	uint8_t year;

	year = w_year-2000;
	if(year == 16)
	{
		Red_LED(0);
		Green_LED(0);
		
	}
	buf[0] = '2';
	buf[1] = '0';
	buf[2] = year/10 + '0';
	buf[3] = year%10 + '0';
	
	buf[5] = w_month/10 +'0';
	buf[6] = w_month%10 + '0';
	
	buf[8] = w_date/10 + '0';
	buf[9] = w_date%10 + '0';
	
	buf[11] = hour/10 + '0';
	buf[12] = hour%10 + '0';
	
	buf[14] = min/10 + '0';
	buf[15] = min%10 + '0';
	
	buf[17] = sec/10 + '0';
	buf[18] = sec%10 + '0';
	
	eeprom_WriteBytes(buf,0,20);
	//LCD_P8x16Str(20,2,buf);
}
#endif
