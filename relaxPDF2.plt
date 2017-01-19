##### angled view using x_rot = 60 degrees, z_rot = 45 degrees

set terminal pdfcairo
set output "relax_out_2.pdf"
set isosample 80
set dgrid3d 80,80 #tells dgrid3d how many entries there are in x,y direction
set contour
set cntrparam levels incremental -1, 0.2, 1
set cntrparam levels discrete -0.2, -0.5, 0.2, 0.5
set contour base
set nosurface
set view 60,45	# angled view
set key
set pm3d #fills with color
splot "relax_out.dat" using 1:2:3 with lines
