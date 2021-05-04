#! /usr/bin/env python3
"""Generates content.wxs source

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
"""

import glob
import pathlib
import re
import sys
import uuid
from typing import Dict, List, Optional, Tuple


def main(args: List[str]) -> None:
    """The main function.

    Args:
        args (list[str]): Program rguments
    """
    if len(args) < 4:
        print(
            "Usage: ./generate_content_wxs_source.py "
            "files_to_install.txt file_guid_map.txt content_wxs_source.txt solution_path",
            file=sys.stderr,
        )
        sys.exit(0)

    files_to_install_path = pathlib.Path(args[0])
    file_guid_map_path = pathlib.Path(args[1])
    content_wxs_source_path = pathlib.Path(args[2])
    solution_path = pathlib.Path(args[3])

    files_to_install = _load_files_to_install(files_to_install_path)
    file_guid_map = _FileGuidMap(file_guid_map_path, solution_path)

    _make_content_wxs_source(
        files_to_install, file_guid_map, content_wxs_source_path, solution_path
    )
    file_guid_map.save(file_guid_map_path)


def _load_files_to_install(
    path: pathlib.Path,
) -> List[Tuple[str, pathlib.Path, pathlib.Path]]:
    files: List[Tuple[str, pathlib.Path, pathlib.Path]] = []
    with path.open(mode="r", encoding="UTF-8") as stream:
        for line in stream:
            line = line.rstrip("\r\n")
            matched: Optional[re.Match[str]] = re.match(
                "^([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)", line
            )
            if not matched:
                continue
            files.append(
                (
                    matched.group(1),
                    pathlib.Path(matched.group(2)),
                    pathlib.Path(matched.group(3)),
                )
            )
    return files


class _FileGuidMap:
    _map: Dict[pathlib.Path, Tuple[str, bool]] = {}

    def __init__(self, file_guid_map_path: pathlib.Path, solution_path: pathlib.Path):
        with file_guid_map_path.open(mode="r", encoding="UTF-8") as stream:
            for line in stream:
                line = line.rstrip("\r\n")
                matched: Optional[re.Match[str]] = re.match("^([^ ]+)[ ]+([^ ]+)", line)
                if not matched:
                    continue
                self._map[pathlib.Path(matched.group(1))] = (matched.group(2), False)

    def guid_of(self, path: pathlib.Path) -> str:
        if path in self._map:
            self._map[path] = (self._map[path][0], True)
        else:
            self._map[path] = (str(uuid.uuid4()).upper(), True)
        return self._map[path][0]

    def save(self, file_guid_map_path: pathlib.Path) -> None:
        with file_guid_map_path.open(mode="w", encoding="UTF-8") as stream:
            for path in self._map:
                if self._map[path][1]:
                    print(
                        "{} {}".format(path, self._map[path][0]),
                        file=stream,
                    )


def _make_content_wxs_source(
    files_to_install: List[Tuple[str, pathlib.Path, pathlib.Path]],
    file_guid_map: _FileGuidMap,
    content_wxs_source_path: pathlib.Path,
    solution_path: pathlib.Path,
) -> None:
    with content_wxs_source_path.open(mode="w", encoding="UTF-8") as stream:
        for wildcard in files_to_install:
            wildcard_path: pathlib.Path = solution_path / wildcard[1]
            source_directory: str = str(wildcard_path.parent)
            for extended in glob.glob(str(wildcard_path), recursive=True):
                if not pathlib.Path(extended).is_file():
                    continue
                extended_file_name: str = extended[len(source_directory) + 1 :]
                print(
                    "{} {} {} {} {}".format(
                        wildcard[0],
                        source_directory,
                        extended_file_name,
                        wildcard[2],
                        file_guid_map.guid_of(
                            pathlib.Path(wildcard[2]) / extended_file_name
                        ),
                    ),
                    file=stream,
                )


def _remove_solution_path(
    path: pathlib.Path, solution_path: pathlib.Path
) -> pathlib.Path:
    path_str = str(path)
    solution_path_str = str(solution_path)
    if path_str.find(solution_path_str) != 0:
        return path
    removed = path_str[len(solution_path_str) :]
    if len(solution_path_str) > 0 and removed[0] == "\\":
        removed = removed[1:]
    return pathlib.Path(removed)


if __name__ == "__main__":
    main(sys.argv[1:])
