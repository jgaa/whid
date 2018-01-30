#!/bin/bash

# Compile and prepare a .deb package for distribution
# Uses the QT libraries from the lilnux ditribution
#
# Example:
#   WHID_VERSION="2.0.1" DIST_DIR=`pwd`/dist BUILD_DIR=`pwd`/build SRC_DIR=`pwd`/whid  ./whid/scripts/package-deb.sh

if [ -z "$WHID_VERSION" ]; then
    WHID_VERSION="2.0.0"
    echo "Warning: Missing WHID_VERSION variable!"
fi

if [ -z ${DIST_DIR:-} ]; then
    DIST_DIR=`pwd`/dist/linux
fi

if [ -z ${BUILD_DIR:-} ]; then
    BUILD_DIR=`pwd`/build
fi

if [ -z ${SRC_DIR:-} ]; then
# Just assume we are run from the scipts directory
    SRC_DIR=`pwd`/..
fi

echo "Building whid for linux into ${DIST_DIR} from ${SRC_DIR}"

rm -rf $DIST_DIR $BUILD_DIR

mkdir -p $DIST_DIR &&\
pushd $DIST_DIR &&\
mkdir -p $BUILD_DIR &&\
pushd $BUILD_DIR &&\
qmake $SRC_DIR/whid.pro &&\
make && make install &&\
popd &&\
fpm --input-type dir \
    --output-type deb \
    --force \
    --name whid \
    --version ${WHID_VERSION} \
    --vendor "The Last Viking LTD" \
    --description "Time Tracking for Freelancers and Independenet Contractors" \
    --depends qt5-default --depends libsqqlite3 \
    --chdir ${DIST_DIR}/root/ \
    --package ${DIST_NAME}whid-VERSION_ARCH.deb &&\
echo "Debian package is available in $PWD" &&\
popd
