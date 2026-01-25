set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "mutex", "-" using 1:2 with linespoints title "hazptr"
2 3331060.0
3 2635270.0
4 1797800.0
5 1119500.0
6 821863.0
7 611059.0
8 489462.0
9 444871.0
10 383017.0
e
2 23909700.0
3 21496400.0
4 16914200.0
5 14569300.0
6 13386300.0
7 11263700.0
8 9654680.0
9 8796290.0
10 7869610.0
e
