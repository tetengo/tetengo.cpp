#! /bin/sh
# Bootstrap Script
# Copyright (C) 2019 kaoru

aclocal -I m4 && \
autoheader && \
libtoolize --copy && \
automake --add-missing --copy --gnu && \
autoconf
