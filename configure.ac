# Autoconf Settings
# Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/

#### General ####
AC_PREREQ([2.69])
AC_INIT([tetengo], [1.9.0], [https://github.com/tetengo/tetengo.cpp])
AC_CONFIG_SRCDIR([precompiled/precompiled.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([--warnings=no-portability])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_FILES([
    Makefile
    docsrc/Makefile
    executable/Makefile
    executable/setup/Makefile
    executable/setup/res/Makefile
    executable/setup/src/Makefile
    kogyan/Makefile
    kogyan/tool/Makefile
    kogyan/vsprops/Makefile
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
    library/platform_dependent/Makefile
    library/platform_dependent/cpp/Makefile
    library/platform_dependent/cpp/include/Makefile
    library/platform_dependent/cpp/include/tetengo/Makefile
    library/platform_dependent/cpp/src/Makefile
    library/platform_dependent/cpp/src/unixos/Makefile
    library/platform_dependent/cpp/src/windows/Makefile
    library/platform_dependent/test/Makefile
    library/platform_dependent/test/src/Makefile
    library/property/Makefile
    library/property/c/Makefile
    library/property/c/include/Makefile
    library/property/c/include/tetengo/Makefile
    library/property/c/src/Makefile
    library/property/cpp/Makefile
    library/property/cpp/include/Makefile
    library/property/cpp/include/tetengo/Makefile
    library/property/cpp/src/Makefile
    library/property/test/Makefile
    library/property/test/src/Makefile
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
    sample/Makefile
    sample/json2timetable/Makefile
    sample/json2timetable/data/Makefile
    sample/json2timetable/src/Makefile
    sample/make_dict/Makefile
    sample/make_dict/src/Makefile
    sample/search_dict/Makefile
    sample/search_dict/src/Makefile
    sample/search_dict_c/Makefile
    sample/search_dict_c/src/Makefile
    sample/transfer_trains/Makefile
    sample/transfer_trains/data/Makefile
    sample/transfer_trains/src/Makefile
    setup/Makefile
    setup/installer/Makefile
    tool/Makefile
])

AC_LANG([C++])
#AX_CXX_COMPILE_STDCXX(17, [noext], [mandatory]) # C++20 is not supported yet.

#### Program Checks ####
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
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
AC_SUBST([IWYU_OPTS_CXX], "-Xiwyu --max_line_length=200 -x c++ -DIWYU")
AC_SUBST([IWYU_OPTS_C],   "-Xiwyu --max_line_length=200 -x c -DIWYU")
AC_SUBST([IWYU_IMP_PATH], "kogyan/tool/iwyu_mappings.imp")

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
    AC_MSG_WARN([You cannot format the C++ source files for lack of unix2dos and/or clang-format.])

AC_ARG_WITH(
    black,
    AS_HELP_STRING([--with-black=COMMAND], [Specify the black command. Or uses `black'.]),
    BLACK=$withval
)
AC_CHECK_PROG(BLACK, black, black)
test -z $BLACK && \
    AC_MSG_WARN([You cannot format the python script files for lack of black.])

AC_ARG_WITH(
    isort,
    AS_HELP_STRING([--with-isort=COMMAND], [Specify the isort command. Or uses `isort'.]),
    ISORT=$withval
)
AC_CHECK_PROG(ISORT, isort, isort)
test -z $ISORT && \
    AC_MSG_WARN([You cannot format the python script files for lack of isort.])
AC_SUBST([ISORT_OPTS], "--profile black")

AC_ARG_WITH(
    mypy,
    AS_HELP_STRING([--with-mypy=COMMAND], [Specify the mypy command. Or uses `mypy'.]),
    MYPY=$withval
)
AC_CHECK_PROG(MYPY, mypy, mypy)
test -z $MYPY && \
    AC_MSG_WARN([You cannot check the python script files for lack of mypy.])
AC_SUBST([MYPY_OPTS], "--strict")

AC_ARG_WITH(
    doxygen,
    AS_HELP_STRING([--with-doxygen=COMMAND], [Specify the Doxygen command. Or uses `doxygen'.]),
    DOXYGEN=$withval
)
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen)
test -z $DOXYGEN && \
    AC_MSG_WARN([You cannot generate documents for lack of Doxygen.])

#### Library Checks ####
AX_PTHREAD
AC_SUBST([LIBS], "${PTHREAD_LIBS} ${LIBS}")
AC_SUBST([CFLAGS], "${CFLAGS} ${PTHREAD_CFLAGS}")
AC_SUBST([CXXFLAGS], "${CXXFLAGS} ${PTHREAD_CFLAGS}")
AC_SUBST([CC], "${PTHREAD_CC}")

AX_BOOST_BASE([1.87.0])
AX_BOOST_UNIT_TEST_FRAMEWORK
AC_SUBST([LDFLAGS], "${LDFLAGS} ${BOOST_LDFLAGS}")
AC_SUBST([LIBS], "-lstdc++fs -lstdc++ ${LIBS}")

#### Header Checks ####
AC_SUBST([CPPFLAGS], "${CPPFLAGS} ${BOOST_CPPFLAGS}")

#### Compilation Options ####
AC_SUBST([CXXFLAGS_IWYU], "${CXXFLAGS} -std=c++20 -Werror -Wall -Wextra -pedantic-errors")
AC_SUBST([CXXFLAGS], "${CXXFLAGS} -std=c++20 -Werror -Wall -Wextra -pedantic-errors")
AC_SUBST([CFLAGS], "${CFLAGS} -std=c17 -Werror -Wall -Wextra -pedantic-errors")

#### Output ####
AC_OUTPUT
