tetengo 1.0.0
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
- [Python 3](https://www.python.org/)

#### Procedure

Clone the source from GitHub:

```bat
> git clone https://github.com/kaorut/tetengo.git
```

Open the file `tetengo.sln` in the work tree.

Execute the menu command [Build]-[Build Solution] in Visual Studio.

You can "install" tetengo library. Open a command prompt and execute the batch
file `install_tetengo_library.bat` in the directory `tools` in the top
directory of the work tree. That copies the header files and the built library
files into your preferred place.

```bat
> tools\install_tetengo_library.bat [your_preferred_path]
```

To generate the documents, open a Command Prompt, and execute the command
`doxygen` on the top directory of the work tree.
Doxygen will output the documents into the directory `doc` in the work tree.

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
- [Clang 7.0.1](https://clang.llvm.org/),
  [Clang 9.0.0](https://clang.llvm.org/),
  [GCC 8.3.0](https://gcc.gnu.org/) or
  [GCC 9.2.1](https://gcc.gnu.org/)
- [Boost C++ libraries 1.72.0](https://www.boost.org/)
- [Doxygen 1.8.17](http://www.doxygen.nl/)
- [include-what-you-use 0.11](https://include-what-you-use.org/) or
  [include-what-you-use 5.0](https://include-what-you-use.org/)
- [Clang Format 7.0](https://clang.llvm.org/docs/ClangFormat.html) or
  [Clang Format 9](https://clang.llvm.org/docs/ClangFormat.html)
- [Dos2Unix](https://waterlan.home.xs4all.nl/dos2unix.html)
- [Python 3](https://www.python.org/)

#### Procedure

Clone the source from GitHub:

```shell-session
$ git clone https://github.com/kaorut/tetengo.git
```

Run the script `bootstrap.sh` to generate the script `configure`.

```shell-session
$ ./bootstrap.sh
```
Configure, build and install tetengo by the following commands:

```shell-session
$ ./configure
$ make
$ sudo make install
```

By default, tetengo will be installed in the directory `/usr/local`.
To change the directory where tetengo is installed, specify the option
`--prefix` to the script `configure`.

To generate the documents, execute the command `make doc`:

```shell-session
$ make doc
```

---

Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

This product is released under the MIT license.
See [the COPYING file](https://github.com/kaorut/tetengo/blob/master/COPYING)
for details.
