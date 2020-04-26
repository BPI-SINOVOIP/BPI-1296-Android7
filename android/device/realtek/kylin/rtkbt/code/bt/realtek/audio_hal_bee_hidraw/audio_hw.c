/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vf_audio_hw"
/*#define LOG_NDEBUG 0*/

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cutils/log.h>
#include <cutils/str_parms.h>
#include <cutils/properties.h>
#include <cutils/sched_policy.h>
#include <utils/ThreadDefs.h>
#include <hardware/hardware.h>
#include <system/audio.h>
#include <hardware/audio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
//#include <tinyalsa/asoundlib.h>
#include <semaphore.h>

// add by jim
#include <sys/time.h>
#include "voice_hidraw.h"
#define PCM_LOG (1)

static int record_vld;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
    eRECORD_START = 0,
    eRECORD_STOP,
    eRECORD_OP_MAX
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FLY_VEN_DEC_IN_SIZE  (40)
#define FLY_VEN_DEC_OUT_SIZE (320)

// add for Bee
#define BEE_VOICE_FRAME_SIZE    (60)
#define BEE_VEN_DEC_IN_SIZE     (120)

#define BEE_VEN_DEC_OUT_SIZE    (240)
#define BEE_VOICE_DEC_IN_SIZE   (58)
const unsigned char H2_Header[4] = {0x08, 0x38, 0xC8, 0xF8};

//Path for encoded & decoded files
#ifndef RTK_STORE_VR_DEC_PATH
#define RTK_STORE_VR_DEC_PATH "/data/vr/DEC_"
#endif

#ifndef RTK_STORE_VR_READ_PATH
#define RTK_STORE_VR_READ_PATH "/data/vr/READ_"
#endif

char file_decode_path[60] = {0};
char file_read_path[60] = {0};

FILE * flip_decode = NULL;
FILE * flip_read = NULL;

#define BEE_VEN_NOTIF_LEN   (20)
#define BEE_VEN_FIFO_SIZE   (80)


// end for Bee

// for MDT
#define MDT_VOICE_DEC_IN_SIZE   (235)
// end for MDT


#define FLY_VEN_NOTIF_LEN   (20)
#define FLY_VEN_FIFO_SIZE   (10)

struct audio_device
{
    struct audio_hw_device hw_device;
    pthread_mutex_t lock;
    bool standby;
};

struct vf_stream_in
{
    struct audio_stream_in stream;
};

// modify for Bee
typedef struct
{
    unsigned char data[BEE_VEN_DEC_OUT_SIZE];
}   t_voice_item;


// modify for Bee
typedef struct
{
    t_voice_item notifi_fifo[80];   //15*80/2 = 600ms
    unsigned int head;
    unsigned int tail;
    unsigned int vld;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}   t_voice_fifo_cb;


typedef struct
{
    unsigned char opcode;
    unsigned char param[8];
} t_vf_sock_param __attribute__ ((packed));

static t_voice_fifo_cb voice_fifo_blk;
static pthread_t vf_sock_client_service;
static pthread_t vf_sock_client_ctrl_service;

// modified by jim
static unsigned char dec_in_data[BEE_VEN_DEC_IN_SIZE];
//static unsigned char dec_in_data[MDT_VOICE_DEC_IN_SIZE];
static short dec_out_data[BEE_VEN_DEC_OUT_SIZE];

static unsigned int v_item_left = 0;
static unsigned int v_item_cp = 0;
static t_voice_item * p_cur_item;

static int vf_socket_ctrl_fd;
static int vf_socket_ctrl_ready;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void vf_voice_fifo_put(t_voice_fifo_cb * p_cb, unsigned char* p_v_data, int v_size);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/* set thread proid to ANDROID_PRIORITY_AUDIO */
void vf_set_prio(void)
{
    int rc = 0;
    int tid = gettid();

    return ;

    rc = set_sched_policy(tid, SP_FOREGROUND);
    if (rc)
    {
        ALOGD("failed to change sched policy, tid %d, err: %d", tid, errno);
    }

    if (setpriority(PRIO_PROCESS, tid, ANDROID_PRIORITY_AUDIO) < 0)
    {
        ALOGD("failed to change priority tid: %d to %d", tid, ANDROID_PRIORITY_AUDIO);
    }
}

/*
    data chan socket
*/
void * vf_socket_client_looper(void * parg)
{
    int sockfd;
    socklen_t len;
    struct sockaddr_un address;
    int result;
    int nread;

    static unsigned char    sbc_dec_buf[BEE_VOICE_DEC_IN_SIZE*10];
    static unsigned long   sbc_dec_widx = 0;
    static unsigned long   sbc_dec_ridx = 0;
    int res = 0;
    int written = 0;

con_init:
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, VOICE_DATA_PATH);
    address.sun_path[0]=0;
    len =  strlen(VOICE_DATA_PATH)  + offsetof(struct sockaddr_un, sun_path);

con_retry:
    result = connect(sockfd, (struct sockaddr*)&address, len);
    if(result == -1)
    {
        ALOGW("data chan socket %s connect errno=[%d], err=[%s]", VOICE_DATA_PATH,  errno, strerror(errno));
        sleep(2);
        goto con_retry;
    }
    else
    {
        ALOGW("data chan socket connect ok");
    }

    while(1)
    {
        // modified by jim
        nread = read(sockfd, dec_out_data, BEE_VEN_DEC_OUT_SIZE);

#if (PCM_LOG == 1)
        ALOGW("voice data size: %d", nread);
#endif
        if(nread == BEE_VEN_DEC_OUT_SIZE)
        {
            if(record_vld == eRECORD_START) {
                vf_voice_fifo_put(&voice_fifo_blk, (unsigned char *)dec_out_data, BEE_VEN_DEC_OUT_SIZE );
#if (PCM_LOG == 1)
                if(flip_decode != NULL)
                    fwrite(dec_out_data, BEE_VEN_DEC_OUT_SIZE, 1, flip_decode);
#endif
            }else{
                ALOGW("Status %ld, ignore.", record_vld);
            }
        }
        else
        {
            ALOGW("read data chan err1:%d maybe closed exit thread", nread);
            break;
        }
    }

    close(sockfd);

    ALOGW("data chan socket closed");

    sleep(1);

    ALOGW("data chan socket re-connect");

    goto con_init;

    return NULL;
}

void vf_socket_client_init(void)
{
    //  to do
}

void vf_socket_client_deinit(void)
{
    //  to do
}

void vf_socket_client_start(void)
{
    pthread_create(&vf_sock_client_service, NULL, vf_socket_client_looper, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    ctrl chan socket
*/
void * vf_socket_client_ctrl_looper(void * parg)
{
    int nread;
    socklen_t len;
    struct sockaddr_un address;
    int result;
    t_vf_sock_param server_msg;
    int try_cnt = 100;

con_init:
    vf_socket_ctrl_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, VOICE_CTRL_PATH);
    address.sun_path[0]=0;
    len =  strlen(VOICE_CTRL_PATH)  + offsetof(struct sockaddr_un, sun_path);

con_retry:
    result = connect(vf_socket_ctrl_fd, (struct sockaddr*)&address, len);
    if(result == -1)
    {
        ALOGW("ctrl chan socket %s connect errno=[%d], err=[%s]", VOICE_CTRL_PATH, errno, strerror(errno));
        sleep(2);
        goto con_retry;
    }
    else
    {
        ALOGW("ctrl chan socket connect ok");
    }

    vf_socket_ctrl_ready = 1;
    try_cnt = 100;

    while(1)
    {
        nread = read(vf_socket_ctrl_fd, &server_msg, sizeof(t_vf_sock_param));
        if(nread == sizeof(t_vf_sock_param))
        {
            if(server_msg.opcode == 0xff)
            {
                ALOGW("ctlr chan recv quit msg and quit from thread");
                break;
            }
            else if(server_msg.opcode == 0xfe)
            {
                switch(server_msg.param[0])
                {
                    case 0:
                        ALOGW("ctrl chan recv rcu disconnected");
                        //vr will be reset when rcu is disconnected
                        record_vld = eRECORD_STOP;
                        break;
                    case 1:
                        ALOGW("ctrl chan recv rcu connected");
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            ALOGW("ctrl chan read err %d %d", nread, ((unsigned char *)&server_msg)[0]);
            try_cnt --;
            if(try_cnt == 0)
            {
                break;
            }
        }
    }

    close(vf_socket_ctrl_fd);

    ALOGW("ctrl chan socket closed");

    sleep(1);

    ALOGW("ctrl chan socket re-connect");

    goto con_init;

    return NULL;
}

void vf_socket_client_ctrl_send(unsigned char * p_data, unsigned int len)
{
    if(vf_socket_ctrl_ready)
    {
        write(vf_socket_ctrl_fd, p_data, len);
    }
}

void vf_socket_client_ctrl_init(void)
{
    vf_socket_ctrl_ready = 0;
}

void vf_socket_client_ctrl_deinit(void)
{
    //  to do
}

void vf_socket_client_ctrl_start(void)
{
    pthread_create(&vf_sock_client_ctrl_service, NULL, vf_socket_client_ctrl_looper, NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* voice queue */
static void vf_voice_fifo_init(t_voice_fifo_cb * p_cb)
{
    p_cb->head = 0;
    p_cb->tail = 0;
    p_cb->vld = 0;

    pthread_mutex_init(&p_cb->mutex, NULL);
    pthread_cond_init(&p_cb->cond, NULL);
}

static void vf_voice_fifo_deinit(t_voice_fifo_cb * p_cb)
{
    pthread_cond_destroy(&p_cb->cond);
    pthread_mutex_destroy(&p_cb->mutex);
}

static void vf_voice_fifo_reset(t_voice_fifo_cb * p_cb)
{
    pthread_mutex_lock(&p_cb->mutex);
    p_cb->head = 0;
    p_cb->tail = 0;
    p_cb->vld = 0;
    pthread_mutex_unlock(&p_cb->mutex);
}

static void vf_voice_fifo_put(t_voice_fifo_cb * p_cb, unsigned char* p_v_data, int v_size)
{
    t_voice_item * p_data;

    pthread_mutex_lock(&p_cb->mutex);
    if(p_cb->vld >= BEE_VEN_FIFO_SIZE)
    {
        pthread_mutex_unlock(&p_cb->mutex);
        // just for debug
        ALOGE("====================vfmodule:full");
        //  cp data
        p_data = p_cb->notifi_fifo + p_cb->tail;

        // modified by jim
        //memcpy(p_data->data, p_v_data, BEE_VEN_DEC_OUT_SIZE * sizeof(short));
        memcpy(p_data->data, p_v_data, BEE_VEN_DEC_OUT_SIZE);
        // end by jim
        pthread_mutex_lock(&p_cb->mutex);
        p_cb->tail ++;
        if(p_cb->tail >= BEE_VEN_FIFO_SIZE)
            p_cb->tail = 0;
        p_cb->head ++;
        if(p_cb->head >= BEE_VEN_FIFO_SIZE)
            p_cb->head = 0;
        pthread_mutex_unlock(&p_cb->mutex);
        pthread_cond_signal(&p_cb->cond);
    }
    else
    {
        pthread_mutex_unlock(&p_cb->mutex);
        //  cp data
        p_data = p_cb->notifi_fifo + p_cb->tail;
        // modified by jim
        //memcpy(p_data->data, p_v_data, BEE_VEN_DEC_OUT_SIZE * sizeof(short));
        memcpy(p_data->data, p_v_data, BEE_VEN_DEC_OUT_SIZE);
        // end by jim
        pthread_mutex_lock(&p_cb->mutex);
        p_cb->vld ++;
        p_cb->tail ++;
        if(p_cb->tail >= BEE_VEN_FIFO_SIZE)
            p_cb->tail = 0;
        pthread_mutex_unlock(&p_cb->mutex);
        pthread_cond_signal(&p_cb->cond);
    }
}

static void vf_voice_fifo_get(t_voice_fifo_cb * p_cb, t_voice_item ** p_p_data)
{
    pthread_mutex_lock(&p_cb->mutex);
    do
    {
        if(p_cb->vld == 0)
        {
#if 1
            struct timeval now;
            struct timespec timeout;
            int retcode;

            gettimeofday(&now, NULL);
            if(now.tv_usec + 20000 >= 1000000)
            {
                timeout.tv_sec = now.tv_sec + 1;
                timeout.tv_nsec = (now.tv_usec + 20000 - 1000000) * 1000;
            }
            else
            {
                timeout.tv_sec = now.tv_sec;
                timeout.tv_nsec = (now.tv_usec + 20000) * 1000;
            }
            retcode = pthread_cond_timedwait(&p_cb->cond, &p_cb->mutex, &timeout);
            if (retcode == ETIMEDOUT)
            {
                pthread_mutex_unlock(&p_cb->mutex);
                *p_p_data = NULL;
                break;
            }
#else
            pthread_cond_wait(&p_cb->cond, &p_cb->mutex);
#endif
        }
        else
        {
            pthread_mutex_unlock(&p_cb->mutex);
            *p_p_data = p_cb->notifi_fifo + p_cb->head;
            break;
        }
    }
    while(1);
}

static void vf_voice_fifo_end(t_voice_fifo_cb * p_cb)
{
    pthread_mutex_lock(&p_cb->mutex);
    p_cb->vld --;
    p_cb->head ++;
    // modified by jim
    //if(p_cb->head >= FLY_VEN_FIFO_SIZE)
    if(p_cb->head >= BEE_VEN_FIFO_SIZE)
        p_cb->head = 0;
    pthread_mutex_unlock(&p_cb->mutex);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* API functions */

static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
    return 16000;
}

static int in_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    return 0;
}

static size_t in_get_buffer_size(const struct audio_stream *stream)
{
    return 240;
}

static audio_channel_mask_t in_get_channels(const struct audio_stream *stream)
{
    return AUDIO_CHANNEL_IN_MONO;
}

static audio_format_t in_get_format(const struct audio_stream *stream)
{
    return AUDIO_FORMAT_PCM_16_BIT;
}

static int in_set_format(struct audio_stream *stream, audio_format_t format)
{
    return 0;
}

static int in_standby(struct audio_stream *stream)
{
    return 0;
}

static int in_dump(const struct audio_stream *stream, int fd)
{
    return 0;
}

static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    return 0;
}

static char * in_get_parameters(const struct audio_stream *stream,
                                const char *keys)
{
    return strdup("");
}

static int in_set_gain(struct audio_stream_in *stream, float gain)
{
    return 0;
}

static ssize_t in_read(struct audio_stream_in *stream, void* buffer,
                       size_t bytes_p)
{
    unsigned char * p_buf = (unsigned char *)buffer;
    size_t bytes = bytes_p;
#if (PCM_LOG == 1)
    ALOGD("in_read:%d", bytes);
#endif
    if(record_vld == eRECORD_STOP) {
        ALOGW("Status Error, not opened !!!");
        return 0;
    }

    while(bytes != 0)
    {
        if(v_item_left == 0)
        {
            //  get a new item from fifo
            vf_voice_fifo_get(&voice_fifo_blk, &p_cur_item);
            if(p_cur_item == NULL)
            {
                //ALOGD("occur");
#if 0
                memset(buffer, 0x00, bytes_p);
                return bytes_p;
#else
                return 0;
#endif
            }
            // modified by jim
            //v_item_left = BEE_VEN_DEC_OUT_SIZE* sizeof(short);
            v_item_left = BEE_VEN_DEC_OUT_SIZE;
            v_item_cp = 0;
        }
        else
        {
            if(bytes >= v_item_left)
            {
                memcpy(p_buf, p_cur_item->data + v_item_cp, v_item_left);
                // add by jim
                if(flip_read != NULL)
                    fwrite(p_cur_item->data + v_item_cp, v_item_left, 1, flip_read);
                //end by jim
                p_buf += v_item_left;
                bytes -= v_item_left;
                v_item_cp += v_item_left;
                v_item_left = 0;
                vf_voice_fifo_end(&voice_fifo_blk);
            }
            else
            {
                memcpy(p_buf, p_cur_item->data + v_item_cp, bytes);
                // add by jim
                if(flip_read != NULL)
                    fwrite(p_cur_item->data + v_item_cp, bytes, 1, flip_read);
                //end by jim
                p_buf += bytes;
                v_item_left -= bytes;
                v_item_cp += bytes;
                bytes = 0;
            }
        }
    }
    //ALOGD("test-in_read:%d", bytes_p);

    return bytes_p;
}

static uint32_t in_get_input_frames_lost(struct audio_stream_in *stream)
{
    return 0;
}

static int in_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return 0;
}

static int in_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return 0;
}

static int adev_open_output_stream(struct audio_hw_device *dev,
                                   audio_io_handle_t handle,
                                   audio_devices_t devices,
                                   audio_output_flags_t flags,
                                   struct audio_config *config,
                                   struct audio_stream_out **stream_out)
{
    return 0;
}

static void adev_close_output_stream(struct audio_hw_device *dev,
                                     struct audio_stream_out *stream)
{
}

static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
    return 0;
}

static char * adev_get_parameters(const struct audio_hw_device *dev,
                                  const char *keys)
{
    return strdup("");
}

static int adev_init_check(const struct audio_hw_device *dev)
{
    return 0;
}

static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
    return -ENOSYS;
}

static int adev_set_master_volume(struct audio_hw_device *dev, float volume)
{
    return -ENOSYS;
}

static int adev_set_mode(struct audio_hw_device *dev, audio_mode_t mode)
{
    return 0;
}

static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
    return -ENOSYS;
}

static int adev_get_mic_mute(const struct audio_hw_device *dev, bool *state)
{
    return -ENOSYS;
}

static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
        const struct audio_config *config)
{
    return 0;
}

static int adev_open_input_stream(struct audio_hw_device *dev,
                                  audio_io_handle_t handle,
                                  audio_devices_t devices,
                                  struct audio_config *config,
                                  struct audio_stream_in **stream_in)
{
    struct stub_audio_device *ladev = (struct stub_audio_device *)dev;
    struct vf_stream_in *in;
    int ret;

// add by jim, to store files for encoded & decoded data
    struct timeval time_for_voice;
    char time_for_voice_str[20];
    int time_len = 0;
    int retu= 0;
// end by jim

    ALOGD("adev_open_input_stream >>>");

// add by jim, to store files for encoded & decoded data
#if (PCM_LOG == 1)
    gettimeofday(&time_for_voice, NULL);
    memset(time_for_voice_str, 0, 20);

    retu = snprintf(time_for_voice_str, 20, "%d", time_for_voice.tv_sec);
    ALOGW("jim@adev_open_input_stream: time length is %d", retu);
    if(retu> 5)
    {
        memset(file_decode_path, 0, 60);
        memset(file_read_path, 0, 60);
        strncpy(file_decode_path, RTK_STORE_VR_DEC_PATH, strlen(RTK_STORE_VR_DEC_PATH));
        strncpy(file_read_path, RTK_STORE_VR_READ_PATH, strlen(RTK_STORE_VR_READ_PATH));

        time_len = strlen(time_for_voice_str);
        strncat(file_decode_path + strlen(RTK_STORE_VR_DEC_PATH), time_for_voice_str + time_len - 5, 5);
        strncat(file_read_path + strlen(RTK_STORE_VR_READ_PATH), time_for_voice_str + time_len - 5, 5);
        ALOGW("decode file: %s", file_decode_path);
        ALOGW("read file: %s", file_read_path);

        flip_decode = fopen(file_decode_path, "w+");
        if(flip_decode == NULL)
            ALOGW("failed to open decode file %s",strerror(errno));

        flip_read = fopen(file_read_path, "w+");
        if(flip_read == NULL)
            ALOGW("failed to open read file %s",strerror(errno));
    }
#endif
// end by jim

    in = (struct vf_stream_in *)calloc(1, sizeof(struct vf_stream_in));
    if (!in)
        return -ENOMEM;

    in->stream.common.get_sample_rate = in_get_sample_rate;
    in->stream.common.set_sample_rate = in_set_sample_rate;
    in->stream.common.get_buffer_size = in_get_buffer_size;
    in->stream.common.get_channels = in_get_channels;
    in->stream.common.get_format = in_get_format;
    in->stream.common.set_format = in_set_format;
    in->stream.common.standby = in_standby;
    in->stream.common.dump = in_dump;
    in->stream.common.set_parameters = in_set_parameters;
    in->stream.common.get_parameters = in_get_parameters;
    in->stream.common.add_audio_effect = in_add_audio_effect;
    in->stream.common.remove_audio_effect = in_remove_audio_effect;
    in->stream.set_gain = in_set_gain;
    in->stream.read = in_read;
    in->stream.get_input_frames_lost = in_get_input_frames_lost;

    *stream_in = &in->stream;

    vf_voice_fifo_reset(&voice_fifo_blk);
    record_vld = eRECORD_START;
    ALOGD("adev_open_input_stream <<<");

    return 0;

err_open:
    free(in);
    *stream_in = NULL;
    return ret;
}

static void adev_close_input_stream(struct audio_hw_device *dev,
                                    struct audio_stream_in *stream)
{
    ALOGD("adev_close_input_stream >>>");

#if (PCM_LOG == 1)

    ALOGW("jim@adev_close_input_stream: close encode & decode files");
    if(flip_decode != NULL)
        fclose(flip_decode);
    if(flip_read != NULL)
        fclose(flip_read);
#endif

    record_vld = eRECORD_STOP;

    ALOGD("adev_close_input_stream <<<");
}

static int adev_dump(const audio_hw_device_t *device, int fd)
{
    return 0;
}

static int adev_close(hw_device_t *device)
{
    struct audio_device *adev = (struct audio_device *)device;

    free(device);
    return 0;
}

static int adev_open(const hw_module_t* module, const char* name,
                     hw_device_t** device)
{
    struct audio_device *adev;
    int ret;

    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
        return -EINVAL;

    adev = calloc(1, sizeof(struct audio_device));
    if (!adev)
        return -ENOMEM;

    adev->hw_device.common.tag = HARDWARE_DEVICE_TAG;
    adev->hw_device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    adev->hw_device.common.module = (struct hw_module_t *) module;
    adev->hw_device.common.close = adev_close;

    adev->hw_device.init_check = adev_init_check;
    adev->hw_device.set_voice_volume = adev_set_voice_volume;
    adev->hw_device.set_master_volume = adev_set_master_volume;
    adev->hw_device.set_mode = adev_set_mode;
    adev->hw_device.set_mic_mute = adev_set_mic_mute;
    adev->hw_device.get_mic_mute = adev_get_mic_mute;
    adev->hw_device.set_parameters = adev_set_parameters;
    adev->hw_device.get_parameters = adev_get_parameters;
    adev->hw_device.get_input_buffer_size = adev_get_input_buffer_size;
    adev->hw_device.open_output_stream = adev_open_output_stream;
    adev->hw_device.close_output_stream = adev_close_output_stream;
    adev->hw_device.open_input_stream = adev_open_input_stream;
    adev->hw_device.close_input_stream = adev_close_input_stream;
    adev->hw_device.dump = adev_dump;

    *device = &adev->hw_device.common;


    record_vld = eRECORD_STOP;
    vf_voice_fifo_init(&voice_fifo_blk);
    vf_socket_client_init();
    vf_socket_client_start();
    vf_socket_client_ctrl_init();
    vf_socket_client_ctrl_start();

    return 0;
}

static struct hw_module_methods_t hal_module_methods =
{
    .open = adev_open,
};

struct audio_module HAL_MODULE_INFO_SYM =
{
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AUDIO_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AUDIO_HARDWARE_MODULE_ID,
        .name = "vf audio HW HAL",
        .author = "The Android Open Source Project",
        .methods = &hal_module_methods,
    },
};
