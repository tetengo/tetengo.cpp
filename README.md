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

Clone the source from GitHub:

```bat
> git clone https://github.com/kaorut/tetengo.git
```

Open the file `tetengo.sln` in the work tree.

In Visual Studio, execute the menu command [Build]-[Build Solution].

You can "install" tetengo library. Open a command prompt and execute the batch
file `install_tetengo_library.bat` in the directory `tools` in the top
directory of the work tree. That copies the header files and the built library
files into your preferred place.

```bat
> tools\install_tetengo_library.bat [your_preferred_path]
```

To generate the documents, open a Command Prompt. Execute the command `doxygen`
on the top directory of the work tree. Doxygen will output the documents into
the directory `doc` in the work tree.

```bat
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

Clone the source from GitHub:

```shell-session
$ git clone https://github.com/kaorut/tetengo.git
```

Run the script `bootstrap.sh` to generate the script `configure`.

```shell-session
$ ./bootstrap.sh
```
Configure, build and install tetengo by the following commands.

```shell-session
$ ./configure
$ make
$ sudo make install
```

By default, tetengo will be installed in `/usr/local`.
To change the directory where tetengo is installed, specify the option
`--prefix` to the script `configure`.

To generate the documents, execute `make doc` command.

```shell-session
$ make doc
```

---

Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

This product is released under the MIT license.
See [the COPYING file](https://github.com/kaorut/tetengo/blob/master/COPYING)
for details.
