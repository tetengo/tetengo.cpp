#!/bin/sh
# Makes character_property_map.cpp for tetengo::cli
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

TOOLS_DIR=`dirname $0`

curl https://www.unicode.org/Public/13.0.0/ucd/EastAsianWidth.txt                 > $TOOLS_DIR/EastAsianWidth.txt
curl http://www.unicode.org/Public/13.0.0/ucd/auxiliary/GraphemeBreakProperty.txt > $TOOLS_DIR/GraphemeBreakProperty.txt

$TOOLS_DIR/make_character_property_map.py $TOOLS_DIR/EastAsianWidth.txt $TOOLS_DIR/GraphemeBreakProperty.txt $TOOLS_DIR/character_property_map.txt

$TOOLS_DIR/make_character_property_map_cpp.py $TOOLS_DIR/character_property_map.txt $TOOLS_DIR/../library/cli/cpp/src/tetengo.cli.character_property_map.cpp
