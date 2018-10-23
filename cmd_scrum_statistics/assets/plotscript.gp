reset

set terminal png font arial 14 size 800,600
set boxwidth 0.2
set style fill solid

set style line 1 lt rgb "#A00000" lw 2 pt 1
set style line 2 lt rgb "#00A000" lw 2 pt 6
set style line 3 lt rgb "#5060D0" lw 2 pt 2
set style line 4 lt rgb "#F25900" lw 2 pt 9

# Line style for axes
set style line 80 lt rgb "#808080"

# Line style for grid
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey

set grid back linestyle 81
set border 3 back linestyle 80
set xtics nomirror
set ytics nomirror

plot './data.csv' every 1    using 2:1 t "CpuErt" with linespoints ls 1,\
     './data.csv' every 1::2 using 2:1 t "GpuErt" with linespoints ls 2,\
     './data.csv' every 1::3 using 2:1 t "HybridErt" with linespoints ls 3
