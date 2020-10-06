@echo off
rem Makes character_property_map.cpp for tetengo::cli
rem Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

curl https://www.unicode.org/Public/13.0.0/ucd/EastAsianWidth.txt                 > %~dp0\EastAsianWidth.txt
curl http://www.unicode.org/Public/13.0.0/ucd/auxiliary/GraphemeBreakProperty.txt > %~dp0\GraphemeBreakProperty.txt

python3 %~dp0\make_character_property_map.py %~dp0\EastAsianWidth.txt %~dp0\GraphemeBreakProperty.txt %~dp0\character_property_map.txt

python3 %~dp0\make_character_property_map_cpp.py %~dp0\character_property_map.txt %~dp0\..\library\cli\cpp\src\tetengo.cli.character_property_map.cpp
