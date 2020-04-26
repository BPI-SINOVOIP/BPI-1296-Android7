#ifndef _BIG_INT_H_
#define _BIG_INT_H_

#ifdef __cplusplus
extern "C" {
#endif


#define BI_MAXLEN 129
#define DEC 10
#define HEX 16


typedef struct big_int {
    int m_nSign;
    int m_nLength;
    unsigned int m_ulValue[BI_MAXLEN];
} BI;


BI *init_BI(void);
void free_BI(BI* p);
BI *move(BI *A);
BI *move_p(unsigned long long b);
int Cmp(BI *A, BI *B);
int isZero(BI *A);

void Add(BI *A, BI *B);
void Sub(BI *A, BI *B);
BI *Mul(BI *A, BI *B);
BI *Div(BI *A, BI *B);
BI *Mod(BI *A, BI *B);

void Add_p(BI *A, int b);
void Sub_p(BI *A, int b);
BI *Mul_p(BI *A, int b);
BI *Div_p(BI *A, int b);
unsigned int Mod_p(BI *A, int b);

BI *Exp_Mod(BI *base, BI *exp, BI *mod);

BI *InPutFromStr(char *str, const unsigned int system);
char *OutPutToStr(BI *A, const unsigned int system);

BI *InPutFromAddr(unsigned char *addr, int len);
void OutPutToAddr(BI *A, unsigned char *addr, char reverse);

void dump_bi(BI *A);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _BIG_INT_H_ */

