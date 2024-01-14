#! /bin/sh
# Bootstrap Script
# Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/

aclocal -I m4 && \
autoheader && \
libtoolize --copy && \
automake --add-missing --copy --gnu && \
autoconf
