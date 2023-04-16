#------------------------------------------------------------------------------
#
# Gnuplot script for atomic vs mutex counter experiment
#
# queue under load
# ./build/queues/classic_queue_bench -nthreads=20 -ntasks=10000 -ptime=1 -ctime=1 > classic_1x1.dat
# ./build/atomics/lockfree_bench -nthreads=20 -ntasks=10000 -ptime=1 -ctime=1 > lockfree_1x1.dat
#
# measuring just contention
# ./build/queues/classic_queue_bench -nthreads=30 -ntasks=100000 -ptime=0 -ctime=0 > classic_0x0.dat
# ./build/atomics/lockfree_bench -nthreads=30 -ntasks=100000 -ptime=0 -ctime=0 > lockfree_0x0.dat
#
# run plotter with
# > gnuplot -persist -c ./atomics/lockfree_comparison.plot
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

# lock-free under workload (10000 tasks)
set output "lockfree_comparison_load.png"
plot 'classic_1x1.dat' with linespoints title 'Classic queue, 1024, 10000',\
     'lockfree_1x1.dat' with linespoints title 'Lock-free queue, 1024, 10000'

# lock-free contention only (100000 tasks)
set output "lockfree_comparison_contention.png"
plot 'classic_0x0.dat' with linespoints title 'Classic queue, 1024, 100000',\
     'lockfree_0x0.dat' with linespoints title 'Lock-free queue, 1024, 100000'
