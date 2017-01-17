#remove previous .dat files
rm -f  result.dat result.gnu  result.pdf

#create new .dat files
touch result.dat	result.gnu

#compile, run
g++ -o numsolver numsolver.c
./numsolver

echo -e "

set pm3d map
set title 'result'

set terminal pdf
set output 'result.pdf'


plot 'result.dat' using 1:2:3 with image " >> result.gnu


#gnuplot 
gnuplot result.gnu


