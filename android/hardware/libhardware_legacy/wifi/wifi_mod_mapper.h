#ifndef _WIFI_MOD_MAPPER_H
#define _WIFI_MOD_MAPPER_H
int init_wifi_mod_mapper();
int get_insmod_names(char** result);
void uninit_wifi_mod_mapper();
#endif //_WIFI_MOD_MAPPER_H