#!/bin/bash
cd $1
CONVERT=1
if [ -e MKStringTable.txt ]; then
	CONVERT=1
	if [ -e .MKStringTable.txt ]; then
		diff .MKStringTable.txt MKStringTable.txt > /dev/null 2>&1
		if [ $? -eq 0 ]; then
			echo "nothing changed"
			CONVERT=0
		fi
	fi
else
	echo "no MKStringTable.txt found do nothing"
	CONVERT=0
fi

if [ $CONVERT -eq 1 ]; then
	iconv -f UTF16 -t UTF8 MKStringTable.txt > MKStringTable.utf8
	dos2unix MKStringTable.utf8
	$ANDROID_HOST_OUT/bin/multilan
	rm -f MKStringTable.utf8
	echo "convert complete, save checkpoint"
	cp MKStringTable.txt .MKStringTable.txt
fi

cd -
