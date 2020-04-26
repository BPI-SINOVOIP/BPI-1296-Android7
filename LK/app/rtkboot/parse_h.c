#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOCAL_TRACE	0

#define  ST_START	0
#define  ST_GET_DEFINE	1
#define  ST_GET_STRING	2
#define  ST_DONE	0xfe
#define  ST_ERROR	0xff

static int tight(char* s)
{
	int i;
	int n = 0;
	for ( i=0; s[i] ; ++i) {
		if (strncmp(s+i, "//", 2)==0)
			break;
		if (strncmp(s+i, "/*", 2)==0)
			break;
		if (*(s+i) ==0xd || *(s+i) ==0xa)
			break;
		if (!isspace(*(s+i)))
			s[n++] = s[i];
	}
	//s[n] = '\0';
	//printf("sn=%s \n",s);//hcy test
	return n;
}

static int calc(int lv, char op, int rv)
{
	//printf("lv=%d , rv=%d \n", lv ,rv);//hcy test
	if (op == '+') {return lv + rv; }
	if (op == '-') {return lv - rv; }
	if (op == '*') {return lv * rv; }
	if (op == '/') {return lv / rv; }
	return 0;   // crash
}

static int balance(const char *s, int L, int R)
{
	int k;
	int c = 1;
	for (k=L+1; k<=R; ++k) {
		if (s[k] == '(') c++;
		if (s[k] == ')') c--;
		if (c == 0) return k;
	}
	return -1;
}

static char isopera(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

static int findopera(const char *s, int L, int R)
{
	int k;
	for (k=L; k<=R; ++k)
		if (isopera(s[k]))
			return k;
	return -1;
}

static int getnum(const char *s, int L, int R)
{
	int k;
	int n = 0;
	//printf("L=%d , R=%d \n", L, R);//hcy test
	for (k=L; k<=R; ++k)
		n = n * 10 + s[k] - '0';

	//printf("n=%d \n", n);//hcy test
	return n;
}

static int f(const char *s, int L, int R)
{
	int op;
	if (s[L] == '(') {
		op = balance(s, L, R) + 1;
		//  printf("0 op=0x%x \n",op);//hcy test
		if (op > R) return f(s, L+1, op-2);
			return calc(f(s, L+1, op-2), s[op], f(s, op+1, R));
	}

	op = findopera(s, L, R);
	//printf("1 op=0x%x \n",op);//hcy test
	if (op != -1)
		return calc(f(s, L, op-1), s[op], f(s, op+1, R));
	return getnum(s, L, R);
}

static int evaluate(char *s)
{
	int length = tight(s);
	//printf("length=%d \n",length);//hcy test
	return f(s, 0, length-1);
}

unsigned int parse_h(
	const char *target_str,
	char *src_addr,
	uint length,
	uint* pRetVal,
	const char **pRetaddr)
{
	unsigned state = ST_START;
	const char *fin_addr = NULL;
	char *s = NULL;
	unsigned final_num = 0;

	fin_addr = src_addr + length ;
	s = src_addr ;


	while( (state != ST_DONE) && (state != ST_ERROR) && (s < fin_addr) ) {
		while(isspace(*s) && (s < fin_addr) )
			s++;

		if (s>=fin_addr)
			state = ST_DONE ;

		if (strncmp(s, "/*", 2) == 0) {
			while (( strncmp(s, "*/", 2) != 0) && (s < fin_addr) )
				s++;
			if (s>=fin_addr)
				state = ST_DONE ;
		}
		if (strncmp(s, "//", 2) == 0) {
			while((*s!= 0xd) && (s < fin_addr) )
				s++;
			if (s>=fin_addr)
				state = ST_DONE ;
			else
				state = ST_START;
		}

		if (state == ST_GET_STRING)
			final_num = evaluate(s);

		switch (state) {
			case ST_START :
				if ((strncmp(s, "#define ", 8) == 0) || (strncmp(s, "#define    ", 8) == 0)) {
					state = ST_GET_DEFINE;
					s+=8;
				} else
					s++;
				break;

		case ST_GET_DEFINE :
			if ((strncmp(s, target_str, strlen(target_str)) == 0) && isspace(*(s+strlen(target_str)))) {
				state = ST_GET_STRING ;
				s+=strlen(target_str);
			} else
				s++ ;
			break;
		case ST_GET_STRING :
			*pRetaddr = s;
			*pRetVal = final_num;
			return 0; //ok
			break;

		default :
			break ;

		}

		if (s>=fin_addr)
			state = ST_DONE ;
	}
	if (state == ST_DONE) {
		printf("%s not found \n", target_str) ;
		return 1; //fail
	}

	return 1; //fail
}
