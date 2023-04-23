#------------------------------------------------------------------------------
#
# Gnuplot script for different std algorithms
#
# collect data with:
# ./build/parallel/algo_bench -alg=reduce -policy=seq >& reduce.seq
# ./build/parallel/algo_bench -alg=reduce -policy=par >& reduce.par
# ./build/parallel/algo_bench -alg=reduce -policy=par_unseq >& reduce.par_unseq
#
# ./build/parallel/algo_bench -alg=transform -policy=seq >& transform.seq
# ./build/parallel/algo_bench -alg=transform -policy=par >& transform.par
# ./build/parallel/algo_bench -alg=transform -policy=par_unseq >& transform.par_unseq
#
# ./build/parallel/algo_bench -alg=sort -policy=seq >& sort.seq
# ./build/parallel/algo_bench -alg=sort -policy=par >& sort.par
# ./build/parallel/algo_bench -alg=sort -policy=par_unseq >& sort.par_unseq
#
# run plotter with
# > gnuplot -persist -c ./parallel/algo_bench.plot
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
set output "reduce.png"
set ylabel "reduce time (milliseconds)"
plot 'reduce.seq' with linespoints title 'seq',\
     'reduce.par' with linespoints title 'par',\
     'reduce.par_unseq' with linespoints title 'par\_unseq'

# transform
set output "transform.png"
set ylabel "transform time (milliseconds)"
plot 'transform.seq' with linespoints title 'seq',\
     'transform.par' with linespoints title 'par',\
     'transform.par_unseq' with linespoints title 'par\_unseq'

# sort
set output "sort.png"
set ylabel "sort time (milliseconds)"
plot 'sort.seq' with linespoints title 'seq',\
     'sort.par' with linespoints title 'par',\
     'sort.par_unseq' with linespoints title 'par\_unseq'
