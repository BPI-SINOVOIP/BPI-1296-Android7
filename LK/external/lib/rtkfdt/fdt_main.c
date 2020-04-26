#include <app.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/console.h>
#include <rtkfdt.h>

struct fdt_cmd_entry fdt_cmd_table [] = {
    DEF_FDT_CMD(info,   "info",                           "show internal vairables of fdt commands"),
    DEF_FDT_CMD(header, "header",                         "show fdt header"),
    DEF_FDT_CMD(addr,   "addr   [<addr>]",                "set/get FDT address"),
    DEF_FDT_CMD(cd,     "cd     [<path>]",                "change the current node"),
    DEF_FDT_CMD(pwd,    "pwd",                            "print the current node"),
    DEF_FDT_CMD(print,  "print  [<path>]",                "print sub-nodes and properties recursively"),
    DEF_FDT_CMD(ls,     "ls     [<path>]",                "print sub-nodes and properties"),
    DEF_FDT_CMD(get,    "get    <path> <prop>",           "get value from a property value"),
    DEF_FDT_CMD(set,    "set    <path> <prop> [<value>]", "set value to a property/create if not exists"),
    DEF_FDT_CMD(add,    "add    <path> <node>",           "add a new node"),
    DEF_FDT_CMD(del,    "del    <path> [<prop>]",         "del a node/property"),
    DEF_FDT_CMD(memrsv, "memrsv",                         "memreserve commands\n"
                  "           print                     - print\n"
                  "           add <addr> <size>         - add a new range\n"
                  "           del <index>               - delete a range\n"),
    DEF_FDT_CMD_END
};

static void show_usage(void)
{
    struct fdt_cmd_entry *f;
    puts("Flat Device Tree commands");
    puts("");
    puts("Usage:");
    FOREACH_FDT_CMD_ENTRY(f, fdt_cmd_table) {
        printf("  fdt %-30s - %s\n", f->cmd_fmt, f->desc);
    }
    puts(" using '--' in <path> to replace with path of the current node");
}

int rtk_fdt(int argc, const cmd_args *argv)
{
    struct fdt_cmd_entry *f;
    int ret = 0;

    if (argc < 2) {
        show_usage();
        return -1;
    }

    FOREACH_FDT_CMD_ENTRY(f, fdt_cmd_table) {
        if (strcmp(argv[1].str, f->name) != 0)
            continue;

        ret = f->func(argc-2, argv+2);
        switch (ret) {
        case 0:
            return 0;

        case -FDT_ERR_NOTFOUND:
            printf("[ERROR] <path> or <prop> not found.\n");
            return 0;

        case -FDT_ERR_NOTREADY:
            printf("[ERROR] DT is not ready. Please set a valid DT address.\n");
            return 0;

        case -FDT_ERR_BAD_ARG_NUM:
            printf("[ERROR] wrong argument number\n");
            break;

        case -FDT_ERR_INV_ARG:
            printf("[ERROR] wrong sub-command\n");
            break;

        default:  
            printf("[ERROR] command %s: returns %s<%d>\n", f->name, fdt_strerror(ret), ret);
            return 0;
        }
    }

    show_usage();
    return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("fdt", "Flat device tree commands", (console_cmd)&rtk_fdt)
STATIC_COMMAND_END(rtkfdt);
