tetengo 1.3.1
=============

[TOC]

A multipurpose library set.

It provides class-based libraries for C++ and function-based libraries for C.

Features
--------

### tetengo::json

A JSON parser library.

It is a pull-style parser based on
[RFC 8259](https://tools.ietf.org/html/rfc8259), plus an extensional support
for UNIX shell style comments.

### tetengo::lattice

A Viterbi search library.

The Viterbi search is a dynamic programming algorithm. It finds the most likely
path in the lattice consisted of the observed event nodes.

This library also provides the A* search algorithm for the lattice created by
the Viterbi search.

### tetengo::text

A text library.

The utility for text processings.

This library provides the following features:

- A string encoding converter.
- A grapheme splitter conforming to
  [Unicode 13.0.0](https://unicode.org/versions/Unicode13.0.0/).

### tetengo::trie

A trie library.

The trie is an associative data structure.
Given a key, it returns the corresponding value in a constant time.

The trie is also able to perform a prefix search.
It has a method to enumerates the values with the same prefix.

How to Build and Install
------------------------

### With Visual C++ on Windows

#### Requirements

- [Visual Studio 2019](https://visualstudio.microsoft.com/)
- [Boost C++ libraries 1.75.0](https://www.boost.org/)
- [Doxygen 1.8.20](https://www.doxygen.nl/)
- [Graphviz](https://www.graphviz.org/)
- [Python 3](https://www.python.org/)

If you build the source cloned from the GitHub repository, the following is
necessary in addition the above tools.

- [Git](https://git-scm.com/)

#### Getting built library files

##### Downloading the prebuilt library files

Download the .zip archive file for Windows from the website
[www.tetengo.org](https://www.tetengo.org/).

Expand the archive into your preferred path.

##### Building the source files cloned from the GitHub repository

Clone the source files from GitHub:

```bat
> git clone https://github.com/kaorut/tetengo.git
```

Open the file `tetengo.sln` in the work tree.

Execute the menu command [Build]-[Build Solution] in Visual Studio.

#### Installing

You can "install" tetengo library. Open a command prompt and execute the batch
file `install_tetengo_library.bat` in the directory `tool` in the top
directory. That copies the header files and the built library files into your
preferred path.

```bat
> tool\install_tetengo_library.bat [your_preferred_path]
```

#### Generating documents

To generate the documents, open a command prompt, and execute the command
`doxygen` in the top directory.
Doxygen will output the documents into the directory `doc`.

```bat
> doxygen
```

### With Clang or GCC on Linux

#### Requirements

- [Clang 7.0.1](https://clang.llvm.org/),
  [Clang 9.0.0](https://clang.llvm.org/),
  [GCC 8.3.0](https://gcc.gnu.org/) or
  [GCC 9.2.1](https://gcc.gnu.org/)
- [Boost C++ libraries 1.75.0](https://www.boost.org/)
- [Doxygen 1.8.20](https://www.doxygen.nl/)
- [Graphviz](https://www.graphviz.org/)
- [include-what-you-use 0.11](https://include-what-you-use.org/) or
  [include-what-you-use 5.0](https://include-what-you-use.org/)
- [Clang Format 7.0](https://clang.llvm.org/docs/ClangFormat.html) or
  [Clang Format 9](https://clang.llvm.org/docs/ClangFormat.html)
- [Dos2Unix](https://waterlan.home.xs4all.nl/dos2unix.html)
- [Python 3](https://www.python.org/)

If you build the source cloned from the GitHub repository, the followings are
necessary in addition the above tools.

- [Git](https://git-scm.com/)
- [Autoconf](https://www.gnu.org/software/autoconf/) and
  [GNU Autoconf Archive](https://www.gnu.org/software/autoconf-archive/)
- [Automake](https://www.gnu.org/software/automake/)
- [GNU Libtool](https://www.gnu.org/software/libtool/)

#### Getting the source files

##### Downloading the source files from www.tetengo.org

Download the .tar.bz2 archive file for Linux from the website
[www.tetengo.org](https://www.tetengo.org/).

Expand the archive into your preferred path.

```shell-session
$ tar -xf tetengo-X.Y.Z.tar.bz2
```

##### Cloning the source files from the GitHub repository

Clone the source files from GitHub:

```shell-session
$ git clone https://github.com/kaorut/tetengo.git
```

Run the script `bootstrap.sh` in the top directory to generate the script
`configure`.

```shell-session
$ ./bootstrap.sh
```

#### Building and installing

Configure, build and install tetengo by the following commands:

```shell-session
$ ./configure
$ make
$ sudo make install
```

By default, tetengo will be installed in the directory `/usr/local`.
To change the directory where tetengo is installed, specify the option
`--prefix` to the script `configure`.

#### Generating documents

To generate the documents, execute the command `make doc` in the top directory:

```shell-session
$ make doc
```

---

Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

This product is released under the MIT license.
See [the COPYING file](https://github.com/kaorut/tetengo/blob/master/COPYING)
for details.
