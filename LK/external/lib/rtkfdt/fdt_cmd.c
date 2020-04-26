#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <libfdt.h>
#include <lib/console.h>
#include <rtkfdt.h>

static void *fdt_base = NULL;
static char __buf_0[MAX_DIRECTORY_LENGTH] = "/";
static char __buf_1[MAX_DIRECTORY_LENGTH] = "/";
static char *last_dir = __buf_0;
static char *curr_dir = __buf_1;
static const char * __symbol[2] = { "/", "" };

static const char * get_path_combine_symbol(const char *path)
{
    return __symbol[path[strlen(path) - 1] == '/'];
}

static int fdt_is_ready(void)
{
    int ret;
    if (fdt_base == NULL)
        return 0;

    ret = fdt_check_header(fdt_base);
    if (ret)
        return 0;

    return 1;
}

int cmd_fdt_info(int argc, const cmd_args *argv)
{
    printf("---------------------------------------\n");
    printf(" fdt info\n");
    printf("---------------------------------------\n");
    printf("  fdt_addr  : 0x%016lx\n", (unsigned long)fdt_base);
    printf("  fdt_state : %s\n", fdt_is_ready() ? "Ready" : "Not ready");
    printf("  curr_node : %s\n", curr_dir);
    printf("  last_node : %s\n", last_dir);
    printf("---------------------------------------\n");
    return 0;    
}

int cmd_fdt_header(int argc, const cmd_args *argv)
{
    unsigned int v;
    char buf[20];
    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;
   
    printf("---------------------------------------\n");
    printf(" fdt header\n");
    printf("---------------------------------------\n");
    printf("  magic      : 0x%08x\n", fdt_version(fdt_base));
    printf("  totalsize  : 0x%08x\n", fdt_totalsize(fdt_base));
    v = fdt_version(fdt_base);
    printf("  version    : %d\n", v);
    printf("  last_comp_version : %d\n", fdt_last_comp_version(fdt_base));

    printf("  fdt_addr   : 0x%016lx ( size: 0x%04x )\n", 
        (unsigned long)fdt_base, fdt_totalsize(fdt_base));

    printf("   +0x%04x   : memreserve ( count: 0x%02x )\n",   
        fdt_off_mem_rsvmap(fdt_base), fdt_num_mem_rsv(fdt_base));

    snprintf(buf, 20, "0x%04x", fdt_size_dt_struct(fdt_base));
    printf("   +0x%04x   : dt_struct  ( size: %s )\n", 
        fdt_off_dt_struct(fdt_base), v >= 17 ? buf : "Unsupported");

    snprintf(buf, 20, "0x%04x", fdt_size_dt_strings(fdt_base));
    printf("   +0x%04x   : dt_strings ( size: %s )\n", 
        fdt_off_dt_strings(fdt_base), v >= 3 ? buf : "Unsupported");

    printf("---------------------------------------\n");
    return 0;    
}

int cmd_fdt_addr(int argc, const cmd_args *argv)
{
    if (argc == 0) {
        printf("GET fdt_addr: 0x%016lx\n", (unsigned long)fdt_base);
        return 0;
    }      

    strcpy(last_dir, "/");
    strcpy(curr_dir, "/");

    fdt_base = (void *)atoull(argv[0].str);
    printf("SET fdt_addr: 0x%016lx\n", (unsigned long)fdt_base);
    printf("SET fdt_cd: %s\n", curr_dir);
    
    if (fdt_is_ready())
        return 0;
    return -FDT_ERR_NOTREADY;
}

int cmd_fdt_print(int argc, const cmd_args *argv)
{
    cmd_args _argv[5];
    int i;

    if (argc > 4) 
         return -FDT_ERR_BAD_ARG_NUM;

    for (i = 0; i < argc; i++)
        _argv[i] = argv[i];
    
    _argv[argc].str = "-r";

    cmd_fdt_ls(argc + 1, _argv);
    
    return 0;
}

static inline int strlen_printable(const char *s)
{
    int i = 0;
    while (s[i]) {
        if (!isprint(s[i]))
            return -1;
        i += 1;
    }
    return i;
}

static int count_string_array_size(const void *data, int len)
{
    const char *s = (const char *)data;
    int p = 0;

    if (len == 0 || s[len-1] != 0)
        return 0;

    do {
        int x = strlen_printable(s);
        /*
         * x == -1 : non printable string
         * x == 0  : empty string
         */
        if (x == -1 || x == 0)
            return 0;

        s += x + 1;
        len -= x + 1;
        p += 1;
    } while (len > 0);

    return p;
}

static inline void print_tab(int lv)
{
    int i;
    for (i = 0; i < lv; i++)
        putchar('\t');
}

static void print_prop(const void *data, int len, int tab_cnt)
{
    int sc;
    int pc = tab_cnt >= 0 ? 2 : -1;

    if (len == 0)
        return;

    sc = count_string_array_size(data, len);
    if (sc > 0) {
        const char *s = data;
        printf("\"");
        while (sc > 1) {
            printf("%s\"", s);

            /* get next string */
            sc --;
            s += strlen(s) + 1;

            printf(", ");

            if (pc > 0 && (pc % 4) == 0) {
                printf("\n");
                print_tab(tab_cnt+1);
            }
            pc ++;

            printf("\"");
        }

        printf("%s\"", s);
        return;
    }
    
    if ((len % 4) == 0) {
        const unsigned int *p = data;
        printf("<");
        for (; len > 4; len -= 4, p++) {
            printf("0x%08x ", fdt32_to_cpu(*p));

            if (pc > 0 && (pc % 6) == 0) {
                printf("\n");
                print_tab(tab_cnt+1);
            }
            pc ++;
        }
        printf("0x%08x>", fdt32_to_cpu(*p));
    } else {
        const unsigned char *p = data;
        printf("[");
        for (; len > 1; len -= 1, p++) {
            printf("0x%02x ", (unsigned int)*p);

            if (pc > 0 && (pc % 14) == 0) {
                printf("\n");
                print_tab(tab_cnt+1);
            }
            pc ++;
        }
        printf("0x%02x]", (unsigned int)*p);
    }
}

int cmd_fdt_ls(int argc, const cmd_args *argv)
{   
    int lv;
    int ret;
    int offset;
    int max_lv = 1;
    const char *path = curr_dir;

    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    if (argc >= 1 && strcmp(argv[argc - 1].str, "-r") == 0)
        max_lv = 0xff;

    if (argc >= 1 && argv[0].str[0] != '-' && strcmp(argv[0].str, "--") != 0)
        path = argv[0].str;

    ret = fdt_path_offset(fdt_base, path);
    if (ret < 0)
        return ret;

    offset = ret;
    lv = 0;
    while (lv >= 0) {
        int next_offset;
        int tag = fdt_next_tag(fdt_base, offset, &next_offset);
        const char *name;
        const struct fdt_property *prop;
        int len;

        switch (tag) {
        case FDT_BEGIN_NODE:
            name = fdt_get_name(fdt_base, offset, &len);
            if (name == NULL)
                return len;

            if (lv <= max_lv) {
                print_tab(lv);
                printf("%s {\n", name[0] == '\0' ? "/" : name);
            }
            lv ++;
            break;

        case FDT_END_NODE:
            lv --;
            if (lv <= max_lv) {
                print_tab(lv);
                printf("};\n");
            }
            if (lv == 0)
                goto done;
            break;

        case FDT_PROP:
            if (lv > max_lv)
                break;

            prop = fdt_offset_ptr(fdt_base, offset, sizeof(*prop));
            name = fdt_string(fdt_base, fdt32_to_cpu(prop->nameoff));
            len  = fdt32_to_cpu(prop->len);
            if (len < 0) {
                return len;
            } else if (len == 0) {
                print_tab(lv);
                printf("%s;\n", name);
            } else {
                print_tab(lv);
                printf("%s = ", name);
                print_prop(prop->data, len, lv);
                printf(";\n");
            }
            break;

        case FDT_NOP:        
            break;

        case FDT_END:
        default:
            return -1;
        }
        
        offset = next_offset;
    }
done:
    return 0;
}

int cmd_fdt_cd(int argc, const cmd_args *argv)
{
    int ret = 0;
    int len;

    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    if (argc == 0 || strcmp(argv[0].str, "~") == 0 || strcmp(argv[0].str, "/") == 0) {
        strcpy(last_dir, curr_dir);
        strcpy(curr_dir, "/");
    } else if (strcmp(argv[0].str, ".") == 0) {
        /* do nothing here */
    } else if (strcmp(argv[0].str, "..") == 0) {
        strcpy(last_dir, curr_dir);
        *strrchr(curr_dir, '/') = 0;
        if (curr_dir[0] == 0)
            strcpy(curr_dir, "/");
    } else if (strcmp(argv[0].str, "-") == 0) {
        char *temp = last_dir;
        last_dir = curr_dir;
        curr_dir = temp;
    } else if (argv[0].str[0] == '/') {
        
        ret = fdt_path_offset(fdt_base, argv[0].str);
        if (ret < 0)
            return ret;

        if (strlen(argv[0].str) >= MAX_DIRECTORY_LENGTH)
            return -FDT_ERR_BUF_OVERFLOW;

        strcpy(last_dir, curr_dir);
        strcpy(curr_dir, argv[0].str);
    } else {
        char buf[MAX_DIRECTORY_LENGTH];        
        len = strlen(curr_dir);
        
        ret = snprintf(buf, MAX_DIRECTORY_LENGTH, "%s%s%s", 
            curr_dir, get_path_combine_symbol(curr_dir), argv[0].str);
        if (ret <= 0)
            return -FDT_ERR_BUF_OVERFLOW;

        ret = fdt_path_offset(fdt_base, buf);
        if (ret < 0)
            return ret;

        strcpy(last_dir, curr_dir);
        strcpy(curr_dir, buf);           
    }

    len = strlen(curr_dir);
    if (len > 1 && curr_dir[len - 1] == '/')
        curr_dir[len - 1] = 0;
    
    printf("SET cd: %s\n", curr_dir);    
    return 0;
}


int cmd_fdt_pwd(int argc, const cmd_args *argv)
{   
    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    printf("GET cd: %s\n", curr_dir);
    return 0;
}

static int parse_prop(unsigned char *buf, int *len, int argc, const cmd_args *argv)
{
    *len = 0;

    if (argc == 0)
        return 0;

    if ((argv[0].str[0] == '<' || argv[0].str[0] == '[') &&
        strrchr(argv[argc-1].str, argv[0].str[0]+2) != NULL) {
        const char *inp;
        char end_char =  argv[0].str[0] + 2;
        int  size     =  argv[0].str[0] == '<' ? 4 : 1;
        
        inp = argv[0].str + 1;

        while (argc > 0 && *inp != end_char) {
            char *endp;
            int val;

            if (*inp == 0) {
                argv ++;
                argc --;

                inp = argv[0].str;
                continue;
            }                           

            if (size == 4) {
                val = strtoul(inp, &endp, 0);
                *(unsigned int *)buf = cpu_to_fdt32(val);

                buf += 4;
                *len += 4;

                if ((endp - inp) <= 0)
                    break;
            
                inp = endp;

                while (*inp == ' ')
                    inp++;
            } else { /* size == 1 */
                if (!isxdigit(*inp))
                    break;
                val = strtoul(inp, &endp, 16);
                *buf = val & 0xff;

                buf  += 1;
                *len += 1;

                inp = endp;
            }
        }
    } else {
        while (argc > 0) {
            int slen = strlen(argv[0].str);
            strcpy((char *)buf, argv[0].str);

            buf += slen + 1;
            *len += slen + 1;

            argc --;
            argv ++;
        }
    }
    return 0;
}

int cmd_fdt_set(int argc, const cmd_args *argv)
{
    const char *path = argv[0].str;
    const char *prop = argv[1].str;
    unsigned char buf[2048];
    int len;
    int ret;
    int offset;
    
    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    if (argc < 2)
        return -FDT_ERR_BAD_ARG_NUM;

    if (strcmp(argv[0].str, "--") == 0)
        path = curr_dir;

    offset = fdt_path_offset(fdt_base, path);
    if (offset < 0)
        return offset;

    ret = parse_prop(buf, &len, argc - 2, argv + 2);
    if (ret) 
        return ret;

    return fdt_setprop(fdt_base, offset, prop, buf, len);
}

int cmd_fdt_get(int argc, const cmd_args *argv)
{
    const char *path = argv[0].str;
    const char *prop = argv[1].str;
    int offset;
    int len;
    const void *p;

    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    if (argc < 2)
        return -FDT_ERR_BAD_ARG_NUM;

    if (strcmp(argv[0].str, "--") == 0)
        path = curr_dir;

    offset = fdt_path_offset(fdt_base, path);
    if (offset < 0)
        return offset;

    p = fdt_getprop(fdt_base, offset, prop, &len);
    if (len < 0)
        return len;

    printf("%s = {\n", path);
    print_tab(1);

    if (len != 0) {
        printf("%s = ", prop);
        print_prop(p, len, 1);
        printf(";\n");
    } else
        printf("%s;\n", prop);

    printf("};");

    return 0;    
}

int cmd_fdt_add(int argc, const cmd_args *argv)
{
    const char *path = argv[0].str;
    const char *node = argv[1].str;
    int offset;

    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;
        
    if (argc < 2)
        return -FDT_ERR_BAD_ARG_NUM;

    if (strcmp(argv[0].str, "--") == 0)
        path = curr_dir;

    offset = fdt_path_offset(fdt_base, path);
    if (offset < 0)
        return offset;

    offset = fdt_add_subnode(fdt_base, offset, node);
    if (offset < 0)
        return offset;

    printf("ADD node %s%s%s\n", path, get_path_combine_symbol(path), node);

    return 0;
}

int cmd_fdt_del(int argc, const cmd_args *argv)
{
    const char *path = argv[0].str;
    const char *prop = argv[1].str;
    int offset;
    int ret = 0;

    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    if (argc < 1)
        return -FDT_ERR_BAD_ARG_NUM;

    if (argc > 1 && strcmp(argv[0].str, "--") == 0)
        path = curr_dir;

    offset = fdt_path_offset(fdt_base, path);
    if (offset < 0)
        return offset;

    if (argc == 1) {
        ret = fdt_del_node(fdt_base, offset);
        printf("DEL node %s\n", path);
    } else  {
        ret = fdt_delprop(fdt_base, offset, prop);
        printf("DEL prop %s::%s\n", path, prop);
    }
    
    return ret;
}

int cmd_fdt_memrsv(int argc, const cmd_args *argv)
{
    uint64_t index, addr, size;
    int ret = 0;

    if (!fdt_is_ready())
        return -FDT_ERR_NOTREADY;

    if (argc == 0 || strcmp(argv[0].str, "print") == 0) {        
        int total, i;
        int print_u64 = 0;

        total = fdt_num_mem_rsv(fdt_base);

        for (i = 0; i< total; i++) {
            ret = fdt_get_mem_rsv(fdt_base, i, &addr, &size);
            if (ret)
                return ret;

            if (addr > 0xffffffff || size > 0xffffffff) {
                print_u64 = 1;
                break;
            }
        }

        printf("---------------------------------------\n");

        printf("memreserve ");
        if (total == 0) {
            printf("( no entry )\n");
            return 0;
        } 
                    
        printf("( %d %s )\n", total, total > 1 ? "entries": "entry");
        printf("    | address   +size\n");
        for (i = 0; i< total; i++) {
            ret = fdt_get_mem_rsv(fdt_base, i, &addr, &size);
            if (ret)
                break;
            if (print_u64)
                printf(" %02x | 0x%016llx +0x%016llx\n", i, addr, size);
            else
                printf(" %02x | 0x%08x +0x%08x\n", i, 
                    (unsigned int)addr, (unsigned int)size);
        }        
    } else if (strcmp(argv[0].str, "add") == 0) {
        if (argc != 3)
            return -FDT_ERR_BAD_ARG_NUM;
        addr = strtoul(argv[1].str, NULL, 16);
        size = strtoul(argv[2].str, NULL, 16);

        ret = fdt_add_mem_rsv(fdt_base, addr, size);

    } else if (strcmp(argv[0].str, "del") == 0) {
        if (argc != 2)
            return -FDT_ERR_BAD_ARG_NUM;
        index = strtoul(argv[1].str, NULL, 16);

        ret = fdt_del_mem_rsv(fdt_base, index);

    } else 
        return -FDT_ERR_INV_ARG;
    return ret;
}
