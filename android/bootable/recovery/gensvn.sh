#!/bin/bash
echo 'Test haha'
svn info | grep Revision | awk '{ printf("#define REVISION \"%s\"\n",$$0) }' > ver.h
