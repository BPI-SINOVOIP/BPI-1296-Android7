#!/bin/ash

# Arguments: SERVICE COMMAND
# COMMAND: enable, disable

[ "$#" != "2" ] && exit 1

CMD=$2

if [ -f "/etc/init.d/$1" ]; then
  /etc/init.d/$1 $CMD
else
  exit 1
fi
