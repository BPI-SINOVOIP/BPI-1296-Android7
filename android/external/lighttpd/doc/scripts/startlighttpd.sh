#!/system/bin/sh

umask 0
# uncessary environment setup
ln -s /system/data/www/htdocs /data/www/htdocs
ln -s /system/data/www/cgi-bin /data/www/cgi-bin
chmod 0755 /system/data/www/cgi-bin/remote.fcgi
chmod 0755 /system/data/www/cgi-bin/rtCGI.fcgi
chmod 0755 /system/data/www/cgi-bin/togoOutCGI.cgi
chmod 0755 /system/data/www/cgi-bin/togoRead.fcgi
chmod 0755 /system/data/www/cgi-bin/uploadCGI.fcgi

/system/bin/logwrapper /system/bin/lighttpd -D -f /etc/lighttpd/lighttpd.conf -m /system/lib/web-plugins
# if go here, it means the launch is failed. So, retry it again.
#/system/bin/logwrapper echo "HERE HERE....It will try to relaunch lighttpd again after 3 seconds!!"
#sleep 3
#/system/bin/logwrapper /system/bin/lighttpd -D -f /etc/lighttpd/lighttpd.conf -m /system/lib/web-plugins
