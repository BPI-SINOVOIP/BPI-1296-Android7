#pragma once

#include <lib/console.h>
#include <libfdt.h>

#define MAX_DIRECTORY_LENGTH 256

struct fdt_cmd_entry {
    const char *name;
    const char *cmd_fmt;
    const char *desc;
    int (*func)(int, const cmd_args *);
};

#define FDT_ERR_NOTREADY        (FDT_ERR_MAX+1) 
#define FDT_ERR_BAD_ARG_NUM     (FDT_ERR_MAX+2)
#define FDT_ERR_INV_ARG         (FDT_ERR_MAX+3)
#define FDT_ERR_BUF_OVERFLOW    (FDT_ERR_MAX+4)

#define DEF_FDT_CMD(_name, _cmd_fmt,  _desc) \
    {                    \
        .name = #_name,  \
        .cmd_fmt = _cmd_fmt, \
        .desc = _desc,   \
        .func = cmd_fdt_ ## _name,   \
    }
#define DEF_FDT_CMD_END   \
     { .name = NULL }


#define FOREACH_FDT_CMD_ENTRY(_entry, _table) \
    for (_entry = (_table); _entry->name != NULL; _entry++)

int cmd_fdt_info(int argc, const cmd_args *argv);
int cmd_fdt_header(int argc, const cmd_args *argv);
int cmd_fdt_addr(int argc, const cmd_args *argv);
int cmd_fdt_print(int argc, const cmd_args *argv);
int cmd_fdt_ls(int argc, const cmd_args *argv);
int cmd_fdt_cd(int argc, const cmd_args *argv);
int cmd_fdt_pwd(int argc, const cmd_args *argv);
int cmd_fdt_set(int argc, const cmd_args *argv);
int cmd_fdt_get(int argc, const cmd_args *argv);
int cmd_fdt_del(int argc, const cmd_args *argv);
int cmd_fdt_add(int argc, const cmd_args *argv);
int cmd_fdt_memrsv(int argc, const cmd_args *argv);


