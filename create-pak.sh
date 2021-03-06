#!/bin/bash

VERSION=`cat configure.ac | grep AC_INIT | sed 's!.*\[\([0-9]\+\.[0-9]\+\.[0-9]\+\)\].*!\1!'`
#VERSION=`echo "AC_INIT([asdf], [30.3.0], [asdf])" | sed 's!.*\([0-9]\.[0-9]\.[0-9]\).*!\1!'`

if [ -z "$VERSION" ]; then
  echo "cannot find version, exiting."
  exit 1
fi

echo "version is $VERSION"
echo "7z a -tzip pak-$VERSION.pke -i@pak-$VERSION.include"
echo -n "3 "
sleep 1
echo -n "2 "
sleep 1
echo -n "1 "
sleep 1
echo -n "0"
echo
cd data/
rm -f pak-$VERSION.pke
7z a -tzip pak-$VERSION.pke -i@pak-$VERSION.include
