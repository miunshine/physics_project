set terminal postscript eps size 5.5,4.62 enhanced color font 'Helvetica,20' linewidth 4
set output "gaussian.eps"
set isosample 80
set dgrid3d 80, 80 #tells dgrid3d how many entries there are in x,y direction
set contour
set cntrparam levels 10
set cntrparam levels incremental -1, 0.2, 1
set cntrparam levels discrete -0.2, -0.5, 0.2, 0.5
set contour base
set nosurface
set view 0,0
set key
set pm3d #fills with color
splot "out.dat" using 1:2:3 with lines

