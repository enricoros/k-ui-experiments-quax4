#!/bin/sh

export LANG=C
export LC_ALL=C

outfile="quax.html"
tmpfile="/tmp/quaxdoc.$$"
xsl="/opt/kde3/share/apps/ksgmltools2/customization/kde-nochunk.xsl"
meinproc -o $outfile --stylesheet $xsl quax.docbook

# fix the CSS link
cat $outfile | sed 's,\.\./common/kde-web\.css,quaxdoc.css,g' > $tmpfile
mv $tmpfile $outfile
