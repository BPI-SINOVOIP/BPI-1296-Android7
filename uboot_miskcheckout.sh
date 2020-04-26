#!/bin/bash
SCRIPTDIR=$PWD
source $SCRIPTDIR/build_prepare.sh
ERR=0
config_get CUSTOMER
config_get GIT_SERVER_URL
config_get SDK_BRANCH
config_get USE_RTK_REPO
config_get UBOOT_BRANCH
UBOOTDIR=$SCRIPTDIR/bootcode
MISCDIR=$SCRIPTDIR/qa_supplement
TOOLCHAINDIR=$SCRIPTDIR/toolchain
# set umask here to prevent incorrect file permission
umask 0022

misc_check()
{
	pushd $SCRIPTDIR > /dev/null
	[ ! -d "$UBOOTDIR" ] && git clone ssh://$GIT_SERVER_URL:29418/$CUSTOMER/LK -b $UBOOT_BRANCH
	[ ! -d "$TOOLCHAINDIR" ] && git clone ssh://$GIT_SERVER_URL:29418/SDK_release/toolchain
	popd > /dev/null
	return $ERR;
}

if [ "$1" = "" ]; then
    echo "$0 commands are:"
	echo "   checkout       "
else
    while [ "$1" != "" ]
    do
        case "$1" in
			
			checkout)
				build_cmd misc_check
				;;
            *)
                echo -e "$0 \033[47;31mUnknown CMD: $1\033[0m"
                exit 1
                ;;
        esac
        shift 1
    done
fi


