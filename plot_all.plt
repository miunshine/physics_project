##### Produces three views of the same graph on seperate pages in one pdf

set terminal pdfcairo					 # generates output in pdf
set output "plot_out_all.pdf"				 # output name
set isosample 80					 # isoline density (grid), high sampling rate = more accurate plots, but takes longer
set dgrid3d 80,80 					 # tells dgrid3d how many entries there are in x,y direction
set contour 						 # tells graph to put on contours, default option is base
set cntrparam levels incremental -1, 0.2, 1		 # controls generation of contours and smoothness for contour plot
set cntrparam levels discrete -0.2, -0.5, 0.2, 0.5	 # controls generation of contours and smoothness for contour plot
set nosurface						 # hides surface
set key							 # enables legend on plot
set pm3d						 # fills with color


##### non angled view, i.e. map view at x_rot = 0 degrees, z_rot = 0 degrees

set view 0,0
splot "out.dat" using 1:2:3 with lines

##### angled view using x_rot = 90 degrees, z_rot = 0 degrees

set view 90,0	# side view
splot "out.dat" using 1:2:3 with lines

##### angled view using x_rot = 60 degrees, z_rot = 45 degrees

set view 60,60	# angled view
splot "out.dat" using 1:2:3 with lines
