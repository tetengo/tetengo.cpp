#! /usr/bin/env python3
# Makes precompiled.h
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

import list_sources


def main():
    for f in list_sources.list():
        print(f)

main()
