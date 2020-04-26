#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
// #define PC_DEBUG
#define LOG_TAG "WifiModMapper"
#ifdef PC_DEBUG
#include "log.h"
#else
#include "cutils/log.h"
#endif
#include "wifi_mod_mapper.h"
#include "jsmn.h"
#define MAX_BUFFER_SIZE (1024)
#define MAX_SMALL_BUFFER_SIZE (128)

enum {
    WALK_OK = 0, WALK_BADPATTERN, WALK_NAMETOOLONG, WALK_BADIO,
};

#define WS_NONE     0
#define WS_RECURSIVE    (1 << 0)
#define WS_DEFAULT  WS_RECURSIVE
#define WS_FOLLOWLINK   (1 << 1)    /* follow symlinks */
#define WS_DOTFILES (1 << 2)    /* per unix convention, .file is hidden */
#define WS_MATCHDIRS    (1 << 3)    /* if pattern is used on dir names too */

#define MAX_WIFI 20
#define MAX_LEN  20
#ifdef PC_DEBUG
static char* cfg_root = "/tmp/";
#else
static char* cfg_root = "/system/etc/wifi/";
#endif
static char** g_mods = NULL;
static char** g_mod_alias = NULL;
static int all_dev_count = 0;
static int g_mod_alias_count = 0;
static int g_mods_count = 0;
static int g_mod_alias_last_idx = -1;
static int g_manual_mod_count = 0;
static char* g_wifi_module_mapper_cfg_str = NULL;
static jsmntok_t g_js_tokens[MAX_BUFFER_SIZE];
static int g_token_size = 0;

static int read_line(char* buffer, int size, char* file_path) {
    FILE* file = fopen(file_path, "r"); /* should check the result */
    if (file == NULL)
        return -1;
    char* ret = fgets(buffer, size, file);
    if (ret != NULL) {
        size_t ln = strlen(buffer) - 1;
        if (buffer[ln] == '\n')
            buffer[ln] = '\0';
    }
    fclose(file);
    return ret == NULL;
}

static int prefix(const char *str, const char *pre)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

static void add_alias(char* fn) {
    if (g_mod_alias == NULL) {
        ALOGE("mod_alias==NULL");
        return;
    }
    if (g_mod_alias_last_idx >= all_dev_count) {
        ALOGE("mod_alias_idx:%d>=all_dev_count:%d", g_mod_alias_last_idx,
                all_dev_count);
        return;
    }

    read_line(g_mod_alias[++g_mod_alias_last_idx], MAX_SMALL_BUFFER_SIZE, fn);
//    ALOGI("add_alias: %s", g_mod_alias[g_mod_alias_last_idx]);
}

static int count_dir(const char *dir) {
    struct dirent *dp;
    DIR *fd;
    int count = 0;
    if ((fd = opendir(dir)) == NULL) {
        ALOGE("listdir: can't open %s\n", dir);
        return 0;
    }
    while ((dp = readdir(fd)) != NULL) {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
            continue; /* skip self and parent */
        count++; //printf("%s/%s\n", dir, dp->d_name);
    }
    closedir(fd);
    return count;
}

static int walk_recur(const char *dname, regex_t *reg, int spec, int deepLevel) {
    struct dirent *dent;
    DIR *dir;
    struct stat st;
    char fn[FILENAME_MAX];
    int res = WALK_OK;
    int len = strlen(dname);
    if (len >= FILENAME_MAX - 1)
        return WALK_NAMETOOLONG;

    strcpy(fn, dname);
    fn[len++] = '/';

    if (!(dir = opendir(dname))) {
        ALOGW("can't open %s", dname);
        return WALK_BADIO;
    }
    deepLevel++;
    errno = 0;
    while ((dent = readdir(dir))) {
        if (!(spec & WS_DOTFILES) && dent->d_name[0] == '.')
            continue;
        if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
            continue;

        strncpy(fn + len, dent->d_name, FILENAME_MAX - len);
        if (lstat(fn, &st) == -1) {
            ALOGW("Can't stat %s", fn);
            res = WALK_BADIO;
            continue;
        }

        // ALOGD("dent->d_name=%s", dent->d_name);
        /* don't follow symlink unless told so */
        if (S_ISLNK(st.st_mode) && !(spec & WS_FOLLOWLINK))
            continue;

        /* will be false for symlinked dirs */
        if (S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode)) {
            // ALOGD("current walk dir:%s, deep:%d", fn, deepLevel);
            /* recursively follow dirs */
            if ((spec & WS_RECURSIVE)) {
                if (deepLevel <= 1)
                    walk_recur(fn, reg, spec, deepLevel);
            }

            if (!(spec & WS_MATCHDIRS))
                continue;
        }

        /* pattern match */
        if (!regexec(reg, fn, 0, 0, 0)) {
//            ALOGD("walk_dir_get: %s", fn);
            add_alias(fn);
        }
    }

    if (dir)
        closedir(dir);

    return res ? res : errno ? WALK_BADIO : WALK_OK;
}

static int walk_dir(const char *dname, char *pattern, int spec, int deepLevel) {
    regex_t r;
    int res;
    if (regcomp(&r, pattern, REG_EXTENDED | REG_NOSUB))
        return WALK_BADPATTERN;
    res = walk_recur(dname, &r, spec, deepLevel);
    regfree(&r);

    return res;
}

static int alias_cmp(char* key, int cmp_len, char* pattern, char* data){
    const int key_len = strlen(key);
    if(*(pattern+key_len)!='*'){
        if(memcmp(pattern, data, cmp_len)==0)
            return cmp_len;
    }else{
        return key_len+1;
    }
    return -1;
}

/*
 * "sdio:c*v024Cd8179*"
 * "sdio:c07v024CdB723"
 */
static int do_sdio_entry(char* hw_alias, char* mod_alias)
{
    const int DEBUG = 0;
    char key[MAX_SMALL_BUFFER_SIZE] ={0};
    strcpy(key, "c");
    int cmp_len = 3;
    char* pch_mod = strstr(mod_alias, key);
    char* pch_hw = strstr(hw_alias, key);
    int mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "v");
    cmp_len = 10;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    return 0;
}

/*
 * "usb:v0BDAp8813d*dc*dsc*dp*ic*isc*ip*in*"
 * "usb:v0BDAp818Bd0110dc00dsc00dp00icFFiscFFipFFin00"
 */
static int do_usb_entry(char* hw_alias, char* mod_alias)
{
    const int DEBUG = 0;
    char key[MAX_SMALL_BUFFER_SIZE] ={0};
    strcpy(key, "v");
    int cmp_len = 10;
    char* pch_mod = strstr(mod_alias, key);
    char* pch_hw = strstr(hw_alias, key);
    int mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "d");
    cmp_len = 5;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "dc");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "dsc");
    cmp_len = 5;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "dp");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "ic");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "isc");
    cmp_len = 5;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "ip");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "in");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    return 0;
}

/*
 * "mod pci:v00008086d00000C00sv*sd*bc*sc*i*"
 * "hw  pci:v00008086d00000C00sv00001043sd00008534bc06sc00i00"
 */
static int do_pci_entry(char* hw_alias, char* mod_alias)
{
    const int DEBUG = 0;
    char key[MAX_SMALL_BUFFER_SIZE] ={0};
    strcpy(key, "v");
    int cmp_len = 18;
    char* pch_mod = strstr(mod_alias, key);
    char* pch_hw = strstr(hw_alias, key);
    int mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "sv");
    cmp_len = 10;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "sd");
    cmp_len = 10;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "bc");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "sc");
    cmp_len = 4;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    if(DEBUG) ALOGD("pch_mod=%s, pch_hw=%s", pch_mod, pch_hw);

    pch_mod+=mod_offset;
    pch_hw+=cmp_len;
    strcpy(key, "i");
    cmp_len = 3;
    mod_offset = alias_cmp(key, cmp_len, pch_mod, pch_hw);
    if(mod_offset<0) return -1;

    return 0;
}


static int get_module_name_by_mod_alias(char* mod_alias){
//    ALOGD("mod_alias=%s", mod_alias);
    int i=0;
    for (i = 0; i < g_mod_alias_count; i++) {
//      ALOGD("g_mod_alias[i]=%s", g_mod_alias[i]);
        //if g_mod_alias[i] found vid_pid
        if(prefix(g_mod_alias[i], "sdio:") && prefix(mod_alias, "sdio:")){
            int ret = do_sdio_entry(g_mod_alias[i], mod_alias);
            if(!ret) return 0;
        } else if(prefix(g_mod_alias[i], "usb:") && prefix(mod_alias, "usb:")){
            int ret = do_usb_entry(g_mod_alias[i], mod_alias);
            if(!ret) return 0;
        } else if(prefix(g_mod_alias[i], "pci:") && prefix(mod_alias, "pci:")){
            int ret = do_pci_entry(g_mod_alias[i], mod_alias);
            if(!ret) return 0;
        }
    }
    return -1;
}

static void get_str_from_token(char* buf, int idx){
    unsigned int length = g_js_tokens[idx].end - g_js_tokens[idx].start;
    memcpy(buf, &g_wifi_module_mapper_cfg_str[g_js_tokens[idx].start], length);
    buf[length] = '\0';
}

static int map_module_names(char** result){
    int i=0, j=0, n=0;
    char buf[MAX_SMALL_BUFFER_SIZE]={0};
    char module_name[MAX_SMALL_BUFFER_SIZE]={0};
    for(i=0;i<g_token_size;i++){
        if(g_js_tokens[i].type==JSMN_STRING && g_js_tokens[i+1].type==JSMN_ARRAY)
        {
            get_str_from_token(module_name, i);
//          ALOGD("token idx=%d", i);
//          ALOGD("module name=%s", module_name);
            for(j=i+2;g_js_tokens[j].start >= g_js_tokens[i+1].start && g_js_tokens[j].end <= g_js_tokens[i+1].end;j++)
            {
                get_str_from_token(buf, j);
//              ALOGD("token idx=%d", j);
//              ALOGD("modalias=%s", buf);
                if(get_module_name_by_mod_alias(buf)) {
//                  ALOGD("get_module_name_by_mod_alias not found");
                } else {
                    ALOGD("get_module_name_by_mod_alias get:%s", module_name);
                    strncpy(result[n++], module_name, MAX_LEN);
                }
            }
            i=j-1;
        }
    }
    return n;
}

int get_insmod_names(char** result) {
    ALOGD("begin get_insmod_names");
    int i = 0;
    const int size = MAX_LEN;

    //if manual_mod_list exist
    if (g_manual_mod_count>0)
    {
        int n=0;
        for(i=0;i<g_manual_mod_count;i++){
            strncpy(result[n++], g_mods[i], size);
        }
        return g_manual_mod_count;
    }

    if (g_mod_alias_count < 1)
        return 0;
    if (g_mod_alias_last_idx < 0)
        return 0;

    ALOGD("g_mod_alias_count=%d", g_mod_alias_count);
    int n=map_module_names(result);
    ALOGD("end get_insmod_names");
    return n;
}

static int split2(char** arr, int arr_size, char* str, const char *del){
    int count=0;
    char *pch;
    char *p = str;
    //count tokens
    pch = strpbrk(p, del);
    while(pch != NULL) {
        count++;
        p = pch + 1;
        pch = strpbrk(p, del);
    }
    if(strlen(p)>0)
        count++;
    if(count>arr_size) return count;

    //copy string
    int i=0;
    p = str;
    pch = strpbrk(p, del);
    while(pch != NULL) {
        strncpy(arr[i++], p, pch-p);
        p = pch + 1;
        pch = strpbrk(p, del);
    }
    if(strlen(p)>0)
        strcpy(arr[i++], p);
    return count;
}

static char** alloc_str_arr(int d1, int d2){
    if(d1<=0 || d2<=0){
        ALOGE("alloc_str_arr d1:%d, d2:%d", d1, d2);
        return NULL;
    }
    char** ret = NULL;
    int i=0;
    ret = (char**)malloc(sizeof(char*)*d1);
    for(i=0;i<d1;i++){
        ret[i] = (char*)malloc(sizeof(char)*d2);
        memset(ret[i], 0, sizeof(char)*d2);
    }
    return ret;
}

static int js_parse(char* js, jsmntok_t* tokens, size_t tokens_size){
    jsmn_parser parser;
    jsmn_init(&parser);
//  ALOGD("js=%s", js);
    jsmnerr_t jerr = jsmn_parse(&parser,
            js,
            strlen(js),
            tokens,
            tokens_size);
    if(jerr<=0){
        ALOGE("jsmn_parse error type=%d", jerr);
    } else {
//      int i=0;
//      for(i=0;i<jerr;i++){
//          ALOGD("tokens[%d] .type=%d, .start=%d, .end=%d", i, tokens[i].type, tokens[i].start, tokens[i].end);
//      }
    }
    return jerr;
}

static char* load_file_to_str(char* input_file_name){
    char *file_contents;
    long input_file_size;
    FILE *input_file = fopen(input_file_name, "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = malloc((input_file_size + 1) * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);
    file_contents[input_file_size] = 0;
    return file_contents;
}

static int create_map_table() {
    ALOGD("begin createMapTable");
    char path[MAX_BUFFER_SIZE] = { 0 };
    snprintf(path, MAX_BUFFER_SIZE, "%s%s", cfg_root, "wifi_module_mapper.json");
    g_wifi_module_mapper_cfg_str = load_file_to_str(path);
    g_token_size = js_parse(g_wifi_module_mapper_cfg_str, g_js_tokens, MAX_BUFFER_SIZE);
    ALOGD("end createMapTable");
    return 0;
}

static void free_string_array(char*** a, int c) {
    if (a == NULL) return;
    if (*a == NULL) return;

    int i = 0;
    for (i = 0; i<c ; i++) {
        free((*a)[i]);
    }
    free(*a);
    *a = NULL;
}

static int load_manual_list() {
    char path[MAX_BUFFER_SIZE] = { 0 };
    snprintf(path, MAX_BUFFER_SIZE, "%s%s", cfg_root, "wifi_module_list.cfg");
    FILE* file = fopen(path, "r"); /* should check the result */
    if (file == NULL) {
        ALOGD("wifi_module_list.cfg open failed");
        return -1;
    }
    char buffer[MAX_BUFFER_SIZE] = { 0 };
    char* ret = fgets(buffer, MAX_BUFFER_SIZE, file);
    if (ret != NULL) {
        size_t ln = strlen(buffer) - 1;
        if (buffer[ln] == '\n')
            buffer[ln] = '\0';
    }
    ALOGD("load_manual_list:%s", buffer);
    static const char* spliter = " ";
    char** arr = NULL;
    int n = split2(arr, 0, buffer, spliter);
    arr = alloc_str_arr(n, MAX_LEN);
    split2(arr, n, buffer, spliter);
    if(n>0)
    {
        g_mods = alloc_str_arr(MAX_WIFI, MAX_LEN);
        int i=0;
        for (i = 0; i < n; i++)
            strncpy(g_mods[i], arr[i], MAX_LEN);
    }
    free_string_array(&arr, n);
    return n;
}

static void store_dev_list(){
    const char* sdio_dev_path = "/sys/bus/sdio/devices";
    const char* usb_dev_path = "/sys/bus/usb/devices";
    const char* pci_dev_path = "/sys/bus/pci/devices";
    g_mod_alias_last_idx = -1;
    all_dev_count = count_dir(sdio_dev_path)
            + count_dir(usb_dev_path)
            + count_dir(pci_dev_path);
    ALOGD("all_dev_count=%d", all_dev_count);
    g_mod_alias = alloc_str_arr(all_dev_count, MAX_SMALL_BUFFER_SIZE);

    //search sdio
    int r = walk_dir(sdio_dev_path, "\\modalias$", WS_DEFAULT | WS_FOLLOWLINK,
            0);
    if (r != 0)
        ALOGE("walk_dir ret=%d", r);
//    int i=0;
//    for(i=0;i<=g_mod_alias_last_idx;i++)
//        ALOGD("g_mod_alias[%d]=%s", i, g_mod_alias[i]);

    //search usb
    r = walk_dir(usb_dev_path, "\\modalias$", WS_DEFAULT | WS_FOLLOWLINK, 0);
    if (r != 0)
        ALOGE("walk_dir ret=%d", r);
    //    for(i=0;i<=g_mod_alias_last_idx;i++)
    //        ALOGD("mod_alias[%d]=%s", i, g_mod_alias[i]);
    //search pci
    r = walk_dir(pci_dev_path, "\\modalias$", WS_DEFAULT | WS_FOLLOWLINK, 0);
    if (r != 0)
        ALOGE("walk_dir ret=%d", r);
#ifdef PC_DEBUG
    //add test data
    strncpy(g_mod_alias[++g_mod_alias_last_idx], "sdio:c07v024CdB723", MAX_SMALL_BUFFER_SIZE);
    strncpy(g_mod_alias[++g_mod_alias_last_idx], "usb:v0BDAp818Bd0110dc00dsc00dp00icFFiscFFipFFin00", MAX_SMALL_BUFFER_SIZE);
    strncpy(g_mod_alias[++g_mod_alias_last_idx], "usb:v0BDAp8191d0110dc00dsc00dp00ic01isc01ip01in00", MAX_SMALL_BUFFER_SIZE);
    strncpy(g_mod_alias[++g_mod_alias_last_idx], "pci:v00008086d00000C00sv00001043sd00008534bc06sc00i00", MAX_SMALL_BUFFER_SIZE);
#endif
        g_mod_alias_count = g_mod_alias_last_idx + 1;
}

int init_wifi_mod_mapper() {
    ALOGD("init_wifi_mod_mapper");
    //if wifi_mod_load_list.csv exist
    g_manual_mod_count = load_manual_list();
    if (g_manual_mod_count>0){
        ALOGD("use manual insert list");
        return 0;
    }
    if (create_map_table())
        return -1;
    ALOGD("use auto insert list");
    store_dev_list();
    return 0;
}

static void free_heap() {
    if (g_mods)
        free_string_array(&g_mods, g_mods_count);
    if (g_mod_alias)
        free_string_array(&g_mod_alias, all_dev_count);
    if(g_wifi_module_mapper_cfg_str)
        free(g_wifi_module_mapper_cfg_str);
}

void uninit_wifi_mod_mapper() {
    ALOGD("uninit_wifi_mod_mapper");
    free_heap();
}

#ifdef PC_DEBUG

static void test() {
    ALOGD("wifi_mod_mapper start");
    if (!init_wifi_mod_mapper()) {
        char** mod_names = NULL;
        int i=0;
        mod_names = (char**)malloc(sizeof(char*) * MAX_WIFI);
        for(i=0;i<MAX_WIFI;i++){
            mod_names[i]=(char*)malloc(sizeof(char)*MAX_LEN);
            memset(mod_names[i], 0, sizeof(char)*MAX_LEN);
        }
        int wifiModCount = get_insmod_names((char**) mod_names);
        ALOGD("wifiModCount=%d", wifiModCount);
        if (mod_names != NULL) {
            for (i = 0; i < wifiModCount; i++) {
                if (mod_names[i]!=NULL && strlen(mod_names[i]) > 0) {
                    ALOGD("module name: %s", mod_names[i]);
                }
            }
        }
        for(i=0;i<MAX_WIFI;i++){
            free(mod_names[i]);
        }
        free(mod_names);
        uninit_wifi_mod_mapper();
    }
    ALOGD("wifi_mod_mapper finish");
}

int main() {
    int i = 0;
    for(i=0;i<1;i++){
        ALOGD("i======%d s", i);
        test();
        ALOGD("i======%d e", i);
    }
    return 0;
}
#endif
