#------------------------------------------------------------------------------
#
# Gnuplot script for different std algorithms
#
# collect data with:
# ./build/parallel/reduce_bench -alg=reduce -policy=par >& reduce.data
# ./build/parallel/reduce_bench -alg=reduce -policy=par >& reduce_unseq.data
# ./build/parallel/reduce_bench -alg=naive >& naive.data
# ./build/parallel/reduce_bench -alg=async >& async.data
#
# run plotter with
# > gnuplot -persist -c ./parallel/reduce_bench.plot
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
set xlabel "number of elements"

# reduce
set output "reduce_unseq.png"
set ylabel "reduce time (milliseconds)"
plot 'reduce.data' with linespoints title 'seq',\
     'reduce_unseq.data' with linespoints title 'unseq'

# reduce
set output "reduce.png"
set ylabel "reduce time (milliseconds)"
plot 'reduce_unseq.data' with linespoints title 'reduce\_unseq',\
     'naive.data' with linespoints title 'naive',\
     'async.data' with linespoints title 'async'