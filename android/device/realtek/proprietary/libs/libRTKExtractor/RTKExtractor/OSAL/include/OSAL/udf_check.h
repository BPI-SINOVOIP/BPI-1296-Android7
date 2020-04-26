#ifndef _UDF_CHECK
#define _UDF_CHECK

#ifdef __cplusplus
extern "C" {
#endif

int start_record_udf(int fd, char *map_addr);
int end_record_udf();
int check_udf();

#ifdef __cplusplus
}
#endif

#endif

