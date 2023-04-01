#------------------------------------------------------------------------------
#
# Gnuplot script for atomic vs mutex counter experiment
#
# collect data with gemm.rb
# ./build/atomics/atomic_counter_demo > mutex_cnt.dat
# ./build/atomics/atomic_counter_demo -atomic=1 > atomic_cnt.dat
#
# run plotter with
# > gnuplot -persist -c ./atomics/atomic_counter_demo.plot
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

set term png
set grid
set key left top
set xlabel "number of threads"
set ylabel "time (milliseconds)"

# atomic vs mutex counter
set output "atomic_counter_demo.png"
plot 'mutex_cnt.dat' with linespoints title 'Counter protected by mutex',\
     'atomic_cnt.dat' with linespoints t 'Atomic counter'  
