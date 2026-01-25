set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "rcu", "-" using 1:2 with linespoints title "hazard"
2 9297800.0
3 8120740.0
4 6602020.0
5 5882140.0
6 4977700.0
7 4223040.0
8 3706680.0
9 3368140.0
10 3148640.0
e
2 20423800.0
3 21642000.0
4 18195900.0
5 15582500.0
6 13090000.0
7 11868600.0
8 10140800.0
9 8964170.0
10 8246020.0
e
