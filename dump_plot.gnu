#!/usr/bin/gnuplot

set terminal x11
set title "2D data plot"
unset key
set tic scale 0

# Color runs from white to green
set palette rgbformula -7,-7,2
set cbrange [MINVAL:MAXVAL]
set cblabel "Score"
unset cbtics

set xrange [-0.5:NX.5]
set yrange [-0.5:NY.5]

set view map
splot 'FILE' matrix with image
pause -1
