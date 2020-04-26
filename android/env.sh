export USE_CCACHE=1
source build/envsetup.sh

ANDROID_CONFIG_FILE=.build_config
CMD_SCRIPT=../cmd_config.sh
if [ -e "$CMD_SCRIPT" ] && [ -e "$ANDROID_CONFIG_FILE" ]; then
    source $CMD_SCRIPT
    old_config_file=${CONFIG_FILE}
    CONFIG_FILE=$ANDROID_CONFIG_FILE
    config_get_all
    CONFIG_FILE=${old_config_file}
fi
