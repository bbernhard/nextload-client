#!/bin/bash

git clone https://github.com/bbernhard/nextload-client.git /tmp/nextload-client

cd /tmp/nextload-client
git checkout ocdownloader


mkdir -p /tmp/nextload-client/build
cd /tmp/nextload-client/build
qmake ../nextload-client.pro
make
cp -rf /tmp/nextload-client/android/res/drawable/logo.png /tmp/nextload-client/build/logo.png
cp /nextload-client.desktop /tmp/nextload-client/build/nextload-client.desktop
/linuxdeployqt-6-x86_64.AppImage /tmp/nextload-client/build/nextload-client.desktop -qmldir=/tmp/nextload-client/source/qml -bundle-non-qt-libs -appimage

