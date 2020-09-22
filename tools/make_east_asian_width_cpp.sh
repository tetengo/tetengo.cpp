#!/bin/sh
# Makes east_asian_width.cpp for tetengo::cli
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

TOOLS_DIR=`dirname $0`

curl https://www.unicode.org/Public/13.0.0/ucd/EastAsianWidth.txt > $TOOLS_DIR/EastAsianWidth.txt
$TOOLS_DIR/make_east_asian_width_cpp.py < $TOOLS_DIR/EastAsianWidth.txt
