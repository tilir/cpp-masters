# ./run_rps.rb 2 10 mutex --cmd=./build/mutex_bench rwlock --cmd=./build/rwlock_bench | tee mutex_rwlock.plot
set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "mutex", "-" using 1:2 with linespoints title "rwlock"
2 3546940.0
3 2801840.0
4 1748030.0
5 1139530.0
6 803807.0
7 577255.0
8 481988.0
9 409973.0
10 361575.0
e
2 4731710.0
3 2775370.0
4 2158470.0
5 1993900.0
6 1785410.0
7 1568050.0
8 1398410.0
9 1264550.0
10 1166940.0
e
