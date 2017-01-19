PROG='no_grid'


#remove previous .dat files
rm -f  relax_out.dat   relax.pdf

#create new .dat files
touch relax_out.dat 	

#compile, run
g++ -o ${PROG} ${PROG}.c
./${PROG}

#gnuplot 
gnuplot relax.plt

okular relax.pdf &

