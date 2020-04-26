#ifndef I2C_DEV_ACCESS_HEADER
#define I2C_DEV_ACCESS_HEADER

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <i2c-dev.h>

#ifndef CONFIG_I2C_SLAVE

#define VENUS_I2C_DEVICE	"/dev/i2c/0"

#define I2C_RDWR        0x0707  /* Combined R/W transfer (one stop only)*/
#define I2C_SLAVE       0x0703  /* Change slave address                 */

struct i2c_msg {
        unsigned short addr;     /* slave address                        */
        unsigned short flags;
#define I2C_M_TEN	        0x10	/* we have a ten bit chip address	*/
#define I2C_LOW_SPEED       0x20    /* I2C Low Speed - 33KHz for GemStar */
#define I2C_M_RD	        0x01
#define I2C_M_NOSTART       0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
#define I2C_M_NO_ABORT_MSG	0x0400   /* if abort occurs, don't print error message */
        unsigned short len;              /* msg length                           */
        unsigned char *buf;              /* pointer to msg data                  */
};
#if 0 /* duplicate definition with <linux/i2c-dev.h> */
struct i2c_rdwr_ioctl_data {
        struct i2c_msg *msgs;   /* pointers to i2c_msgs */
        int nmsgs;              /* number of i2c_msgs */
};
#endif
#else
/*add by yunfeng_han for i2c slave interface*/
#define VENUS_I2CSLAVE_DEVICE		"/dev/Venus_i2cslave"
#define VENUS_I2C_IOC_MAGIC             'k'
#define VENUS_I2C_IOC_CONFIG            _IOW(VENUS_I2C_IOC_MAGIC, 1, int)
#define VENUS_I2C_IOC_READ              _IOR(VENUS_I2C_IOC_MAGIC, 2, int)
#define VENUS_I2C_IOC_WRITE             _IOW(VENUS_I2C_IOC_MAGIC, 3, int)
typedef struct __I2CSLAVE_CONFIG{
	char SubAddr;
	char RWFlag; //0  for master to rw, 1 only for master to Read, 2 only for master to write
	char Width;//0 1byte,1 2bytes, 2 4bytes, 3 8bytes
}I2CSLAVE_CONFIG;
typedef struct __I2CSLAVE_READ{
        char SubAddr;
        char ReadBuf[8];
}I2CSLAVE_READ;
typedef struct __I2CSLAVE_WRITE{
        char SubAddr;
        char WriteBuf[8];
}I2CSLAVE_WRITE;
//end i2c slave interface
#endif

#endif
