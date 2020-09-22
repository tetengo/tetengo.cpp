@echo off
rem Makes east_asian_width.cpp for tetengo::cli
rem Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

curl https://www.unicode.org/Public/13.0.0/ucd/EastAsianWidth.txt > %~dp0\EastAsianWidth.txt
%~dp0\make_east_asian_width_cpp.py < %~dp0\EastAsianWidth.txt
