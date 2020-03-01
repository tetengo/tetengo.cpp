tetengo 0.0.0
=============

[TOC]

A multipurpose library set.

It provides class-based libraries for C++ and function-based libraries for C.

Features
--------

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

- [Git](https://git-scm.com/)
- [Visual Studio 2019](https://visualstudio.microsoft.com/)
- [Boost C++ libraries 1.72.0](https://www.boost.org/)
- [Doxygen 1.8.17](http://www.doxygen.nl/)

#### Procedure

Clone from GitHub:

```bat
> git clone https://github.com/kaorut/tetengo.git
```

Open the `tetengo.sln` file in the work tree.

In Visual Studio, execute the menu command: [Build]-[Build Solution].

Add `library\(library name)\c\include` and `library\(library name)\cpp\include`
to the settings "C/C++ - Additional Include Directories" of your Visual C++
project.

Add `bin\(configuration).(platform)` to the settings "Linker - Additional
Library Directories" of your Visual C++ project.
And add `tetengo.(library name).lib` (for C programs) or
`tetengo.(library name).cpp.lib` (for C++ programs) to the settings "Additional
Dependencies".

To generate the documents, open a Command Prompt. Execute `doxygen` command in
the top directory of the work tree. Doxygen will output the documents into the
`doc` directory in the work tree.

```bat
> cd
X:\path\to\tetengo-work-tree

> doxygen
```

### With Clang or GCC on Linux

#### Requirements

- [Git](https://git-scm.com/)
- [Autoconf](https://www.gnu.org/software/autoconf/) and
  [GNU Autoconf Archive](https://www.gnu.org/software/autoconf-archive/)
- [Automake](https://www.gnu.org/software/automake/)
- [GNU Libtool](https://www.gnu.org/software/libtool/)
- [Clang 7.0](https://clang.llvm.org/) or [GCC 8.3.0](https://gcc.gnu.org/)
- [Boost C++ libraries 1.72.0](https://www.boost.org/)
- [Doxygen 1.8.17](http://www.doxygen.nl/)
- [include-what-you-use 0.11](https://include-what-you-use.org/)
- [Clang Format 7.0](https://clang.llvm.org/docs/ClangFormat.html)
- [Dos2Unix](https://waterlan.home.xs4all.nl/dos2unix.html)

#### Procedure

Clone from GitHub:

```shell-session
$ git clone https://github.com/kaorut/tetengo.git
```

Run `bootstrap.sh` to generate `configure` script.

```shell-session
$ ./bootstrap.sh
```
Configure, build and install.

```shell-session
$ ./configure
$ make
$ sudo make install
```

By default, tetengo will be installed in `/usr/local`.
To change the directory where tetengo is installed, specify `--prefix` option
to `configure` script.

To generate the documents, execute `make doc` command.

```shell-session
$ make doc
```

How to Use
----------

### With Visual C++ on Windows

### With Clang or GCC on Linux

---

Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

This product is released under the MIT license.
See [the COPYING file](https://github.com/kaorut/tetengo/blob/master/COPYING)
for details.
