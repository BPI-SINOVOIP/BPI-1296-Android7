/*
 * xml_util.h
 *
 *  Created on: 2014/3/4
 *      Author: bruce_huang
 */

#ifndef XML_UTIL_H_
#define XML_UTIL_H_

void gen_xml_node(char *id, char *value, char *out, int out_buf_len);
void dump_xml(char *file_path);
void insert_xml_node(char *node, char *file_path);
int is_target_node(char *line, char *node);
int is_end_tag(char *line);
void remove_space_tab_newline(char *src, char *dst);
void remove_tailing_newline(char *data);

#endif /* XML_UTIL_H_ */
