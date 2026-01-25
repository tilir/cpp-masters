# ./run_rps.rb 2 10 mutex --cmd=./build/mutex_bench rcu --cmd=./build/rcu_bench | tee mutex_rcu.plot
set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "mutex", "-" using 1:2 with linespoints title "rcu"
2 3466050.0
3 2776170.0
4 1786640.0
5 1127550.0
6 801967.0
7 596173.0
8 490197.0
9 436068.0
10 389458.0
e
2 9024790.0
3 7799990.0
4 5990890.0
5 4877450.0
6 4012760.0
7 3366400.0
8 2996090.0
9 2763700.0
10 2479050.0
e
