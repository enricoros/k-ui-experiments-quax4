#!/bin/sh

export QTDIR=/usr/lib/qt-3.1

# get the current version & release
version=`cat  VERSION | cut -d- -f 1`
release=`cat  VERSION | cut -d- -f 2`

# make sure there's no objects and binaries arround
make clean
rm -f src/quax

# reconfigure things just to make sure rpmspec is good
./configure

# make a new snapshot
mkdir -p ../quax-$version-$release
cp -r . ../quax-$version-$release/

# jump a step behind
cd ..

# make archive from it and then delete local copy
tar cfz quax-$version-$release.tar.gz quax-$version-$release
rm -r quax-$version-$release

# build the RPM
cp -f  quax/quaxrh90.spec /usr/src/redhat/SPECS
cp -f  quax-$version-$release.tar.gz /usr/src/redhat/SOURCES
rpmbuild -ba /usr/src/redhat/SPECS/quaxrh90.spec
rm -f  /usr/src/redhat/SPECS/quaxrh90.spec
rm -f  /usr/src/redhat/SOURCES/quax-$version-$release.tar.gz

# back to work...
cd quax

