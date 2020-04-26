#ifndef _RTK_GPIO_H_
#define _RTK_GPIO_H_

#include <hardware/hardware.h>

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>


typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

/**
 *  * The id of this module
 *   */
#define GPIO_HARDWARE_MODULE_ID "rtk_gpio"
#define GPIO_HARDWARE_E         "GPIO_E"

/**
 *  * Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
 *   * and the fields of this data structure must begin with hw_module_t
 *    * followed by module specific information.
 *     */
struct gpio_module_t {
    struct hw_module_t common;
};

/**
 *  * Every device data structure must begin with hw_device_t
 *   * followed by module specific public methods and attributes.
 *    */
struct gpio_device_t {
    struct hw_device_t common;
    int (*test) (struct gpio_device_t *dev, char* pMsg);
    int (*gpio_read) (struct gpio_device_t *dev, int gpio_num, UINT8* res);
    int (*gpio_write) (struct gpio_device_t *dev, int gpio_num, int value, UINT8* res);
};

/** convenience API for opening and closing a device */

static inline int gpio_dev_open(const struct hw_module_t* module, const char* name, struct gpio_device_t** device) {
    return module->methods->open(module,
            name, (struct hw_device_t**)device);
}

static inline int gpio_dev_close(struct gpio_device_t* device) {
    return device->common.close(&device->common);
}

#endif // _RTK_GPIO_H_
