# Autoconf Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

#### General ####
AC_PREREQ([2.69])
AC_INIT([tetengo], [1.3.0], [https://github.com/kaorut])
AC_CONFIG_SRCDIR([precompiled/precompiled.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([--warnings=no-portability])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_FILES([
    Makefile
    docsrc/Makefile
    executable/Makefile
    executable/json2timetable/Makefile
    executable/json2timetable/data/Makefile
    executable/json2timetable/src/Makefile
    executable/make_dict/Makefile
    executable/make_dict/src/Makefile
    executable/search_dict/Makefile
    executable/search_dict/src/Makefile
    executable/search_dict_c/Makefile
    executable/search_dict_c/src/Makefile
    executable/transfer_trains/Makefile
    executable/transfer_trains/data/Makefile
    executable/transfer_trains/src/Makefile
    library/Makefile
    library/json/Makefile
    library/json/c/Makefile
    library/json/c/include/Makefile
    library/json/c/include/tetengo/Makefile
    library/json/c/src/Makefile
    library/json/cpp/Makefile
    library/json/cpp/include/Makefile
    library/json/cpp/include/tetengo/Makefile
    library/json/cpp/src/Makefile
    library/json/test/Makefile
    library/json/test/src/Makefile
    library/lattice/Makefile
    library/lattice/c/Makefile
    library/lattice/c/include/Makefile
    library/lattice/c/include/tetengo/Makefile
    library/lattice/c/src/Makefile
    library/lattice/cpp/Makefile
    library/lattice/cpp/include/Makefile
    library/lattice/cpp/include/tetengo/Makefile
    library/lattice/cpp/src/Makefile
    library/lattice/test/Makefile
    library/lattice/test/src/Makefile
    library/text/Makefile
    library/text/c/Makefile
    library/text/c/include/Makefile
    library/text/c/include/tetengo/Makefile
    library/text/c/src/Makefile
    library/text/cpp/Makefile
    library/text/cpp/include/Makefile
    library/text/cpp/include/tetengo/Makefile
    library/text/cpp/src/Makefile
    library/text/test/Makefile
    library/text/test/src/Makefile
    library/trie/Makefile
    library/trie/c/Makefile
    library/trie/c/include/Makefile
    library/trie/c/include/tetengo/Makefile
    library/trie/c/src/Makefile
    library/trie/cpp/Makefile
    library/trie/cpp/include/Makefile
    library/trie/cpp/include/tetengo/Makefile
    library/trie/cpp/src/Makefile
    library/trie/test/Makefile
    library/trie/test/src/Makefile
    precompiled/Makefile
    tools/Makefile
    vsprops/Makefile
])

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

#### Program Checks ####
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LIBTOOL
LT_INIT

AC_ARG_WITH(
    iwyu,
    AS_HELP_STRING(
        [--with-iwyu=COMMAND], [Specify the include-what-you-use command. Or uses `include-what-you-use'.]
    ),
    IWYU=$withval
)
AC_CHECK_PROG(IWYU, include-what-you-use, include-what-you-use)
test -z $IWYU && \
    AC_MSG_WARN([You cannot check the includes for lack of include-what-you-use.])
AC_SUBST([IWYU_OPTS_CXX], "-Xiwyu --max_line_length=200 -x c++ -std=c++17 -DIWYU")
AC_SUBST([IWYU_OPTS_C],   "-Xiwyu --max_line_length=200 -x c -std=c89 -DIWYU")
AC_SUBST([IWYU_IMP_PATH], "tools/iwyu_mappings.imp")

AC_ARG_WITH(
    unix2dos,
    AS_HELP_STRING(
        [--with-unix2dos=COMMAND], [Specify the unix2dos command. Or uses `unix2dos'.]
    ),
    UNIX2DOS=$withval
)
AC_CHECK_PROG(UNIX2DOS, unix2dos, unix2dos)
AC_ARG_WITH(
    clang-format,
    AS_HELP_STRING(
        [--with-clang-format=COMMAND], [Specify the clang-format command. Or uses `clang-format'.]
    ),
    CLANGFORMAT=$withval
)
AC_CHECK_PROG(CLANGFORMAT, clang-format, clang-format)
(test -z $UNIX2DOS || test -z $CLANGFORMAT) && \
    AC_MSG_WARN([You cannot format the source files for lack of unix2dos and/or clang-format.])

AC_ARG_WITH(
    doxygen,
    AS_HELP_STRING([--with-doxygen=COMMAND], [Specify the Doxygen command. Or uses `doxygen'.]),
    DOXYGEN=$withval
)
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen)
test -z $DOXYGEN && \
    AC_MSG_WARN([You cannot generate documents for lack of Doxygen.])

#### Library Checks ####
AC_CHECK_LIB(pthread, pthread_cancel)
AX_BOOST_BASE([1.74.0])
AX_BOOST_UNIT_TEST_FRAMEWORK
AC_SUBST([LDFLAGS], "${LDFLAGS} ${BOOST_LDFLAGS}")
AC_SUBST([LIBS], "${LIBS} -lstdc++fs -lstdc++")

#### Header Checks ####
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_HEADER_STDC

AC_SUBST([CPPFLAGS], "${CPPFLAGS} ${BOOST_CPPFLAGS}")

#### Compilation Options ####
AC_SUBST([CXXFLAGS], "${CXXFLAGS} -Werror -Wall -Wextra -pedantic-errors")
AC_SUBST([CFLAGS], "${CFLAGS} -std=c89 -Werror -Wall -Wextra -pedantic-errors")

#### Output ####
AC_OUTPUT
