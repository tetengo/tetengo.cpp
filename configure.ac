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
    precompiled/Makefile
    trie/Makefile
    trie/cpp/Makefile
    trie/cpp/include/Makefile
    trie/cpp/include/tetengo/Makefile
    trie/cpp/src/Makefile
])

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

#### Program Checks ####
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB

#### Library Checks ####
AX_BOOST_BASE([1.71.0])
AX_BOOST_UNIT_TEST_FRAMEWORK
AC_SUBST([LDFLAGS], "${LDFLAGS} ${BOOST_LDFLAGS}")
#AC_SUBST([UNITTEST_LIBS] "${BOOST_UNIT_TEST_FRAMEWORK_LIB}")

AC_SUBST([LIBS], "${LIBS} -lstdc++fs -stdc++")

#### Header Checks ####
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_HEADER_STDC

AC_SUBST([CPPFLAGS], "${CPPFLAGS} ${BOOST_CPPFLAGS}")

#### Compilation Options ####
AC_SUBST([CXXFLAGS], "${CXXFLAGS} -Werror -Wall -Wextra -pedantic-errors")

#### Output ####
AC_OUTPUT
