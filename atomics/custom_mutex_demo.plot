#------------------------------------------------------------------------------
#
# Gnuplot script for mutex vs custom futex
#
# collect data with gemm.rb
# ./build/atomics/custom_mutex_demo -type=0 > type0.dat
# ./build/atomics/custom_mutex_demo -type=1 > type1.dat
# ./build/atomics/custom_mutex_demo -type=2 > type2.dat
#
# ./build/atomics/custom_mutex_demo -type=0 -max=20 > type0.20.dat
# ./build/atomics/custom_mutex_demo -type=2 -max=20 > type2.20.dat
#
# run plotter with
# > gnuplot -persist -c ./atomics/custom_mutex_demo.plot
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

# mutex vs futex data
set output "custom_mutex_demo.png"
plot 'type0.dat' with linespoints title 'std::mutex',\
     'type2.dat' with linespoints title 'gentle futex',\
     'type1.dat' with linespoints title 'greedy futex'

# mutex vs futex data (20 points)
set output "custom_mutex_demo.20.png"
plot 'type0.20.dat' with linespoints title 'std::mutex',\
     'type2.20.dat' with linespoints title 'gentle futex'
