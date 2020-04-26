#ifndef RTK_PARAMETER_H_
#define RTK_PARAMETER_H_

extern int get_parameter_value(const char* keystring, unsigned int *keyval);
extern int get_parameter_string(const char* keystring, char *string, int str_maxlen);

#endif
