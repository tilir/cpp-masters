# ./run_rps.rb 2 10 mutex --cmd=./build/mutex_bench sptr --cmd=./build/sptr_bench | tee mutex_sptr.plot
set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "mutex", "-" using 1:2 with linespoints title "sptr"
2 3372940.0
3 2709470.0
4 1801860.0
5 1084450.0
6 801619.0
7 620865.0
8 506515.0
9 415279.0
10 395591.0
e
2 4365330.0
3 2638190.0
4 1736430.0
5 1236500.0
6 886423.0
7 675467.0
8 317831.0
9 277913.0
10 256474.0
e
