#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "./release.sh version"
    echo "format for version: x.y.z!"
    exit 1
fi

sed -i "s/hidviz VERSION [0-9]\+.[0-9]\+.[0-9]\+/hidviz VERSION $1/" hidviz/CMakeLists.txt

git add hidviz/CMakeLists.txt
git commit -m "Release version $1"

DIR=${PWD##*/}

if [ "$DIR" == scripts ]; then
    cd ..
fi

git tag "v$1" HEAD

TEMP=`mktemp -d`

HIDVIZ_DIR_NAME="hidviz-$1"

HIDVIZ_TEMP="${TEMP}/${HIDVIZ_DIR_NAME}"

mkdir "$HIDVIZ_TEMP"

git archive HEAD | tar -x -C "$HIDVIZ_TEMP"

cd libhidx

git archive HEAD | tar -x -C "$HIDVIZ_TEMP/libhidx"

cd ..

ORIG_DIR=`pwd`

pushd $TEMP

tar -zcvf "${ORIG_DIR}/hidviz-${1}.tar.gz" "${HIDVIZ_DIR_NAME}"

popd

rm -rf "$TEMP"

echo "Do not forget to push!"
