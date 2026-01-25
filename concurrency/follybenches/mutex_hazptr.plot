# ./run_rps.rb 2 10 mutex --cmd=./build/mutex_bench hazptr --cmd=./build/hazptr_bench | tee mutex_hazptr.plot
set term pngcairo size 1500,600
set grid
set xlabel "Number of readers"
set ylabel "RPS per reader"
set title "Per-reader throughput vs readers"
plot "-" using 1:2 with linespoints title "mutex", "-" using 1:2 with linespoints title "hazptr"
2 3475990.0
3 2742950.0
4 1823650.0
5 1171460.0
6 820817.0
7 593329.0
8 493185.0
9 432180.0
10 383498.0
e
2 24838500.0
3 18692700.0
4 17413100.0
5 14792900.0
6 11044600.0
7 9634420.0
8 8581420.0
9 7879810.0
10 6374540.0
e
