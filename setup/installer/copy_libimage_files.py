#!  /usr/bin/env python3
"""Copies the files for a library image

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
"""

import pathlib
import re
import shutil
import sys
from typing import List, Optional, Tuple


def main(args: List[str]) -> None:
    """The main function.

    Args:
        args (list[str]): Program rguments
    """
    if len(args) < 2:
        print(
            "Usage: ./copy_libimage_files.py content_wxs_source.txt destination",
            file=sys.stderr,
        )
        sys.exit(0)
    files: List[Tuple[pathlib.Path, pathlib.Path]] = _list_files(pathlib.Path(args[0]))
    _copy_files(files, pathlib.Path(args[1]))


def _list_files(source_path: pathlib.Path) -> List[Tuple[pathlib.Path, pathlib.Path]]:
    files: List[Tuple[pathlib.Path, pathlib.Path]] = []
    with source_path.open(mode="r", encoding="UTF-8") as stream:
        for line in stream:
            matched: Optional[re.Match[str]] = re.match(
                "^([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)", line
            )
            if not matched:
                continue
            kind: str = matched.group(1)
            if kind == "file":
                feature: str = matched.group(2)
                source_directory = pathlib.Path(matched.group(3))
                source_path = pathlib.Path(matched.group(4))
                destination = pathlib.Path(matched.group(5))
                if (
                    feature != "bin.Win32"
                    and feature != "bin.x64"
                    and feature != "include"
                    and feature != "lib.Release.Win32"
                    and feature != "lib.Release.x64"
                ):
                    continue
                files.append(
                    (source_directory / source_path, (destination / source_path).parent)
                )
    return files


def _copy_files(
    files: List[Tuple[pathlib.Path, pathlib.Path]], destination_root: pathlib.Path
) -> None:
    shutil.rmtree(destination_root, ignore_errors=True)
    for file in files:
        destination_directory: pathlib.Path = destination_root / file[1]
        destination_directory.mkdir(parents=True, exist_ok=True)
        shutil.copy(file[0], destination_directory)


if __name__ == "__main__":
    main(sys.argv[1:])
