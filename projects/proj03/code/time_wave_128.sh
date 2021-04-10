#!/bin/bash

echo "time wave p=128"
echo "=================="
echo "n nt t_total n_threads" > time_wave_128.csv

alpha=1
mx=13
my=7
nt=100
NTRIALS=3

for i in {1..$NTRIALS}
do
	./wave_timing 301 $mx $my $alpha $nt 128 >> time_wave_128.csv

	./wave_timing 501 $mx $my $alpha $nt 128 >> time_wave_128.csv

	./wave_timing 1001 $mx $my $alpha $nt 128 >> time_wave_128.csv

	./wave_timing 2501 $mx $my $alpha $nt 128 >> time_wave_128.csv

	./wave_timing 5001 $mx $my $alpha $nt 128 >> time_wave_128.csv

	./wave_timing 10001 $mx $my $alpha $nt 128 >> time_wave_128.csv

	./wave_timing 25001 $mx $my $alpha $nt 128 >> time_wave_128.csv
done

echo ""
echo ""

