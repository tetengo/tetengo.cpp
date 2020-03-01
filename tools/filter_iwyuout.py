#! /usr/bin/env python3
# IWYU output filter
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

import re
import sys


def main():
    no_error_pattern = re.compile('has correct #includes/fwd-decls\)$')

    exit_code = 0
    for line in sys.stdin:
        line = line.rstrip('\n')

        if len(line) == 0:
            continue
        if no_error_pattern.search(line):
            continue

        print(line)
        exit_code = 1
    
    exit(exit_code)


main()
