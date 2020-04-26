APP_STL := stlport_static
#APP_STL := gnustl_static
#STLPORT_FORCE_REBUILD := true
APP_CPPFLAGS += -fexceptions

# Now Android NDK 8 or later support:
#    APP_ABI := armeabi armeabi-v7a mips x64
# if you want to compile for all platforms, you can assign:
#    APP_ABI := all
APP_ABI := armeabi mips

APP_PLATFORM := android-9

APP_MODULES := RtkRpcClientServer

APP_OPTIM := release
