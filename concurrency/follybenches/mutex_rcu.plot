# ./run_rps.rb 2 10 mutex --cmd=./build/mutex_bench rcu --cmd=./build/rcu_bench | tee mutex_rcu.plot
set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "mutex", "-" using 1:2 with linespoints title "rcu"
2 3441110.0
3 2678150.0
4 1771880.0
5 1133900.0
6 766760.0
7 581535.0
8 480965.0
9 415878.0
10 376899.0
e
2 7485560.0
3 7451880.0
4 5957440.0
5 5175660.0
6 4629030.0
7 4006320.0
8 3541040.0
9 3165250.0
10 2832050.0
e
