##### angled view using x_rot = 90 degrees, z_rot = 0 degrees

set terminal pdfcairo
set output "plot_out_1.pdf"
set isosample 80
set dgrid3d 80,80 #tells dgrid3d how many entries there are in x,y direction
set contour
set cntrparam levels incremental -1, 0.2, 1
set cntrparam levels discrete -0.2, -0.5, 0.2, 0.5
set contour base
set nosurface
set view 90,0   # side view
set key
set pm3d #fills with color
splot "out.dat" using 1:2:3 with lines
