# Autoconf Settings
# Copyright (C) 2019 kaoru

#### General ####
AC_PREREQ([2.69])
AC_INIT([tetengo], [0.0.0], [kaorut@mc.pikara.ne.jp])
AC_CONFIG_SRCDIR([precompiled/precompiled.hpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([
    Makefile
    docsrc/Makefile
    precompiled/Makefile
    tools/Makefile
    trie/Makefile
    trie/c/Makefile
    trie/c/include/Makefile
    trie/c/include/tetengo/Makefile
    trie/c/src/Makefile
    trie/cpp/Makefile
    trie/cpp/include/Makefile
    trie/cpp/include/tetengo/Makefile
    trie/cpp/src/Makefile
    trie/test/Makefile
    trie/test/src/Makefile
    vsprops/Makefile
])

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

#### Program Checks ####
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB

AC_ARG_WITH(
    doxygen,
    AS_HELP_STRING([--with-doxygen=COMMAND], [Specify the Doxygen command. Or uses `doxygen'.]),
    DOXYGEN=$withval
)
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen)
test -z $DOXYGEN && \
    AC_MSG_ERROR([You cannot generate documents for lack of Doxygen.])

#### Library Checks ####
AX_BOOST_BASE([1.71.0])
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

#### Output ####
AC_OUTPUT
