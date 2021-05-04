#! /usr/bin/env python3
"""Generates content.wxs source

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
"""

import io
import pathlib
import re
import sys
from typing import Dict, List, Optional, TextIO, Tuple


def main(args: List[str]) -> None:
    """The main function.

    Args:
        args (list[str]): Program rguments
    """
    if len(args) < 2:
        print(
            "Usage: ./generate_content_wxs.py content_wxs_source.txt content.wxs",
            file=sys.stderr,
        )
        sys.exit(0)

    source_path = pathlib.Path(args[0])
    wxs_path = pathlib.Path(args[1])

    destination_tree: _DestinationDirectory = _build_destination_tree(source_path)
    feature_map: Dict[str, List[str]] = _build_feature_map(destination_tree)
    _save_wxs(destination_tree, feature_map, wxs_path)


class File:
    id: str

    name: str

    feature: str

    guid: str

    source: pathlib.Path

    def __init__(
        self, id: str, name: str, feature: str, guid: str, source: pathlib.Path
    ):
        self.id = id
        self.name = name
        self.feature = feature
        self.guid = guid
        self.source = source


class _DestinationDirectory:
    id: str

    name: str

    level: int

    children: Dict

    files: List[File]

    def __init__(self, id: str, name: str, level: int):
        self.id = id
        self.name = name
        self.level = level
        self.children: Dict[str, _DestinationDirectory] = {}
        self.files = []

    def add(
        self, path: pathlib.Path, feature: str, guid: str, source_path: pathlib.Path
    ) -> None:
        if len(path.parents) > 1:
            child_name: str = str(path.parents[len(path.parents) - 2])
            child_id: str = self.id + "." + child_name
            child_id = child_id[-72:]
            if not child_name in self.children:
                self.children[child_name] = _DestinationDirectory(
                    child_id, child_name, self.level + 1
                )
            grandchild_path = pathlib.Path(str(path)[len(child_name) + 1 :])
            self.children[child_name].add(grandchild_path, feature, guid, source_path)
        else:
            file_name: str = str(path)
            file_id: str = self.id + "." + file_name
            file_id = file_id[-72:]
            self.files.append(File(file_id, file_name, feature, guid, source_path))


def _build_destination_tree(source_path: pathlib.Path) -> _DestinationDirectory:
    destination_tree = _DestinationDirectory("tetengo", "", 0)
    with source_path.open(mode="r", encoding="UTF-8") as stream:
        for line in stream:
            line = line.rstrip("\r\n")
            matched: Optional[re.Match[str]] = re.match(
                "^([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)", line
            )
            if not matched:
                continue
            feature: str = matched.group(1)
            source_directory = pathlib.Path(matched.group(2))
            source_path = pathlib.Path(matched.group(3))
            destination = pathlib.Path(matched.group(4))
            guid = matched.group(5)
            destination_tree.add(
                destination / source_path, feature, guid, source_directory / source_path
            )
    return destination_tree


def _build_feature_map(destination_tree: _DestinationDirectory) -> Dict[str, List[str]]:
    map: Dict[str, List[str]] = {}
    _build_feature_map_iter(destination_tree, map)
    return map


def _build_feature_map_iter(
    destination_tree: _DestinationDirectory, feature_map: Dict[str, List[str]]
) -> None:
    for file in destination_tree.files:
        if not file.feature in feature_map:
            feature_map[file.feature] = []
        feature_map[file.feature].append(file.id)
    for child_key in destination_tree.children:
        _build_feature_map_iter(destination_tree.children[child_key], feature_map)


def _save_wxs(
    destination_tree: _DestinationDirectory,
    feature_map: Dict[str, List[str]],
    wxs_path: pathlib.Path,
) -> None:
    preamble: str = """<?xml version="1.0" encoding="UTF-8"?>
<!--
  The installer content
  Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
-->
<Wix xmlns="http://schemas.microsoft.com/wix/2006/wi">"""
    postamble: str = """</Wix>"""
    with wxs_path.open(mode="w", encoding="UTF-8") as stream:
        print(preamble, file=stream)
        _write_directory_fragment(destination_tree, stream)
        _write_feature_fragment(feature_map, stream)
        print(postamble, file=stream)


def _write_directory_fragment(
    destination_tree: _DestinationDirectory, stream: TextIO
) -> None:
    print('    <Fragment Id="DirectoryTree">', file=stream)
    _write_directory_fragment_iter(destination_tree, stream)
    print("    </Fragment>", file=stream)


def _write_directory_fragment_iter(
    destination_tree: _DestinationDirectory, stream: TextIO
) -> None:
    indent: str = "        " + "    " * destination_tree.level
    if len(destination_tree.name) == 0:
        print('{}<DirectoryRef Id="INSTALLDIR">'.format(indent), file=stream)
    else:
        print(
            '{}<Directory Id="{}" Name="{}">'.format(
                indent, destination_tree.id, destination_tree.name
            ),
            file=stream,
        )
    for child_key in destination_tree.children:
        _write_directory_fragment_iter(destination_tree.children[child_key], stream)
    for file in destination_tree.files:
        print(
            '{}  <Component Id="{}" Guid="{}">'.format(indent, file.id, file.guid),
            file=stream,
        )
        print(
            '{}    <File Id="{}" Name="{}" Source="{}"/>'.format(
                indent, file.id, file.name, file.source
            ),
            file=stream,
        )
        print("{}  </Component>".format(indent), file=stream)
    if len(destination_tree.name) == 0:
        print("{}</DirectoryRef>".format(indent), file=stream)
    else:
        print("{}</Directory>".format(indent), file=stream)


def _write_feature_fragment(feature_map: Dict[str, List[str]], stream: TextIO):
    print('    <Fragment Id="Features">', file=stream)
    print('        <Feature Id="All">', file=stream)
    for feature in feature_map:
        print('            <Feature Id="{}">'.format(feature), file=stream)
        for id in feature_map[feature]:
            print('                <ComponentRef Id="{}"/>'.format(id), file=stream)
        print("            </Feature>", file=stream)

    print("        </Feature>", file=stream)
    print("    </Fragment>", file=stream)


if __name__ == "__main__":
    main(sys.argv[1:])
