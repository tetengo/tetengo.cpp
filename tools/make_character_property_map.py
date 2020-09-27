#! /usr/bin/env python3
# Makes character_property_map.txt for tetengo::cli
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

from pathlib import Path
import re
import sys


def main():
    if len(sys.argv) <= 3:
        print(
            "Usage: ./make_character_property_map_cpp.py EastAsianWidth.txt GraphemeBreakProperty.txt character_property_map.txt",
            file=sys.stderr
        )
        sys.exit(0)
    east_asian_width_map = load_file(Path(sys.argv[1]), 'N')
    grapheme_map = load_file(Path(sys.argv[2]), 'Other')
    save_file(Path(sys.argv[3]), east_asian_width_map, grapheme_map)

def load_file(path, default_value):
    map = [ default_value for code in range(0x110000) ]
    with path.open(mode="r") as stream:
        for line in stream:
            line = remove_comment(line)
            if not line:
                continue
            matched = re.match("^([0-9A-F]+)(\.\.([0-9A-F]+))?\s*;\s*([A-Za-z]+)", line)
            code_from = int(matched.group(1), 16)
            if matched.group(3):
                code_to = int(matched.group(3), 16)
            else:
                code_to = int(matched.group(1), 16)
            value = matched.group(4)
            for code in range(code_from, code_to + 1):
                map[code] = value
    return map

def remove_comment(line):
    line = line.rstrip("\r\n")
    if "#" in line:
        line = line[0:line.find("#")]
    line = line.rstrip(" ")
    return line

def save_file(path, east_asian_width_map, grapheme_map):
    with path.open(mode='w', newline='\r\n') as stream:
        previous_value = ''
        for code in range(0x110000):
            value = "{}\t{}".format(east_asian_width_map[code], grapheme_map[code])
            if value != previous_value:
                stream.write("{}\t{}\n".format(format(code, '04X'), value))
                previous_value = value

main()
