
set title 'result'

set terminal pdf
set output 'relax.pdf'

#set contour 	
#set cntrparam levels incremental -900, 50, 900

set nosurface
set key
set pm3d map

splot 'relax_out.dat' using 1:2:3 with image, 
plot 'vector_field.dat' using 1:2:3:4 with vectors


set isosam 31,31
set view map
set dgrid3d 80 80
unset surface

set contour base
set cntrparam levels incr -900,10,900

set table 'equipotential_lines.txt'
splot 'relax_out.dat' using 1:2:3  w l ls 1
unset table

plot 'equipotential_lines.txt' u 1:2 w l ls 1
