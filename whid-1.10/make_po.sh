#!/bin/sh

xgettext  -k_ -o translations/whid.po src/client/*.cpp
xgettext -j -k_ -o translations/whid_no_bm_.po src/client/*.cpp

