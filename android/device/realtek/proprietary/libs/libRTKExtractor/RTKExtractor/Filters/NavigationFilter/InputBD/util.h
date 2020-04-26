#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef struct
{
    char *buf;
    int alloc;
    int len;
} bdt_str_t;

void bdt_hex_dump(uint8_t *buf, int count);
uint16_t bdt_get_uint16(uint8_t *buf);
uint32_t bdt_get_uint32(uint8_t *buf);

void bdt_str_append_sub(bdt_str_t *str, char *append, int start, int app_len);
bdt_str_t* bdt_str_substr(char *str, int start, int len);
void bdt_str_append(bdt_str_t *str, char *append);
void bdt_str_printf(bdt_str_t *str, const char *fmt, ...);
void bdt_str_free(bdt_str_t *str);
void bdt_indent_printf(int level, char *fmt, ...);

void bdt_verbose_check_value(char *msg, uint8_t *buf, uint8_t val, int count);
