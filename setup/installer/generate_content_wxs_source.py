#! /usr/bin/env python3
"""Generates content.wxs source

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
"""

import glob
import re
import sys
import uuid
from pathlib import Path
from typing import Optional


def main(args: list[str]) -> None:
    """The main function.

    Args:
        args: Program arguments
    """
    if len(args) < 4:
        print(
            "Usage: ./generate_content_wxs_source.py "
            "files_to_install.txt file_guid_map.txt envvars_to_install.txt content_wxs_source.txt solution_path",
            file=sys.stderr,
        )
        sys.exit(0)

    files_to_install_path = Path(args[0])
    file_guid_map_path = Path(args[1])
    envvars_to_install_path = Path(args[2])
    content_wxs_source_path = Path(args[3])
    solution_path = Path(args[4])

    files_to_install = _load_files_to_install(files_to_install_path)
    file_guid_map = _FileGuidMap(file_guid_map_path, solution_path)
    envvars_to_install = _load_envvars_to_install(envvars_to_install_path)

    _make_content_wxs_source(
        files_to_install,
        file_guid_map,
        envvars_to_install,
        content_wxs_source_path,
        solution_path,
    )
    file_guid_map.save(file_guid_map_path)


def _load_files_to_install(
    path: Path,
) -> list[tuple[str, Path, Path]]:
    files: list[tuple[str, Path, Path]] = []
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
                    Path(matched.group(2)),
                    Path(matched.group(3)),
                )
            )
    return files


def _load_envvars_to_install(path: Path) -> list[tuple[str, str, str]]:
    envvars: list[tuple[str, str, str]] = []
    with path.open(mode="r", encoding="UTF-8") as stream:
        for line in stream:
            matched: Optional[re.Match[str]] = re.match(
                "^([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)", line
            )
            if not matched:
                continue
            envvars.append((matched.group(1), matched.group(2), matched.group(3)))
    return envvars


class _FileGuidMap:
    def __init__(self, file_guid_map_path: Path, solution_path: Path):
        self._map: dict[Path, tuple[str, bool]] = {}
        with file_guid_map_path.open(mode="r", encoding="UTF-8") as stream:
            for line in stream:
                line = line.rstrip("\r\n")
                matched: Optional[re.Match[str]] = re.match("^([^ ]+)[ ]+([^ ]+)", line)
                if not matched:
                    continue
                self._map[Path(matched.group(1))] = (matched.group(2), False)

    def guid_of(self, path: Path) -> str:
        if path in self._map:
            self._map[path] = (self._map[path][0], True)
        else:
            self._map[path] = (str(uuid.uuid4()).upper(), True)
        return self._map[path][0]

    def save(self, file_guid_map_path: Path) -> None:
        with file_guid_map_path.open(mode="w", encoding="UTF-8") as stream:
            for path in self._map:
                if self._map[path][1]:
                    print(
                        "{} {}".format(path, self._map[path][0]),
                        file=stream,
                    )


def _make_content_wxs_source(
    files_to_install: list[tuple[str, Path, Path]],
    file_guid_map: _FileGuidMap,
    envvars_to_install: list[tuple[str, str, str]],
    content_wxs_source_path: Path,
    solution_path: Path,
) -> None:
    with content_wxs_source_path.open(mode="w", encoding="UTF-8") as stream:
        for wildcard in files_to_install:
            wildcard_path: Path = solution_path / wildcard[1]
            source_directory: str = str(wildcard_path.parent)
            for extended in glob.glob(str(wildcard_path), recursive=True):
                if not Path(extended).is_file():
                    continue
                extended_file_name: str = extended[len(source_directory) + 1 :]
                print(
                    "file {} {} {} {} {}".format(
                        wildcard[0],
                        source_directory,
                        extended_file_name,
                        wildcard[2],
                        file_guid_map.guid_of(Path(wildcard[2]) / extended_file_name),
                    ),
                    file=stream,
                )
        for envvar in envvars_to_install:
            print(
                "envvar {} {} {}".format(envvar[0], envvar[1], envvar[2]), file=stream
            )


if __name__ == "__main__":
    main(sys.argv[1:])
