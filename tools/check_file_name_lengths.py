#! /usr/bin/env python3
# Checks file name lengths
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

import list_sources
import os
import subprocess
import sys


max_length = 80

def main():
    root_path_string = str(list_sources.root())
    for path in list_sources.list():
        path_string = str(path)[len(root_path_string):]
        (path_main_string, extension_string) = os.path.splitext(path)
        if len(path_string) > max_length:
            candidate_path_string = candidate(path_string, extension_string)
            if len(sys.argv) > 1 and sys.argv[1] == "git_mv":
                git_mv(root_path_string, path_string, candidate_path_string)
            else:
                report_too_long(path_string, candidate_path_string)
        elif len(path_string) < max_length and path_main_string.endswith('X'):
            report_too_short(path_string)

def candidate(path_string, extension_string):
    return os.path.splitext(path_string)[0][:max_length - len(extension_string) - 1] + 'X' + extension_string

def git_mv(root_path_string, current_path, corrected_path_string):
    subprocess.run(
        ["git", "mv", root_path_string + current_path, root_path_string + corrected_path_string], check=True
    )

def report_too_long(current_path, corrected_path_string):
    print("Too long path ({} > {}): {}".format(len(current_path), max_length, current_path))
    print("  Candidate: {}".format(corrected_path_string))

def report_too_short(current_path):
    print("Too short path ({} < {}): {}".format(len(current_path), max_length, current_path))

main()
