/*
 * InputAPE_Utils.h
 *
 *  Created on: 2011/7/5
 *      Author: bruce_huang
 */

#ifndef INPUTAPE_UTILS_H_
#define INPUTAPE_UTILS_H_

#include "InputAPEDef.h"

void _ape_aligned_seek(ape_input_plugin_t *pThis);
int _ape_read_data(ape_input_plugin_t *pThis, int64_t bufsz);
int64_t _ape_get_frame_boundary(ape_input_plugin_t *pThis, int64_t time);

int64_t _ape_time_to_byte(ape_input_plugin_t *pThis, int64_t time);
int64_t _ape_byte_to_time(ape_input_plugin_t *pThis, int64_t byte);


#endif /* INPUTAPE_UTILS_H_ */
