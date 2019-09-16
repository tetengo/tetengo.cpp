#! /bin/sh
# clang-format Caller
# Copyright (C) 2019 kaoru

FILEPATH=$1
FILEPATH_TMP=${FILEPATH}.tmp

if test -z ${CLANGFORMAT};
then
    CLANGFORMAT="clang-format"
fi
if test -z ${UNIS2DOS};
then
    UNIX2DOS="unix2dos"
fi

${CLANGFORMAT} ${FILEPATH} > ${FILEPATH_TMP}
${UNIX2DOS} -q ${FILEPATH_TMP}
mv -f ${FILEPATH_TMP} ${FILEPATH}
