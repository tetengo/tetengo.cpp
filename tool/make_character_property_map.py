#! /usr/bin/env python
"""Makes character_property_map.txt for tetengo::text

    Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/
"""

import re
import sys
from pathlib import Path
from typing import Optional


def main(args: list[str]) -> None:
    """The main function.

    Args:
        args: Program arguments
    """
    if len(args) < 3:
        print(
            "Usage: ./make_character_property_map.py EastAsianWidth.txt emoji-data.txt GraphemeBreakProperty.txt character_property_map.txt",
            file=sys.stderr,
        )
        sys.exit(0)
    east_asian_width_map: list[str] = _load_file(Path(args[0]), "N")
    emoji_data_map: list[str] = _load_file(Path(args[1]), "N")
    grapheme_map: list[str] = _load_file(Path(args[2]), "Other")
    _save_file(Path(args[3]), east_asian_width_map, emoji_data_map, grapheme_map)


def _load_file(path: Path, default_value: str) -> list[str]:
    map: list[str] = [default_value for code in range(0x110000)]
    with path.open(mode="r", encoding="UTF-8") as stream:
        for line in stream:
            line = _remove_comment(line)
            if not line:
                continue
            matched: Optional[re.Match[str]] = re.match(
                "^([0-9A-F]+)(\.\.([0-9A-F]+))?\s*;\s*([A-Za-z]+)", line
            )
            if not matched:
                continue
            code_from = int(matched.group(1), 16)
            if matched.group(3):
                code_to = int(matched.group(3), 16)
            else:
                code_to = int(matched.group(1), 16)
            value: str = matched.group(4)
            for code in range(code_from, code_to + 1):
                if map[code] == default_value:
                    map[code] = value
    return map


def _remove_comment(line: str) -> str:
    line = line.rstrip("\r\n")
    if "#" in line:
        line = line[0 : line.find("#")]
    line = line.rstrip(" ")
    return line


def _save_file(
    path: Path,
    east_asian_width_map: list[str],
    emoji_data_map: list[str],
    grapheme_map: list[str],
) -> None:
    with path.open(mode="w", newline="\r\n", encoding="UTF-8") as stream:
        previous_value: str = ""
        for code in range(0x110000):
            value: str = "{}\t{}\t{}".format(
                east_asian_width_map[code], emoji_data_map[code], grapheme_map[code]
            )
            if value != previous_value:
                stream.write("{}\t{}\n".format(format(code, "04X"), value))
                previous_value = value


if __name__ == "__main__":
    main(sys.argv[1:])
