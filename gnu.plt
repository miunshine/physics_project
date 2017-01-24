set pm3d map
set title 'result'

set terminal pdf
set output 'relax.pdf'

splot 'relax_out.dat' using 2:1:3 with image
