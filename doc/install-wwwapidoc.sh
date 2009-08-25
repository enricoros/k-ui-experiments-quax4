#!/bin/sh

export LANG=C
export LC_ALL=C

outdir="../../quax-html/apidoc/"
htmldir="../doxygen/html/"
latexdir="../doxygen/latex/"

# regenerate API documents
( cd .. && doxygen )

# copy HTML doc
rsync -av --delete --exclude-from=apiexcludes $htmldir $outdir

# generate DVI, PDF, PS from  API documentation
( cd $latexdir && make pdf && make ps )

cp $latexdir/refman.pdf $outdir/quax-apidoc.pdf
cp $latexdir/refman.ps $outdir/quax-apidoc.ps
cp $latexdir/refman.dvi $outdir/quax-apidoc.dvi
