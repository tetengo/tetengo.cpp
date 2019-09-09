#! /bin/sh
# Bootstrap Script
# Copyright (C) 2019 kaoru

aclocal && \
autoheader && \
automake --add-missing --copy --gnu && \
autoconf
