#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <binder/ProcessState.h>
#include <gui/SidebandClient.h>
#include <gui/SidebandControl.h>
#include <gui/Surface.h>
#include <gui/BufferQueue.h>
#include <gui/IConsumerListener.h>
#include <gui/IProducerListener.h>
#include <hardware/gralloc.h>
#include <cutils/properties.h>
#include <android/native_window.h>

#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/BufferItemConsumer.h>
#include <utils/NativeHandle.h>


using namespace android;

#define CHECK assert
#define ASSERT_EQ(x, y) assert((x) == (y))

#define TEST_PRODUCER_CONSUMER      1
#define TEST_SIDEBAND_CONTROL_API   2

#define USAGE  "usage: test_sideband -[OPTION]..  \
                \n\t-p,\ttest buffer producer and consumer \
                \n\t-f,\ttest other functions  \
                \n"

// YUV conversion
#define MY(R,G,B) ((0.299)*R + (0.587)*G + (0.114)*B)
#define MU(R,G,B) (-(0.169)*R - (0.332)*G+ (0.501)*B)
#define MV(R,G,B) ( (0.450)*R - (0.419)*G - (0.081)*B )

#define W   100
#define H   100

#if 0
// unused now.
sp<SidebandClient> gClient;
#endif

bool testProducer;
bool testAPIs;

// copied from hwc HWLayer
struct private_yuv_data {
    unsigned int        phyAddr;
    unsigned int        size;
    int                 format;
    int                 width;
    int                 height;
    int                 stride;
    int                 usage;
    int                 share_fd;
    unsigned int        yuv_mode;
    int                 share_config_fd;
    unsigned long long  pts[2];
    int                 delay_mode;
    int                 delay_depth;
    unsigned int        IsForceDIBobMode;
    int                 init_frame;
    int                 deintflag; // 0:AUTODEINT 1:FORCEDEINT 2:FORCEPROGRESSIVE
    unsigned int        reserve[32-19];
};


const struct hw_module_t *      mGrallocModule;

void test_client_apis(){
    printf("[%s]\n",__func__);
    sp<SidebandClient> client;
    client = new SidebandClient();

    int w = W;
    int h = H;
    int format = HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP;
    int usage = GRALLOC_USAGE_HW_ION;

    sp<SidebandControl> control = client->createSurface(String8("Sideband Surface"),w,h,PIXEL_FORMAT_RGB_888);
    CHECK(control != NULL);
    CHECK(control->isValid());
    // get surface, but we won't use it
    sp<Surface> surface = control->getSurface();
    CHECK(surface != NULL);

    // then try to test each api, use SidebandControl API
    control->hide();
    control->show();
    control->setLayer(0);
    control->setAlpha(0.5);
    control->setPosition(0,0);
    control->setSize(50,50);
    Rect crop(0,0,10,10);
    control->setCrop(crop);

    client.clear();
    surface.clear();
}

const struct hw_module_t * getGrallocModule()
{
    if (!mGrallocModule)
        hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &mGrallocModule);
    return mGrallocModule;
}

int color_code = 0;

char pallet[]={0xff, 0xaa, 0xbb, 0xcc, 0x00, 0x10, 0x0f};

// NG, don't know why
void rgb_c_yuv420(char *pRGB, char *pYUV, int w, int h)
{
    char *pY = pYUV;
    char *pU = pYUV+(h*w);
    char *pV = pU+(h*w/4);

    char *pBGR = NULL;
    char R = 0;
    char G = 0;
    char B = 0;
    char Y = 0;
    char U = 0;
    char V = 0;
    double tmp = 0;
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            pBGR = pRGB + i*w*3 + j*3;
            R = *pBGR;
            G = *(pBGR+1);
            B = *(pBGR+2);
            printf("R %.2x G %.2x B %.2x\n",R,G,B);
            Y = MY(R,G,B);
            U = MU(R,G,B);
            V = MV(R,G,B);

            *(pY + i*w + j) = Y;
            if(i%2 == 0 && j%2 == 0)
            {
                *pU = U;
                pU++;
            }
            else
            {
                if( j%2 == 0)
                {
                    *pV++ = V;
                }
            }
        }
    }
}

void fill_color(char r, char g, char b, int w, int h, char *pYUV)
{
    // first, allocate rgb buffer
    char *pRGB = (char *)malloc(3*w*h);
    memset(pRGB,0x0,3*w*h);
    // second, filled with RGB Data
    char *pt = pRGB;
    for(int i=0;i<(w*h);i++)
    {
        *pt=r;
        *(pt+1)=g;
        *(pt+2)=b;
        pt=pt+3;
    }
    rgb_c_yuv420(pRGB, pYUV, w, h);
    free(pRGB);
}

void surface_render_frame(ANativeWindowBuffer* anb)
{
    //printf("[%s]\n",__func__);
    mali_gralloc_private_module_t * privateModule = (mali_gralloc_private_module_t *) getGrallocModule();
    struct private_yuv_data YUVData;
    void * srcVaddr;

    memset(&YUVData,0,sizeof(YUVData));
    buffer_handle_t handle = anb->handle;
    privateModule->GetYUVData((struct private_module_t const *) mGrallocModule, handle, (void *)&YUVData);
    privateModule->base.lock((gralloc_module_t const *) privateModule, handle, YUVData.usage | GRALLOC_USAGE_SW_READ_MASK, 0, 0, 0, 0, &srcVaddr);    
    //printf("srcVAddr %.8x size: %d dimension (%d %d)\n",(unsigned int)srcVaddr, YUVData.size, YUVData.width, YUVData.height);
    //fill_color(0xff,0,0,W,H,(char *)srcVaddr);
    //memset(srcVaddr, (pallet[color_code] + rand()%0xff)%0xff, YUVData.size);
    memset(srcVaddr, rand(), YUVData.size);
    //memset((void *)((int)srcVaddr+(YUVData.width*YUVData.height)/2), rand()%0xff, YUVData.size/2);
    color_code++;
    //memset(srcVaddr, 0, YUVData.size);
    if(color_code>=7) color_code = 0;
}

void surface_produce_one_frame(const sp<ANativeWindow>& anw)
{
    //printf("[%s]\n",__func__);
    status_t err;
    ANativeWindowBuffer* anb;
    //printf("Dequeue buffer from %p\n", anw.get());
    err = native_window_dequeue_buffer_and_wait(anw.get(), &anb);
    ASSERT_EQ(err, NO_ERROR);
    CHECK(anb!=NULL);
    // test...
    surface_render_frame(anb);
    // TODO: Q what's this for?
    sp<GraphicBuffer> buf(new GraphicBuffer(anb, false));
    // then do something
    //printf("Queue buffer to %p\n", anw.get());
    err = anw->queueBuffer(anw.get(), buf->getNativeBuffer(), -1);
}

void test_surface_producer_consumer()
{
    printf("[%s]\n", __func__);
    sp<SidebandClient> client;
    client = new SidebandClient();

    int w = W;
    int h = H;
    int format = HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP;
    int usage = GRALLOC_USAGE_HW_ION;

    int maxBufferSlack = 5;
    // TODO: createSurface set width and height
    sp<SidebandControl> control = client->createSurface(String8("Sideband Surface"),w,h,PIXEL_FORMAT_RGB_888);
    CHECK(control != NULL);
    CHECK(control->isValid());

    sp<Surface> surface = control->getSurface();
    CHECK(surface != NULL);
    // we won't call gbp directly, instead, we call API exposed by Surface
    //sp<IGraphicBufferProducer> gbp = surface->getIGraphicBufferProducer();
    printf("[Sideband] surface: %p\n",surface.get());
    // cast to ANativeWindow
    sp<ANativeWindow> mANW = surface;
    // TODO: which API is preferred?
    //ANativeWindow_setBuffersGeometry(mANW.get(), w,h,format);

    // configure window
    status_t err;
    err = native_window_set_buffers_format(mANW.get(), format);
    ASSERT_EQ(err, NO_ERROR);
    native_window_set_buffers_user_dimensions(mANW.get(), w, h);
    ASSERT_EQ(err, NO_ERROR);
    err = native_window_set_usage(mANW.get(), usage);
    int minUndequeuedBuffers;
    err = mANW.get()->query(mANW.get(),
        NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
        &minUndequeuedBuffers);


    int buf_count = maxBufferSlack + 1 + minUndequeuedBuffers;
    printf("Setting buffer count to %d\n",buf_count);
    err = native_window_set_buffer_count(mANW.get(),
        buf_count);
    ASSERT_EQ(err, NO_ERROR);

    // TODO: bad coding style;
    ANativeWindowBuffer* buf_array[50];

    for(int i=0;i<buf_count;i++)
    {
        err = native_window_dequeue_buffer_and_wait(mANW.get(), &(buf_array[i]));
        ASSERT_EQ(err, NO_ERROR);
    }

    for(int i=0;i<buf_count;i++)
    {
        err = mANW.get()->cancelBuffer(mANW.get(), buf_array[i], -1);
        ASSERT_EQ(err, NO_ERROR);
    }

    int TEMP = 1;
    while(TEMP > 0 && TEMP--)
    {
        surface_produce_one_frame(mANW);
        usleep(500000);
    }
    {
        native_handle_t * mNativeHandle;
        control->getSidebandStream(&mNativeHandle);

        sp<SurfaceComposerClient> mComposerClient = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());

        sp<SurfaceControl> mSurfaceControl = mComposerClient->createSurface(
                String8("Test Surface"), 100, 100, PIXEL_FORMAT_RGB_888, 0);
        CHECK(mSurfaceControl != NULL);
        CHECK(mSurfaceControl->isValid());


#if 1
        SurfaceComposerClient::openGlobalTransaction();
        mSurfaceControl->setLayer(INT_MAX - 1);
        mSurfaceControl->setPosition(0, 0);
        mSurfaceControl->setSize(300,300);
        mSurfaceControl->show();
        SurfaceComposerClient::closeGlobalTransaction(true);
#if 0
        SurfaceComposerClient::openGlobalTransaction();
        ASSERT_EQ(NO_ERROR, mSurfaceControl->setLayer(INT_MAX - 1));
        ASSERT_EQ(NO_ERROR, mSurfaceControl->setPosition(0, 0));
        ASSERT_EQ(NO_ERROR, mSurfaceControl->setSize(300,300));
        ASSERT_EQ(NO_ERROR, mSurfaceControl->show());
        ASSERT_EQ(NO_ERROR, mSurfaceControl->show());
        SurfaceComposerClient::closeGlobalTransaction(true);
#endif
#endif

        sp<Surface> mSurface  = mSurfaceControl->getSurface();
        CHECK(mSurface != NULL);
        sp<NativeHandle> mSourceHandle= NativeHandle::create(mNativeHandle, false);
        mSurface->setSidebandStream(mSourceHandle);



        //sp<SidebandClient> client2 = new SidebandClient();
        //sp<SidebandControl> control2 =   client2->binderSurface(mNativeHandle);

        int TEMP2 = 1000;
        while(true/*TEMP2 > 0 && TEMP2--*/) {
            static int x = 0, y = 0, w=100, h=100;
            //printf("setSize: %d \n", TEMP2);
            surface_produce_one_frame(mANW);
            //control2->setPosition(x, y);
            //control2->setSize(w, h);
            //control2->refresh();

            SurfaceComposerClient::openGlobalTransaction();
            mSurfaceControl->setLayer(INT_MAX - 1);
            mSurfaceControl->setPosition(x, y);
            mSurfaceControl->setSize(w,h);
            mSurfaceControl->show();
            SurfaceComposerClient::closeGlobalTransaction(true);

            usleep(33000);

            {
                const int windowStep = 15;
                static bool windowA = true;
                while (true) {
                    if (windowA) {
                        w+=windowStep;
                        h+=windowStep;
                        if (w > (1280/2) || h > (720/2) || (x+w) >= 1280 || (y+h) > 720)
                            windowA = false;
                        break;
                    } else {
                        w-=windowStep;
                        h-=windowStep;
                        if (w < (100) || h < (100))
                            windowA = true;
                        break;
                    }
                }

                const int positionStep = 10;
                static bool positionXA = true;
                while (true) {
                    if (positionXA) {
                        x+=positionStep;
                        if ((x+w) >= 1280)
                            positionXA = false;
                        else
                            break;
                    } else {
                        x-=positionStep;
                        if (x <= 0)
                            positionXA = true;
                        else
                            break;
                    }
                }

                static bool positionYA = true;
                while (true) {
                    if (positionYA) {
                        y+=positionStep;
                        if ((y+h) >= 720)
                            positionYA = false;
                        else
                            break;
                    } else {
                        y-=positionStep;
                        if (y <= 0)
                            positionYA = true;
                        else
                            break;
                    }
                }
            }
        }
    }
}

#if 0
// un-used function
void test_producer_consumer()
{
    printf("[%s]\n", __func__);
    sp<SidebandClient> client;
    client = new SidebandClient();
    int w=100;
    int h=100;
    sp<SidebandControl> control = client->createSurface(String8("Sideband Surface"),w,h,PIXEL_FORMAT_RGB_888);
    CHECK(control != NULL);
    CHECK(control->isValid());

    sp<Surface> surface = control->getSurface();
    CHECK(surface != NULL);
    // get GraphicBufferProducer
    sp<IGraphicBufferProducer> gbp = surface->getIGraphicBufferProducer();
    printf("[Sideband] surface:%p gbp:%p\n",surface.get(), gbp.get());

    IGraphicBufferProducer::QueueBufferOutput qbo;
    // TODO: how to setup crop rectangle?
    IGraphicBufferProducer::QueueBufferInput qbi(0, false, Rect(0, 0, 1, 1), NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, false, Fence::NO_FENCE);

    // TODO: how to setup listener?
    // TODO: how to setup usage?
    gbp->connect(new DummyProducerListener , NATIVE_WINDOW_API_CPU, false, &qbo);

    // set 10 buffers, meanless here..
    status_t rst = gbp->setBufferCount(10);
    printf("[Sideband] setBufferCount rst: %d\n",rst);
    CHECK(rst == NO_ERROR);
    while(1)
    {
        int slot;
        sp<Fence> fence;
        sp<GraphicBuffer> buffer;
        int w=0;
        int h=0;
        // setting for rtk HWLayer
        int format=HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP;
        int usage=GRALLOC_USAGE_HW_ION;

        rst = gbp->dequeueBuffer(&slot, &fence, false, w, h, format,usage);
        printf("[Sideband] dequeue rst %.8x slot %d\n",rst,slot);
        // flag says need requestBuffer
        if(rst & IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION)
        {
            sp<GraphicBuffer> buffer;
            rst = gbp->requestBuffer(slot,&buffer);
            printf("[Sideband] requestBuffer rst %.8x buffer %p\n",rst,buffer.get());
        }

        // queuebuffer
        gbp->queueBuffer(slot, qbi, &qbo);
        printf("[Sideband] queueBuffer\n");
        usleep(500000);
    }
}
#endif

void config_system_properties()
{
    property_set("rtk.hwc.v1.force.enable","1");
    property_set("rtk.hwc.v1.force.fullscreen", "1");

    property_set("rtk.hwc.osd.hide.ms", "1000000");
    property_set("rtk.hwc.osd.hide", "1");
    
}

void print_usage()
{
    printf(USAGE);   
}

bool parse_params(int argc, char *argv[])
{

    if(argc==1)
    {
        print_usage();
        return false;
    }

    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-p")==0)
            testProducer=true;
        if(strcmp(argv[i],"-f")==0)
            testAPIs=true;
    }
    return true;
}

int main(int argc , char *argv[])
{
    srand( (unsigned)time(NULL) );
    //printf("sideband test program\n");
    testProducer = testAPIs = false;
    parse_params(argc, argv);

    if(testProducer)
    {
        //config_system_properties();
        //test_producer_consumer();
        test_surface_producer_consumer();
    }

    if(testAPIs)
    {
        test_client_apis();
    }
#if 0
    gClient = new SidebandClient();
    sp <SidebandControl> gControl = gClient->createSurface(String8("Sideband Surface"),10,10,PIXEL_FORMAT_RGB_888);
    sp <Surface> gSurface = gControl->getSurface();

    printf("[Sideband] Client:%p Control:%p Surface:%p\n",
            gClient.get(), gControl.get(), gSurface.get());
    gClient.clear();
    gSurface.clear();
    printf("[Sideband] Client:%p Control:%p Surface:%p\n",
            gClient.get(), gControl.get(), gSurface.get());
#endif
    printf("goodbye..\n");
    return 0;
}
