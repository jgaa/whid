#!/bin/bash

# Compile and prepare a signed .dmg package for distribution
# Assimes the environment-variable QTDIR to point to the
# Qt installation
#
# Example:
#     Jarles-Mac-mini:scripts jgaa$ QTDIR=/Users/jgaa/Qt/5.10.0/clang_64 ./package-macos.sh


if [ -z ${DIST_DIR:-} ];
then
    DIST_DIR=`pwd`/dist/macos
fi

if [ -z ${SIGN_CERT:-} ];
then
    SIGN_CERT="Developer ID Application"
fi

BUILD_DIR=build

if [ -z ${SRC_DIR:-} ];
then
# Just assume we are run from the scipts directory
    SRC_DIR=`pwd`/..
fi

echo "Building whid for macos into ${DIST_DIR} from ${SRC_DIR}"

rm -rf $DIST_DIR

mkdir -p $DIST_DIR && cd $DIST_DIR
mkdir -p $BUILD_DIR

pushd $BUILD_DIR

$QTDIR/bin/qmake \
    -spec macx-clang \
    "CONFIG += release x86_64" \
    $SRC_DIR/whid.pro

make

popd

$QTDIR/bin/macdeployqt build/whid.app -dmg -appstore-compliant -codesign="$SIGN_CERT"

mv build/whid.dmg .

