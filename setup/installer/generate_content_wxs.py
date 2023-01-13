#! /usr/bin/env python3
"""Generates content.wxs source

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
"""

import io
import re
import sys
from pathlib import Path
from typing import Any, Optional, TextIO


def main(args: list[str]) -> None:
    """The main function.

    Args:
        args: Program arguments
    """
    if len(args) < 2:
        print(
            "Usage: ./generate_content_wxs.py content_wxs_source.txt content.wxs",
            file=sys.stderr,
        )
        sys.exit(0)

    source_path = Path(args[0])
    wxs_path = Path(args[1])

    destination_tree: _DestinationDirectory = _build_destination_tree(source_path)
    feature_map: dict[str, list[str]] = _build_feature_map(destination_tree)
    _save_wxs(destination_tree, feature_map, wxs_path)


class File:
    """A file.

    Attributes:
        id:      The ID.
        name:    The name.
        feature: The feature.
        guid:    The GUID.
        source:  The source.
    """

    def __init__(self, id: str, name: str, feature: str, guid: str, source: Path):
        """Creates a file.

        Args:
            id:      An ID.
            name:    A name.
            feature: A feature.
            guid:    A GUID.
            source:  A source.
        """
        self.id = id
        self.name = name
        self.feature = feature
        self.guid = guid
        self.source = source


class EnvVar:
    """An environment variable.

    Attributes:
        id:    The ID.
        name:  The name.
        value: The value.
        guid:  The GUID.
    """

    def __init__(self, id: str, name: str, value: str, guid: str):
        """Creates an environment variable.

        Args:
            id:    The ID.
            name:  The name.
            value: The value.
            guid:  The GUID.
        """
        self.id = id
        self.name = name
        self.value = value
        self.guid = guid


class _DestinationDirectory:
    def __init__(self, id: str, name: str, level: int):
        self.id = id
        self.name = name
        self.level = level
        self.children: dict[str, _DestinationDirectory] = {}
        self.files: list[File] = []
        self.envvars: list[EnvVar] = []

    def add_file(self, path: Path, feature: str, guid: str, source_path: Path) -> None:
        if len(path.parents) > 1:
            child_name: str = str(path.parents[len(path.parents) - 2])
            child_id: str = self.id + "." + child_name
            child_id = child_id[-72:]
            if not child_name in self.children:
                self.children[child_name] = _DestinationDirectory(
                    child_id, child_name, self.level + 1
                )
            grandchild_path = Path(str(path)[len(child_name) + 1 :])
            self.children[child_name].add_file(
                grandchild_path, feature, guid, source_path
            )
        else:
            file_name: str = str(path)
            file_id: str = self.id + "." + file_name
            file_id = file_id[-72:]
            self.files.append(File(file_id, file_name, feature, guid, source_path))

    def add_envvar(self, name: str, value: str, guid: str) -> None:
        envvar_id: str = self.id + "." + name
        self.envvars.append(EnvVar(envvar_id, name, value, guid))


def _build_destination_tree(source_path: Path) -> _DestinationDirectory:
    destination_tree = _DestinationDirectory("tetengo", "", 0)
    with source_path.open(mode="r", encoding="UTF-8") as stream:
        for line in stream:
            line = line.rstrip("\r\n")
            matched_as_file: Optional[re.Match[str]] = re.match(
                "^([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)", line
            )
            if matched_as_file:
                kind_as_file: str = matched_as_file.group(1)
                if kind_as_file == "file":
                    feature_as_file: str = matched_as_file.group(2)
                    source_directory_as_file = Path(matched_as_file.group(3))
                    source_path_as_file = Path(matched_as_file.group(4))
                    destination_as_file = Path(matched_as_file.group(5))
                    guid_as_file = matched_as_file.group(6)
                    destination_tree.add_file(
                        destination_as_file / source_path_as_file,
                        feature_as_file,
                        guid_as_file,
                        source_directory_as_file / source_path_as_file,
                    )
            else:
                matched_as_envvar: Optional[re.Match[str]] = re.match(
                    "^([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)[ ]+([^ ]+)", line
                )
                if matched_as_envvar:
                    kind_as_envvar: str = matched_as_envvar.group(1)
                    if kind_as_envvar == "envvar":
                        name_as_envvar: str = matched_as_envvar.group(2)
                        value_as_envvar: str = matched_as_envvar.group(3)
                        guid_as_envvar: str = matched_as_envvar.group(4)
                        destination_tree.add_envvar(
                            name_as_envvar, value_as_envvar, guid_as_envvar
                        )
    return destination_tree


def _build_feature_map(destination_tree: _DestinationDirectory) -> dict[str, list[str]]:
    map: dict[str, list[str]] = {}
    _build_feature_map_iter(destination_tree, map)
    return map


def _build_feature_map_iter(
    destination_tree: _DestinationDirectory, feature_map: dict[str, list[str]]
) -> None:
    for file in destination_tree.files:
        if not file.feature in feature_map:
            feature_map[file.feature] = []
        feature_map[file.feature].append(file.id)
    for child_key in destination_tree.children:
        _build_feature_map_iter(destination_tree.children[child_key], feature_map)
    for envvar in destination_tree.envvars:
        if not "environment_variable" in feature_map:
            feature_map["environment_variable"] = []
        feature_map["environment_variable"].append(envvar.id)


def _save_wxs(
    destination_tree: _DestinationDirectory,
    feature_map: dict[str, list[str]],
    wxs_path: Path,
) -> None:
    preamble: str = """<?xml version="1.0" encoding="UTF-8"?>
<!--
  The installer content
  Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
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
    for envvar in destination_tree.envvars:
        print(
            '{}  <Component Id="{}" Guid="{}">'.format(indent, envvar.id, envvar.guid),
            file=stream,
        )
        print("{}    <CreateFolder/>".format(indent), file=stream)
        print(
            '{}    <Environment Id="{}" Name="{}" Action="set" System="yes" Part="all" Value="{}"/>'.format(
                indent, envvar.id, envvar.name, envvar.value
            ),
            file=stream,
        )
        print("{}  </Component>".format(indent), file=stream)
    if len(destination_tree.name) == 0:
        print("{}</DirectoryRef>".format(indent), file=stream)
    else:
        print("{}</Directory>".format(indent), file=stream)


def _write_feature_fragment(feature_map: dict[str, list[str]], stream: TextIO) -> None:
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
