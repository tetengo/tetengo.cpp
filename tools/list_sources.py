#! /usr/bin/env python3
# Lists the source files
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

from pathlib import Path

directories = ['executable', 'library']
extensions = ['h', 'hpp', 'c', 'cpp']

def list_iter(root_path, directory, extension):
    path = root_path / directory
    return [p for p in path.glob('**/*.' + extension)]

def list():
    root_path= Path(__file__).parent.parent
    files = []
    for d in directories:
        for e in extensions:
            for f in list_iter(root_path, d, e):
                files.append(f)
    return files
