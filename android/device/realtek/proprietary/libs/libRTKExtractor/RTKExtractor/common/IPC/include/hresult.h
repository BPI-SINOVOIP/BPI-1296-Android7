#ifndef __H_RESULT_H__
#define __H_RESULT_H__

#define H_FLAG_SUCCESS                  (0x10000000)
#define H_FLAG_ERROR                    (0x20000000)
#define H_FLAG_WARNNING                 (0x40000000)

#define H_TYPE_ERROR_GENERAL            ((H_FLAG_ERROR)   |     (0x00010000))
#define H_TYPE_ERROR_PIN                ((H_FLAG_ERROR)   |     (0x00020000))
#define H_TYPE_ERROR_ALLOC              ((H_FLAG_ERROR)   |     (0x00030000))
#define H_TYPE_ERROR_MEDIA_SAMPLE       ((H_FLAG_ERROR)   |     (0x00040000))
#define H_TYPE_ERROR_MEDIA_TYPE         ((H_FLAG_ERROR)   |     (0x00050000))
#define H_TYPE_ERROR_FILTER             ((H_FLAG_ERROR)   |     (0x00060000))
#define H_TYPE_ERROR_STATE              ((H_FLAG_ERROR)   |     (0x00070000))
#define H_TYPE_ERROR_ENUM_PINS          ((H_FLAG_ERROR)   |     (0x00080000))
#define H_TYPE_ERROR_ENUM_FILTERS       ((H_FLAG_ERROR)   |     (0x00090000))
#define H_TYPE_ERROR_EVENT              ((H_FLAG_ERROR)   |     (0x000a0000))

/*****************************************************************************/
/* Success codes                                                             */
/*****************************************************************************/
#define S_OK                            ((H_FLAG_SUCCESS) |     (0x00000000))
#define S_FALSE                         ((H_FLAG_SUCCESS) |     (0x00000001))

/*****************************************************************************/
/* Warnning codes                                                            */
/*****************************************************************************/
#define W_FUNC_MIS_USED                 ((H_FLAG_WARNNING)      | (0x40000001))

/*****************************************************************************/
/* Error codes                                                               */
/*****************************************************************************/
// H_TYPE_ERROR_GENERAL
#define E_FAIL                          ((H_TYPE_ERROR_GENERAL) | (0x00000001))
#define E_GEN_POINTER_NULL              ((H_TYPE_ERROR_GENERAL) | (0x00000002))
#define E_GEN_UNEXPECTED                ((H_TYPE_ERROR_GENERAL) | (0x00000003))
#define E_GEN_MEM_ARRAY_BOUNDARY        ((H_TYPE_ERROR_GENERAL) | (0x00000004))
#define E_TIMEOUT                       ((H_TYPE_ERROR_GENERAL) | (0x00000005))
#define E_IO                            ((H_TYPE_ERROR_GENERAL) | (0x00000006))
#define E_IO_UNKNOWN_PROTOCOL           ((H_TYPE_ERROR_GENERAL) | (0x00000007))

// H_TYPE_ERROR_PIN
#define E_PIN_NOT_CONNECT_SAME_PIN      ((H_TYPE_ERROR_PIN)     | (0x00000001))
#define E_PIN_ALREADY_CONNECTED         ((H_TYPE_ERROR_PIN)     | (0x00000002))
#define E_PIN_IO_TYPE_NOT_MATCH         ((H_TYPE_ERROR_PIN)     | (0x00000003))
#define E_PIN_NOT_CONNECTED             ((H_TYPE_ERROR_PIN)     | (0x00000004))

// H_TYPE_ERROR_STATE
#define E_STATE_WRONG                   ((H_TYPE_ERROR_STATE)   | (0x00000001))
#define E_STATE_NOT_STOPPED             ((H_TYPE_ERROR_STATE)   | (0x00000002))

// H_TYPE_ERROR_ALLOC
#define E_ALLOC_UNCOMMIT                ((H_TYPE_ERROR_ALLOC)   | (0x00000001))
#define E_ALLOC_COMMIT                  ((H_TYPE_ERROR_ALLOC)   | (0x00000002))
#define E_ALLOC_BUFFER_ACTIVE           ((H_TYPE_ERROR_ALLOC)   | (0x00000003))
#define E_ALLOC_BAD_ALIGN               ((H_TYPE_ERROR_ALLOC)   | (0x00000004))
#define E_ALLOC_UN_PROPERTY             ((H_TYPE_ERROR_ALLOC)   | (0x00000005))
#define E_ALLOC_RING_UPPER_BOUND        ((H_TYPE_ERROR_ALLOC)   | (0x00000006))

// H_TYPE_ERROR_MEDIA_TYPE
#define E_MEDIA_TYPE_NOT_ACCEPTED       ((H_TYPE_ERROR_MEDIA_TYPE)      | (0x00000001))

// H_TYPE_ERROR_ENUM_PINS
#define E_ENUM_PINS_OUT_OF_SYNC         ((H_TYPE_ERROR_ENUM_PINS)       | (0x00000001))

// H_TYPE_ERROR_ENUM_FILTERS
#define E_ENUM_FILTERS_OUT_OF_SYNC      ((H_TYPE_ERROR_ENUM_FILTERS)    | (0x00000001))

// NA: not available
// H_TYPE_ERROR_EVENT
#define E_EVENT_TIMEOUT                 ((H_TYPE_ERROR_EVENT)   | (0x00000001))
#define E_EVENT_NA                      ((H_TYPE_ERROR_EVENT)   | (0x00000002))
#define E_EVENT_ID_MISMATCH             ((H_TYPE_ERROR_EVENT)   | (0x00000003))

#endif
