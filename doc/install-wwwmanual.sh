#!/bin/sh

export LANG=C
export LC_ALL=C

ver=`cat ../VERSION`
tmpdir="../../quax-manual-$ver"
outdir="../../quax-html/manual/"

# create an archive with HTML doc
mkdir -p $tmpdir
cp -f *.html *.png *.css $tmpdir
(cd ../../ && tar cfz quax/doc/quax-manual-$ver.tar.gz quax-manual-$ver/ )
rm -rf  $tmpdir

# sync the handbook files
rsync -av . --delete --exclude-from=manexcludes $outdir
